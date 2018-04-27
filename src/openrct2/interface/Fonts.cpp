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

#include "../config/Config.h"
#include "../core/String.hpp"
#include "Fonts.h"
#include "FontFamilies.h"

#include "../drawing/TTF.h"
#include "../localisation/Language.h"

#ifndef NO_TTF
uint8 const HINTING_DISABLED         =  0;
uint8 const HINTING_THRESHOLD_LOW    = 40;
uint8 const HINTING_THRESHOLD_MEDIUM = 60;

// clang-format off
TTFFontSetDescriptor TTFFontMSGothic = { {
    { "msgothic.ttc", "MS PGothic",  9, 1,  1,  9, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "msgothic.ttc", "MS PGothic", 12, 1,  0, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "msgothic.ttc", "MS PGothic", 12, 1,  0, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "msgothic.ttc", "MS PGothic", 13, 1,  0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontHiragano = { {
    { u8"ヒラギノ丸ゴ ProN W4.ttc", "Hiragino Maru Gothic ProN",  9, 1,  1,  9, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"ヒラギノ丸ゴ ProN W4.ttc", "Hiragino Maru Gothic ProN", 11, 1,  0, 13, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"ヒラギノ丸ゴ ProN W4.ttc", "Hiragino Maru Gothic ProN", 11, 1,  0, 13, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"ヒラギノ丸ゴ ProN W4.ttc", "Hiragino Maru Gothic ProN", 12, 1,  0, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontMingLiu = { {
    {    "msjh.ttc", "JhengHei",  9, -1, -1,  9, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "mingliu.ttc",  "MingLiU", 11,  1,  1, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "mingliu.ttc",  "MingLiU", 12,  1,  0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "mingliu.ttc",  "MingLiU", 13,  1,  0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontHeiti = { {
    { u8"华文黑体.ttf", "STHeiti",  9, -1, -1,  9, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"华文黑体.ttf", "STHeiti", 11,  1,  1, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"华文黑体.ttf", "STHeiti", 12,  1,  0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"华文黑体.ttf", "STHeiti", 13,  1,  0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontSimSun = { {
    {   "msyh.ttc",  "YaHei",  9, -1, -1,  9, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "simsun.ttc", "SimSun", 11,  1, -1, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "simsun.ttc", "SimSun", 12,  1, -2, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "simsun.ttc", "SimSun", 13,  1,  0, 16, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontLiHeiPro = { {
    { u8"儷黑 Pro.ttf", "LiHei Pro",  9, 1, -1,  9, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"儷黑 Pro.ttf", "LiHei Pro", 11, 1,  0, 14, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"儷黑 Pro.ttf", "LiHei Pro", 12, 1,  0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
    { u8"儷黑 Pro.ttf", "LiHei Pro", 13, 1,  0, 16, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontGulim = { {
    { "gulim.ttc", "Gulim", 10, 1, 0, 10, HINTING_DISABLED,         nullptr },
    { "gulim.ttc", "Gulim", 12, 1, 0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "gulim.ttc", "Gulim", 12, 1, 0, 15, HINTING_THRESHOLD_MEDIUM, nullptr },
    { "gulim.ttc", "Gulim", 13, 1, 0, 16, HINTING_THRESHOLD_MEDIUM, nullptr },
} };

TTFFontSetDescriptor TTFFontNanum = { {
    { "NanumGothic.ttc", "Nanum Gothic", 10, 1, 0, 10, HINTING_DISABLED,      nullptr },
    { "NanumGothic.ttc", "Nanum Gothic", 12, 1, 0, 15, HINTING_THRESHOLD_LOW, nullptr },
    { "NanumGothic.ttc", "Nanum Gothic", 12, 1, 0, 15, HINTING_THRESHOLD_LOW, nullptr },
    { "NanumGothic.ttc", "Nanum Gothic", 13, 1, 0, 16, HINTING_THRESHOLD_LOW, nullptr },
} };

TTFFontSetDescriptor TTFFontArial = { {
    { "arial.ttf", "Arial",  9, 0, -1,  9, HINTING_THRESHOLD_LOW, nullptr },
    { "arial.ttf", "Arial", 10, 0, -1, 12, HINTING_THRESHOLD_LOW, nullptr },
    { "arial.ttf", "Arial", 11, 0, -1, 12, HINTING_THRESHOLD_LOW, nullptr },
    { "arial.ttf", "Arial", 12, 0, -1, 14, HINTING_THRESHOLD_LOW, nullptr },
} };

TTFFontSetDescriptor TTFFontArialUnicode = { {
    { "arialuni.ttf", "Arial Unicode MS",  9, 0, -1,  9, HINTING_THRESHOLD_LOW, nullptr },
    { "arialuni.ttf", "Arial Unicode MS", 10, 0, -1, 12, HINTING_THRESHOLD_LOW, nullptr },
    { "arialuni.ttf", "Arial Unicode MS", 11, 0, -1, 12, HINTING_THRESHOLD_LOW, nullptr },
    { "arialuni.ttf", "Arial Unicode MS", 12, 0, -1, 14, HINTING_THRESHOLD_LOW, nullptr },
} };
// clang-format on
#endif // NO_TTF

static void LoadSpriteFont()
{
    ttf_dispose();
    gUseTrueTypeFont   = false;
#ifndef NO_TTF
    gCurrentTTFFontSet = nullptr;
#endif // NO_TTF
}

#ifndef NO_TTF
static bool LoadFont(TTFFontSetDescriptor * font)
{
    gUseTrueTypeFont   = true;
    gCurrentTTFFontSet = font;

    ttf_dispose();
    bool fontInitialised = ttf_initialise();
    return fontInitialised;
}

static bool LoadCustomConfigFont()
{
    static TTFFontSetDescriptor TTFFontCustom = { {
        { gConfigFonts.file_name, gConfigFonts.font_name, gConfigFonts.size_tiny, gConfigFonts.x_offset, gConfigFonts.y_offset,
          gConfigFonts.height_tiny, gConfigFonts.hinting_threshold, nullptr },
        { gConfigFonts.file_name, gConfigFonts.font_name, gConfigFonts.size_small, gConfigFonts.x_offset, gConfigFonts.y_offset,
          gConfigFonts.height_small, gConfigFonts.hinting_threshold, nullptr },
        { gConfigFonts.file_name, gConfigFonts.font_name, gConfigFonts.size_medium, gConfigFonts.x_offset,
          gConfigFonts.y_offset, gConfigFonts.height_medium, gConfigFonts.hinting_threshold, nullptr },
        { gConfigFonts.file_name, gConfigFonts.font_name, gConfigFonts.size_big, gConfigFonts.x_offset, gConfigFonts.y_offset,
          gConfigFonts.height_big, gConfigFonts.hinting_threshold, nullptr },
    } };

    ttf_dispose();
    gUseTrueTypeFont   = true;
    gCurrentTTFFontSet = &TTFFontCustom;

    bool fontInitialised = ttf_initialise();
    return fontInitialised;
}
#endif // NO_TTF

void TryLoadFonts()
{
#ifndef NO_TTF
    TTFontFamily const * fontFamily = LanguagesDescriptors[gCurrentLanguage].font_family;

    if (fontFamily != FAMILY_OPENRCT2_SPRITE)
    {
        if (!String::IsNullOrEmpty(gConfigFonts.file_name))
        {
            if (LoadCustomConfigFont())
            {
                return;
            }
            log_verbose("Unable to initialise configured TrueType font -- falling back to the language's default.");
        }

        for (auto &font : *fontFamily)
        {
            if (LoadFont(font))
            {
                return;
            }

            TTFFontDescriptor smallFont = font->size[FONT_SIZE_SMALL];
            log_verbose("Unable to load TrueType font '%s' -- trying the next font in the family.", smallFont.font_name);
        }

        if (fontFamily != &TTFFamilySansSerif)
        {
            log_verbose("Unable to initialise any of the preferred TrueType fonts -- falling back to sans serif fonts.");

            for (auto &font : TTFFamilySansSerif)
            {
                if (LoadFont(font))
                {
                    return;
                }

                TTFFontDescriptor smallFont = font->size[FONT_SIZE_SMALL];
                log_verbose("Unable to load TrueType font '%s' -- trying the next font in the family.", smallFont.font_name);
            }

            log_verbose("Unable to initialise any of the preferred TrueType fonts -- falling back to sprite font.");
        }
    }
#endif // NO_TTF
    LoadSpriteFont();
}
