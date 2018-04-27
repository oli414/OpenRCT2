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

#include <cwctype>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "../localisation/Language.h"
#include "../util/Util.h"

#include "Math.hpp"
#include "Memory.hpp"
#include "String.hpp"

namespace String
{
    std::string ToStd(const utf8 * str)
    {
        if (str == nullptr) return std::string();
        else return std::string(str);
    }

    std::string StdFormat(const utf8 * format, ...)
    {
        va_list args;
        va_start(args, format);
        const utf8 * buffer = Format_VA(format, args);
        va_end(args);
        std::string returnValue = ToStd(buffer);
        Memory::Free(buffer);
        return returnValue;
    }

    std::string ToUtf8(const std::wstring &s)
    {
        std::string result;
        utf8 * cstr = widechar_to_utf8(s.c_str());
        if (cstr != nullptr)
        {
            result = std::string(cstr);
        }
        free(cstr);
        return result;
    }

    std::wstring ToUtf16(const std::string &s)
    {
        std::wstring result;
        wchar_t * wcstr = utf8_to_widechar(s.c_str());
        if (wcstr != nullptr)
        {
            result = std::wstring(wcstr);
        }
        free(wcstr);
        return result;
    }

    bool IsNullOrEmpty(const utf8 * str)
    {
        return str == nullptr || str[0] == '\0';
    }

    sint32 Compare(const std::string &a, const std::string &b, bool ignoreCase)
    {
        return Compare(a.c_str(), b.c_str(), ignoreCase);
    }

    sint32 Compare(const utf8 * a, const utf8 * b, bool ignoreCase)
    {
        if (a == b) return 0;
        if (a == nullptr) a = "";
        if (b == nullptr) b = "";
        if (ignoreCase)
        {
            return _stricmp(a, b);
        }
        else
        {
            return strcmp(a, b);
        }
    }

    bool Equals(const std::string &a, const std::string &b, bool ignoreCase)
    {
        return Equals(a.c_str(), b.c_str(), ignoreCase);
    }

    bool Equals(const utf8 * a, const utf8 * b, bool ignoreCase)
    {
        if (a == b) return true;
        if (a == nullptr || b == nullptr) return false;

        if (ignoreCase)
        {
            return _stricmp(a, b) == 0;
        }
        else
        {
            return strcmp(a, b) == 0;
        }
    }

    bool StartsWith(const utf8 * str, const utf8 * match, bool ignoreCase)
    {
        if (ignoreCase)
        {
            while (*str != '\0' && *match != '\0')
            {
                if (tolower(*str++) != tolower(*match++))
                {
                    return false;
                }
            }
            return true;
        }
        else
        {
            while (*str != '\0' && *match != '\0')
            {
                if (*str++ != *match++)
                {
                    return false;
                }
            }
            return true;
        }
    }

    bool StartsWith(const std::string &str, const std::string &match, bool ignoreCase)
    {
        return StartsWith(str.c_str(), match.c_str(), ignoreCase);
    }

    size_t IndexOf(const utf8 * str, utf8 match, size_t startIndex)
    {
        const utf8 * ch = str + startIndex;
        for (; *ch != '\0'; ch++)
        {
            if (*ch == match)
            {
                return (size_t)(ch - str);
            }
        }
        return SIZE_MAX;
    }

    ptrdiff_t LastIndexOf(const utf8 * str, utf8 match)
    {
        const utf8 * lastOccurance = nullptr;
        const utf8 * ch = str;
        for (; *ch != '\0'; ch++)
        {
            if (*ch == match)
            {
                lastOccurance = ch;
            }
        }

        if (lastOccurance == nullptr)
        {
            return -1;
        }
        else
        {
            return lastOccurance - str;
        }
    }

    size_t LengthOf(const utf8 * str)
    {
        return utf8_length(str);
    }

    size_t SizeOf(const utf8 * str)
    {
        return strlen(str);
    }

    utf8 * Set(utf8 * buffer, size_t bufferSize, const utf8 * src)
    {
        return safe_strcpy(buffer, src, bufferSize);
    }

    utf8 * Set(utf8 * buffer, size_t bufferSize, const utf8 * src, size_t srcSize)
    {
        utf8 * dst = buffer;
        size_t minSize = Math::Min(bufferSize - 1, srcSize);
        for (size_t i = 0; i < minSize; i++)
        {
            *dst++ = *src;
            if (*src == '\0') break;
            src++;
        }
        *dst = '\0';
        return buffer;
    }

    utf8 * Append(utf8 * buffer, size_t bufferSize, const utf8 * src)
    {
        return safe_strcat(buffer, src, bufferSize);
    }

    utf8 * Format(utf8 * buffer, size_t bufferSize, const utf8 * format, ...)
    {
        va_list args;

        va_start(args, format);
        vsnprintf(buffer, bufferSize, format, args);
        va_end(args);

        // Terminate buffer in case formatted string overflowed
        buffer[bufferSize - 1] = '\0';

        return buffer;
    }

    utf8 * Format(const utf8 * format, ...)
    {
        va_list args;
        va_start(args, format);
        utf8 * result = Format_VA(format, args);
        va_end(args);
        return result;
    }

    utf8 * Format_VA(const utf8 * format, va_list args)
    {
        va_list args1, args2;
        va_copy(args1, args);
        va_copy(args2, args);

        // Try to format to a initial buffer, enlarge if not big enough
        size_t bufferSize = 4096;
        utf8 * buffer = Memory::Allocate<utf8>(bufferSize);

        // Start with initial buffer
        sint32 len = vsnprintf(buffer, bufferSize, format, args);
        if (len < 0)
        {
            Memory::Free(buffer);
            va_end(args1);
            va_end(args2);

            // An error occurred...
            return nullptr;
        }

        size_t requiredSize = (size_t)len + 1;
        if (requiredSize > bufferSize)
        {
            // Try again with bigger buffer
            buffer = Memory::Reallocate<utf8>(buffer, bufferSize);
            len = vsnprintf(buffer, bufferSize, format, args);
            if (len < 0)
            {
                Memory::Free(buffer);
                va_end(args1);
                va_end(args2);

                // An error occurred...
                return nullptr;
            }
        }
        else
        {
            // Reduce buffer size to only what was required
            bufferSize = requiredSize;
            buffer = Memory::Reallocate<utf8>(buffer, bufferSize);
        }

        // Ensure buffer is terminated
        buffer[bufferSize - 1] = '\0';

        va_end(args1);
        va_end(args2);
        return buffer;
    }

    utf8 * AppendFormat(utf8 * buffer, size_t bufferSize, const utf8 * format, ...)
    {
        utf8 * dst = buffer;
        size_t i;
        for (i = 0; i < bufferSize; i++)
        {
            if (*dst == '\0') break;
            dst++;
        }

        if (i < bufferSize - 1)
        {
            va_list args;
            va_start(args, format);
            vsnprintf(dst, bufferSize - i - 1, format, args);
            va_end(args);

            // Terminate buffer in case formatted string overflowed
            buffer[bufferSize - 1] = '\0';
        }

        return buffer;
    }

    utf8 * Duplicate(const std::string &src)
    {
        return String::Duplicate(src.c_str());
    }

    utf8 * Duplicate(const utf8 * src)
    {
        utf8 * result = nullptr;
        if (src != nullptr)
        {
            size_t srcSize = SizeOf(src) + 1;
            result = Memory::Allocate<utf8>(srcSize);
            memcpy(result, src, srcSize);
        }
        return result;
    }

    utf8 * DiscardUse(utf8 * * ptr, utf8 * replacement)
    {
        Memory::Free(*ptr);
        *ptr = replacement;
        return replacement;
    }

    utf8 * DiscardDuplicate(utf8 * * ptr, const utf8 * replacement)
    {
        return DiscardUse(ptr, String::Duplicate(replacement));
    }

    std::vector<std::string> Split(const std::string &s, const std::string &delimiter)
    {
        if (delimiter.empty())
        {
            throw std::invalid_argument(nameof(delimiter) " can not be empty.");
        }

        std::vector<std::string> results;
        if (!s.empty())
        {
            size_t index = 0;
            size_t nextIndex;
            do
            {
                nextIndex = s.find(delimiter, index);
                std::string value;
                if (nextIndex == std::string::npos)
                {
                    value = s.substr(index);
                }
                else
                {
                    value = s.substr(index, nextIndex - index);
                }
                results.push_back(value);
                index = nextIndex + delimiter.size();
            }
            while (nextIndex != SIZE_MAX);
        }
        return results;
    }

    utf8 * SkipBOM(utf8 * buffer)
    {
        return (utf8*)SkipBOM((const utf8 *)buffer);
    }

    const utf8 * SkipBOM(const utf8 * buffer)
    {
        if ((uint8)buffer[0] == 0xEF && (uint8)buffer[1] == 0xBB && (uint8)buffer[2] == 0xBF)
        {
            return buffer + 3;
        }
        return buffer;
    }

    size_t GetCodepointLength(codepoint_t codepoint)
    {
        return utf8_get_codepoint_length(codepoint);
    }

    codepoint_t GetNextCodepoint(utf8 * ptr, utf8 * * nextPtr)
    {
        return GetNextCodepoint((const utf8 *)ptr, (const utf8 * *)nextPtr);
    }

    codepoint_t GetNextCodepoint(const utf8 * ptr, const utf8 * * nextPtr)
    {
        return utf8_get_next(ptr, nextPtr);
    }

    utf8 * WriteCodepoint(utf8 * dst, codepoint_t codepoint)
    {
        return utf8_write_codepoint(dst, codepoint);
    }

    bool IsWhiteSpace(codepoint_t codepoint)
    {
        // 0x3000 is the 'ideographic space', a 'fullwidth' character used in CJK languages.
        return iswspace((wchar_t)codepoint) || codepoint == 0x3000;
    }

    utf8 * Trim(utf8 * str)
    {
        utf8 * firstNonWhitespace = nullptr;

        codepoint_t codepoint;
        utf8 * ch = str;
        utf8 * nextCh;
        while ((codepoint = GetNextCodepoint(ch, &nextCh)) != '\0')
        {
            if (codepoint <= WCHAR_MAX && !IsWhiteSpace(codepoint))
            {
                if (firstNonWhitespace == nullptr)
                {
                    firstNonWhitespace = ch;
                }
            }
            ch = nextCh;
        }

        if (firstNonWhitespace != nullptr &&
            firstNonWhitespace != str)
        {
            // Take multibyte characters into account: use the last byte of the
            // current character.
            size_t newStringSize = (nextCh - 1) - firstNonWhitespace;

#ifdef DEBUG
            size_t currentStringSize = String::SizeOf(str);
            Guard::Assert(newStringSize < currentStringSize, GUARD_LINE);
#endif

            std::memmove(str, firstNonWhitespace, newStringSize);
            str[newStringSize] = '\0';
        }
        else
        {
            *ch = '\0';
        }

        return str;
    }

    const utf8 * TrimStart(const utf8 * str)
    {
        codepoint_t codepoint;
        const utf8 * ch = str;
        const utf8 * nextCh;
        while ((codepoint = GetNextCodepoint(ch, &nextCh)) != '\0')
        {
            if (codepoint <= WCHAR_MAX && !IsWhiteSpace(codepoint))
            {
                return ch;
            }
            ch = nextCh;
        }
        // String is all whitespace
        return ch;
    }

    utf8 * TrimStart(utf8 * buffer, size_t bufferSize, const utf8 * src)
    {
        return String::Set(buffer, bufferSize, TrimStart(src));
    }

    std::string TrimStart(const std::string &s)
    {
        const utf8 * trimmed = TrimStart(s.c_str());
        return std::string(trimmed);
    }

    std::string Trim(const std::string &s)
    {
        codepoint_t codepoint;
        const utf8 * ch = s.c_str();
        const utf8 * nextCh;
        const utf8 * startSubstr = nullptr;
        const utf8 * endSubstr = nullptr;
        while ((codepoint = GetNextCodepoint(ch, &nextCh)) != '\0')
        {
            bool isWhiteSpace = codepoint <= WCHAR_MAX && IsWhiteSpace(codepoint);
            if (!isWhiteSpace)
            {
                if (startSubstr == nullptr)
                {
                    startSubstr = ch;
                }

                // Take multibyte characters into account: move pointer towards
                // the last byte of the current character.
                endSubstr = nextCh - 1;
            }
            ch = nextCh;
        }

        if (startSubstr == nullptr)
        {
            // String is all whitespace
            return std::string();
        }

        size_t stringLength = endSubstr - startSubstr + 1;
        return std::string(startSubstr, stringLength);
    }

    std::string Convert(const std::string_view& src, sint32 srcCodePage, sint32 dstCodePage)
    {
#ifdef _WIN32
        // Convert from source code page to UTF-16
        std::wstring u16;
        {
            int srcLen = (int)src.size();
            int sizeReq = MultiByteToWideChar(srcCodePage, 0, src.data(), srcLen, nullptr, 0);
            u16 = std::wstring(sizeReq, 0);
            MultiByteToWideChar(srcCodePage, 0, src.data(), srcLen, u16.data(), sizeReq);
        }

        // Convert from UTF-16 to destination code page
        std::string dst;
        {
            int srcLen = (int)u16.size();
            int sizeReq = WideCharToMultiByte(dstCodePage, 0, u16.data(), srcLen, nullptr, 0, nullptr, nullptr);
            dst = std::string(sizeReq, 0);
            WideCharToMultiByte(dstCodePage, 0, u16.data(), srcLen, dst.data(), sizeReq, nullptr, nullptr);
        }

        return dst;
#else
        STUB();
        return std::string(src);
#endif
    }
}
