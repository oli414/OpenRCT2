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

#pragma once

#include "common.h"

#include "object/Object.h"

enum PARK_LOAD_ERROR
{
    PARK_LOAD_ERROR_OK,
    PARK_LOAD_ERROR_MISSING_OBJECTS,
    PARK_LOAD_ERROR_INVALID_EXTENSION,
    PARK_LOAD_ERROR_UNSUPPORTED_RCTC_FLAG,
    PARK_LOAD_ERROR_UNKNOWN = 255
};

#include <string>
#include <vector>
#include "core/String.hpp"

interface IObjectManager;
interface IObjectRepository;
interface IStream;
struct scenario_index_entry;

struct ParkLoadResult final
{
public:
    const PARK_LOAD_ERROR               Error;
    const std::vector<rct_object_entry> MissingObjects;
    const uint8                         Flag;

    static ParkLoadResult CreateOK();
    static ParkLoadResult CreateInvalidExtension();
    static ParkLoadResult CreateMissingObjects(const std::vector<rct_object_entry> &missingObjects);
    static ParkLoadResult CreateUnknown();
    static ParkLoadResult CreateUnsupportedRCTCflag(uint8 classic_flag);

private:
    explicit ParkLoadResult(PARK_LOAD_ERROR error);
    ParkLoadResult(PARK_LOAD_ERROR error, const std::vector<rct_object_entry> &missingObjects);
    ParkLoadResult(PARK_LOAD_ERROR error, const uint8 flag);
};

/**
 * Interface to import scenarios and saved games.
 */
interface IParkImporter
{
public:
    virtual ~IParkImporter() = default;

    virtual ParkLoadResult  Load(const utf8 * path) abstract;
    virtual ParkLoadResult  LoadSavedGame(const utf8 * path, bool skipObjectCheck = false) abstract;
    virtual ParkLoadResult  LoadScenario(const utf8 * path, bool skipObjectCheck = false) abstract;
    virtual ParkLoadResult  LoadFromStream(IStream * stream,
                                           bool isScenario,
                                           bool skipObjectCheck = false,
                                           const utf8 * path = String::Empty) abstract;

    virtual void Import() abstract;
    virtual bool GetDetails(scenario_index_entry * dst) abstract;
};

namespace ParkImporter
{
    IParkImporter * Create(const std::string &hintPath);
    IParkImporter * CreateS4();
    IParkImporter * CreateS6(IObjectRepository * objectRepository, IObjectManager * objectManager);

    bool ExtensionIsRCT1(const std::string &extension);
    bool ExtensionIsScenario(const std::string &extension);
}

void park_importer_load_from_stream(void * stream, const utf8 * hintPath);
bool park_importer_extension_is_scenario(const utf8 * extension);

PARK_LOAD_ERROR             ParkLoadResult_GetError(const ParkLoadResult * t);
size_t                      ParkLoadResult_GetMissingObjectsCount(const ParkLoadResult * t);
const rct_object_entry *    ParkLoadResult_GetMissingObjects(const ParkLoadResult * t);
uint8                       ParkLoadResult_GetFlag(const ParkLoadResult * t);
void                        ParkLoadResult_Delete(ParkLoadResult * t);
ParkLoadResult *            ParkLoadResult_CreateInvalidExtension();
