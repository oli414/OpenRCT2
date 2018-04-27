#pragma region Copyright(c) 2014 - 2017 OpenRCT2 Developers
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

#ifndef OPENRCT2_FONTS_H
#define OPENRCT2_FONTS_H

#include "../drawing/Font.h"

#ifndef NO_TTF
extern TTFFontSetDescriptor TTFFontMSGothic;
extern TTFFontSetDescriptor TTFFontHiragano;
extern TTFFontSetDescriptor TTFFontMingLiu;
extern TTFFontSetDescriptor TTFFontHeiti;
extern TTFFontSetDescriptor TTFFontSimSun;
extern TTFFontSetDescriptor TTFFontLiHeiPro;
extern TTFFontSetDescriptor TTFFontGulim;
extern TTFFontSetDescriptor TTFFontNanum;
extern TTFFontSetDescriptor TTFFontArial;
extern TTFFontSetDescriptor TTFFontArialUnicode;
#define FONT(x) x
#else
#define FONT(x) FONT_OPENRCT2_SPRITE
#endif // NO_TTF

void TryLoadFonts();

#endif // OPENRCT2_FONTS_H
