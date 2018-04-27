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

#include "OpenRCT2.h"

sint32 gExitCode;
sint32 gOpenRCT2StartupAction = STARTUP_ACTION_TITLE;
utf8 gOpenRCT2StartupActionPath[512] = { 0 };
utf8 gExePath[MAX_PATH];
utf8 gCustomUserDataPath[MAX_PATH] = { 0 };
utf8 gCustomOpenrctDataPath[MAX_PATH] = { 0 };
utf8 gCustomRCT2DataPath[MAX_PATH] = { 0 };
utf8 gCustomPassword[MAX_PATH] = { 0 };

bool gOpenRCT2Headless = false;
bool gOpenRCT2NoGraphics = false;

bool gOpenRCT2ShowChangelog;
bool gOpenRCT2SilentBreakpad;

#ifndef DISABLE_NETWORK
// OpenSSL's message digest context used for calculating sprite checksums
EVP_MD_CTX * gHashCTX = nullptr;
#endif // DISABLE_NETWORK

uint32 gCurrentDrawCount = 0;
uint8 gScreenFlags;
uint32 gScreenAge;
uint8 gSavePromptMode;
