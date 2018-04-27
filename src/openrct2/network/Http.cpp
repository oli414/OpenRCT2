#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#include "http.h"

#ifdef DISABLE_HTTP

void http_init() { }
void http_dispose() { }

#else

#include <cstring>
#include <thread>

#include "../core/Console.hpp"
#include "../core/Math.hpp"
#include "../Version.h"

#ifdef _WIN32
    // cURL includes windows.h, but we don't need all of it.
    #define WIN32_LEAN_AND_MEAN
#endif
#include <curl/curl.h>

#define MIME_TYPE_APPLICATION_JSON "application/json"
#define OPENRCT2_USER_AGENT "OpenRCT2/" OPENRCT2_VERSION

struct HttpRequest2
{
    void *          Tag = nullptr;
    std::string     Method;
    std::string     Url;
    HTTP_DATA_TYPE  Type;
    bool            ForceIPv4 = false;
    size_t          Size = 0;
    union
    {
        char *      Buffer = nullptr;
        json_t *    Json;
    } Body;

    HttpRequest2() { }

    HttpRequest2(const HttpRequest2 &request)
    {
        Tag = request.Tag;
        Method = request.Method;
        Url = request.Url;
        Type = request.Type;
        ForceIPv4 = request.ForceIPv4;
        Size = request.Size;
        if (request.Type == HTTP_DATA_JSON)
        {
            Body.Json = json_deep_copy(request.Body.Json);
        }
        else
        {
            Body.Buffer = new char[request.Size];
            memcpy(Body.Buffer, request.Body.Buffer, request.Size);
        }
    }

    explicit HttpRequest2(const http_request_t * request)
    {
        Tag = request->tag;
        Method = std::string(request->method);
        Url = std::string(request->url);
        Type = request->type;
        ForceIPv4 = request->forceIPv4;
        Size = request->size;
        if (request->type == HTTP_DATA_JSON)
        {
            Body.Json = json_deep_copy(request->root);
        }
        else
        {
            Body.Buffer = new char[request->size];
            memcpy(Body.Buffer, request->body, request->size);
        }
    }

    ~HttpRequest2()
    {
        if (Type == HTTP_DATA_JSON)
        {
            json_decref(Body.Json);
        }
        else
        {
            delete Body.Buffer;
        }
    }
};

struct read_buffer {
    char *ptr;
    size_t length;
    size_t position;
};

struct write_buffer {
    char *ptr;
    size_t length;
    size_t capacity;
};

void http_init()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

void http_dispose()
{
    curl_global_cleanup();
}

static size_t http_request_write_func(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    write_buffer *writeBuffer = (write_buffer*)userdata;

    size_t newBytesLength = size * nmemb;
    if (newBytesLength > 0) {
        size_t newCapacity = writeBuffer->capacity;
        size_t newLength = writeBuffer->length + newBytesLength;
        while (newLength > newCapacity) {
            newCapacity = Math::Max<size_t>(4096, newCapacity * 2);
        }
        if (newCapacity != writeBuffer->capacity) {
            writeBuffer->ptr = (char*)realloc(writeBuffer->ptr, newCapacity);
            writeBuffer->capacity = newCapacity;
        }

        memcpy(writeBuffer->ptr + writeBuffer->length, ptr, newBytesLength);
        writeBuffer->length = newLength;
    }
    return newBytesLength;
}

static http_response_t *http_request(const HttpRequest2 &request)
{
    CURL *curl;
    CURLcode curlResult;
    http_response_t *response;
    read_buffer readBuffer = { nullptr };
    write_buffer writeBuffer;

    curl = curl_easy_init();
    if (curl == nullptr)
        return nullptr;

    if (request.Type == HTTP_DATA_JSON && request.Body.Json != nullptr) {
        readBuffer.ptr = json_dumps(request.Body.Json, JSON_COMPACT);
        readBuffer.length = strlen(readBuffer.ptr);
        readBuffer.position = 0;
    } else if (request.Type == HTTP_DATA_RAW && request.Body.Buffer != nullptr) {
        readBuffer.ptr = request.Body.Buffer;
        readBuffer.length = request.Size;
        readBuffer.position = 0;
    }

    writeBuffer.ptr = nullptr;
    writeBuffer.length = 0;
    writeBuffer.capacity = 0;

    curl_slist *headers = nullptr;

    if (request.Type == HTTP_DATA_JSON) {
        headers = curl_slist_append(headers, "Accept: " MIME_TYPE_APPLICATION_JSON);

        if (request.Body.Json != nullptr) {
            headers = curl_slist_append(headers, "Content-Type: " MIME_TYPE_APPLICATION_JSON);
        }
    }

    if (readBuffer.ptr != nullptr) {
        char contentLengthHeaderValue[64];
        snprintf(contentLengthHeaderValue, sizeof(contentLengthHeaderValue), "Content-Length: %zu", readBuffer.length);
        headers = curl_slist_append(headers, contentLengthHeaderValue);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, readBuffer.ptr);
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.Method.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, OPENRCT2_USER_AGENT);
    curl_easy_setopt(curl, CURLOPT_URL, request.Url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeBuffer);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_request_write_func);
    if (request.ForceIPv4)
    {
        // Force resolving to IPv4 to fix issues where advertising over IPv6 does not work
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    }

    curlResult = curl_easy_perform(curl);

    if (request.Type == HTTP_DATA_JSON && request.Body.Json != nullptr) {
        free(readBuffer.ptr);
    }

    if (curlResult != CURLE_OK) {
        log_error("HTTP request failed: %s.", curl_easy_strerror(curlResult));
        if (writeBuffer.ptr != nullptr)
            free(writeBuffer.ptr);

        return nullptr;
    }

    long httpStatusCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpStatusCode);

    char* contentType;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);

    // Null terminate the response buffer
    writeBuffer.length++;
    writeBuffer.ptr = (char*)realloc(writeBuffer.ptr, writeBuffer.length);
    writeBuffer.capacity = writeBuffer.length;
    writeBuffer.ptr[writeBuffer.length - 1] = 0;

    response = nullptr;

    // Parse as JSON if response is JSON
    if (contentType != nullptr && strstr(contentType, "json") != nullptr) {
        json_t *root;
        json_error_t error;
        root = json_loads(writeBuffer.ptr, 0, &error);
        if (root != nullptr) {
            response = (http_response_t*) malloc(sizeof(http_response_t));
            response->tag = request.Tag;
            response->status_code = (sint32) httpStatusCode;
            response->root = root;
            response->type = HTTP_DATA_JSON;
            response->size = writeBuffer.length;
        }
        free(writeBuffer.ptr);
    } else {
        response = (http_response_t*) malloc(sizeof(http_response_t));
        response->tag = request.Tag;
        response->status_code = (sint32) httpStatusCode;
        response->body = writeBuffer.ptr;
        response->type = HTTP_DATA_RAW;
        response->size = writeBuffer.length;
    }

    curl_easy_cleanup(curl);

    return response;
}

void http_request_async(const http_request_t * request, void (*callback)(http_response_t*))
{
    auto request2 = HttpRequest2(request);
    auto thread = std::thread([](const HttpRequest2 &req, void(*callback2)(http_response_t*)) -> void
    {
        http_response_t * response = http_request(req);
        callback2(response);
    }, std::move(request2), callback);
    thread.detach();
}

void http_request_dispose(http_response_t *response)
{
    if (response->type == HTTP_DATA_JSON && response->root != nullptr)
        json_decref(response->root);
    else if (response->type == HTTP_DATA_RAW && response->body != nullptr)
        free(response->body);

    free(response);
}

const char *http_get_extension_from_url(const char *url, const char *fallback)
{
    const char *extension = strrchr(url, '.');

    // Assume a save file by default if no valid extension can be determined
    if (extension == nullptr || strchr(extension, '/') != nullptr) {
        return fallback;
    } else {
        return extension;
    }
}

size_t http_download_park(const char * url, void * * outData)
{
    // Download park to buffer in memory
    HttpRequest2 request;
    request.Url = url;
    request.Method = "GET";
    request.Type = HTTP_DATA_NONE;

    http_response_t *response = http_request(request);

    if (response == nullptr || response->status_code != 200) {
        Console::Error::WriteLine("Failed to download '%s'", request.Url.c_str());
        if (response != nullptr) {
            http_request_dispose(response);
        }

        *outData = nullptr;
        return 0;
    }

    size_t dataSize = response->size - 1;
    void * data = malloc(dataSize);
    if (data == nullptr) {
        dataSize = 0;
        Console::Error::WriteLine("Failed to allocate memory for downloaded park.");
    } else {
        memcpy(data, response->body, dataSize);
    }

    http_request_dispose(response);

    *outData = data;
    return dataSize;
}

#endif
