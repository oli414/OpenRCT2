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

#include "../common.h"
#include "Map.h"

colour_t wall_get_primary_colour(const rct_tile_element * tileElement);
colour_t wall_get_secondary_colour(const rct_tile_element * wallElement);
colour_t wall_get_tertiary_colour(const rct_tile_element * tileElement);
void wall_set_primary_colour(rct_tile_element * tileElement, colour_t colour);
void wall_set_secondary_colour(rct_tile_element * wallElement, colour_t secondaryColour);
void wall_set_tertiary_colour(rct_tile_element * tileElement, colour_t colour);

