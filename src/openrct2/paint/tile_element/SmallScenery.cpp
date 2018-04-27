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

#include "TileElement.h"
#include "../../config/Config.h"
#include "../../Game.h"
#include "../../interface/Viewport.h"
#include "../../localisation/Date.h"
#include "../Paint.h"
#include "../../ride/TrackDesign.h"
#include "../Supports.h"
#include "../../world/Map.h"
#include "../../world/Scenery.h"
#include "../../world/SmallScenery.h"

static constexpr const LocationXY16 lengths[] = {
    { 12, 26 },
    { 26, 12 },
    { 12, 26 },
    { 26, 12 }
};

/**
 *
 *  rct2: 0x006DFF47
 */
void scenery_paint(paint_session * session, uint8 direction, sint32 height, const rct_tile_element * tileElement)
{
    if (gCurrentViewportFlags & VIEWPORT_FLAG_HIGHLIGHT_PATH_ISSUES)
    {
        return;
    }
    //RCT2_CALLPROC_X(0x6DFF47, 0, 0, direction, height, (sint32)tileElement, 0, 0); return;
    session->InteractionType = VIEWPORT_INTERACTION_ITEM_SCENERY;
    LocationXYZ16 boxlength;
    LocationXYZ16 boxoffset;
    boxoffset.x = 0;
    boxoffset.y = 0;
    boxoffset.z = height;
    sint32 baseImageid = 0;
    const sint32 rotation = session->CurrentRotation;
    if (gTrackDesignSaveMode) {
        if (!track_design_save_contains_tile_element(tileElement)) {
            baseImageid = SPRITE_ID_PALETTE_COLOUR_1(PALETTE_46);
        }
    }
    if (tileElement->flags & TILE_ELEMENT_FLAG_GHOST) {
        session->InteractionType = VIEWPORT_INTERACTION_ITEM_NONE;
        baseImageid = CONSTRUCTION_MARKER;
    }
    uint32 dword_F64EB0 = baseImageid;

    rct_scenery_entry *entry = get_small_scenery_entry(tileElement->properties.scenery.type);

    if (entry == nullptr)
    {
        return;
    }

    baseImageid = entry->image + direction;
    boxlength.x = 2;
    boxlength.y = 2;
    sint8 x_offset = 0;
    sint8 y_offset = 0;
    if (scenery_small_entry_has_flag(entry, SMALL_SCENERY_FLAG_FULL_TILE))
    {
        if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_HALF_SPACE)) {
            // 6DFFE3:
            static constexpr const LocationXY16 scenery_half_tile_offsets[] = {
                {  3,  3 },
                {  3, 17 },
                { 17,  3 },
                {  3,  3 }
            };
            boxoffset.x = scenery_half_tile_offsets[direction].x;
            boxoffset.y = scenery_half_tile_offsets[direction].y;
            boxlength.x = lengths[direction].x;
            boxlength.y = lengths[direction].y;
            x_offset = 3;
            y_offset = 3;
        } else {
            x_offset = 15;
            y_offset = 15;
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VOFFSET_CENTRE)) {
                x_offset = 3;
                y_offset = 3;
                boxlength.x = 26;
                boxlength.y = 26;
                if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_NO_WALLS)) {
                    x_offset = 1;
                    y_offset = 1;
                    boxlength.x = 30;
                    boxlength.y = 30;
                }
            }
            boxoffset.x = x_offset;
            boxoffset.y = y_offset;
        }
    } else {
        // 6DFFC2:
        uint8 ecx = (tile_element_get_scenery_quadrant(tileElement) + rotation) & 3;
        x_offset = ScenerySubTileOffsets[ecx].x;
        y_offset = ScenerySubTileOffsets[ecx].y;
        boxoffset.x = x_offset;
        boxoffset.y = y_offset;
    }
    // 6E007F:
    boxlength.z = entry->small_scenery.height - 4;
    if (boxlength.z > 128 || boxlength.z < 0) {
        boxlength.z = 128;
    }
    if (scenery_small_entry_has_flag(entry, SMALL_SCENERY_FLAG_CAN_WITHER))
    {
        if (tileElement->properties.scenery.age >= SCENERY_WITHER_AGE_THRESHOLD_1) {
            baseImageid += 4;
        }
        if (tileElement->properties.scenery.age >= SCENERY_WITHER_AGE_THRESHOLD_2) {
            baseImageid += 4;
        }
    }
    if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_HAS_PRIMARY_COLOUR))
    {
        if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_HAS_SECONDARY_COLOUR))
        {
            baseImageid |= SPRITE_ID_PALETTE_COLOUR_2(scenery_small_get_primary_colour(tileElement),
                                                      scenery_small_get_secondary_colour(tileElement));
        }
        else
        {
            baseImageid |= SPRITE_ID_PALETTE_COLOUR_1(scenery_small_get_primary_colour(tileElement));
        }
    }
    if (dword_F64EB0 != 0) {
        baseImageid = (baseImageid & 0x7FFFF) | dword_F64EB0;
    }
    if (!(scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VISIBLE_WHEN_ZOOMED))) {
        sub_98197C(
            session, baseImageid, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
            boxoffset.y, boxoffset.z);
    }

    if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_HAS_GLASS)) {
        if (dword_F64EB0 == 0) {
            // Draw translucent overlay:
            // TODO: Name palette entries
            sint32 image_id = (baseImageid & 0x7FFFF) + (GlassPaletteIds[scenery_small_get_primary_colour(tileElement)] << 19) + 0x40000004;
            sub_98199C(
                session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                boxoffset.y, boxoffset.z);
        }
    }

    if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_ANIMATED)) {
        rct_drawpixelinfo* dpi = session->Unk140E9A8;
        if ((scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VISIBLE_WHEN_ZOOMED)) || (dpi->zoom_level <= 1)) {
            // 6E01A9:
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_FOUNTAIN_SPRAY_1)) {
                // 6E0512:
                sint32 image_id = ((gCurrentTicks / 2) & 0xF) + entry->image + 4;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);
            } else
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_FOUNTAIN_SPRAY_4)) {
                // 6E043B:
                sint32 image_id = ((gCurrentTicks / 2) & 0xF) + entry->image + 8;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);

                image_id = direction + entry->image + 4;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);

                image_id = ((gCurrentTicks / 2) & 0xF) + entry->image + 24;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);
            } else
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_IS_CLOCK)) {
                // 6E035C:
                sint32 minuteImageOffset = ((gRealTimeOfDay.minute + 6) * 17) / 256;
                sint32 timeImageBase = gRealTimeOfDay.hour;
                while (timeImageBase >= 12) {
                    timeImageBase -= 12;
                }
                timeImageBase = (timeImageBase * 4) + minuteImageOffset;
                if (timeImageBase >= 48) {
                    timeImageBase -= 48;
                }
                sint32 image_id = timeImageBase + (direction * 12);
                if (image_id >= 48) {
                    image_id -= 48;
                }

                image_id = image_id + entry->image + 68;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);

                image_id = gRealTimeOfDay.minute + (direction * 15);
                if (image_id >= 60) {
                    image_id -= 60;
                }
                image_id = image_id + entry->image + 8;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);
            } else
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_SWAMP_GOO)) {
                // 6E02F6:
                sint32 image_id = gCurrentTicks;
                image_id += session->SpritePosition.x / 4;
                image_id += session->SpritePosition.y / 4;
                image_id = (image_id / 4) & 15;
                image_id += entry->image;
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                sub_98199C(
                    session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                    boxoffset.y, boxoffset.z);
            }
            else if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_HAS_FRAME_OFFSETS))
            {
                sint32 frame = gCurrentTicks;
                if (!(scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_COG))) {
                    // 6E01F8:
                    frame += ((session->SpritePosition.x / 4) + (session->SpritePosition.y / 4));
                    frame += (tileElement->type & 0xC0) / 16;
                }
                // 6E0222:
                uint16 delay = entry->small_scenery.animation_delay & 0xFF;
                frame >>= delay;
                frame &= entry->small_scenery.animation_mask;
                sint32 image_id = 0;
                if (frame < entry->small_scenery.num_frames) {
                    image_id = entry->small_scenery.frame_offsets[frame];
                }
                image_id = (image_id * 4) + direction + entry->image;
                if (scenery_small_entry_has_flag(entry, SMALL_SCENERY_FLAG_VISIBLE_WHEN_ZOOMED | SMALL_SCENERY_FLAG17)) {
                    image_id += 4;
                }
                if (scenery_small_entry_has_flag(entry, SMALL_SCENERY_FLAG_HAS_PRIMARY_COLOUR))
                {
                    if (scenery_small_entry_has_flag(entry, SMALL_SCENERY_FLAG_HAS_SECONDARY_COLOUR)) {
                        image_id |= SPRITE_ID_PALETTE_COLOUR_2(scenery_small_get_primary_colour(tileElement),
                                                               scenery_small_get_secondary_colour(tileElement));
                    }
                    else
                    {
                        image_id |= SPRITE_ID_PALETTE_COLOUR_1(scenery_small_get_primary_colour(tileElement));
                    }
                }
                if (dword_F64EB0 != 0) {
                    image_id = (image_id & 0x7FFFF) | dword_F64EB0;
                }
                if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VISIBLE_WHEN_ZOOMED)) {
                    sub_98197C(
                        session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                        boxoffset.y, boxoffset.z);
                }
                else {
                    sub_98199C(
                        session, image_id, x_offset, y_offset, boxlength.x, boxlength.y, boxlength.z - 1, height, boxoffset.x,
                        boxoffset.y, boxoffset.z);
                }
            }
        }
    }
    // 6E0556: Draw supports:
    if (scenery_small_get_supports_needed(tileElement)) {
        if (!(scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_NO_SUPPORTS))) {
            sint32 ax = 0;
            sint32 supportHeight = height;
            if (supportHeight & 0xF) {
                supportHeight &= 0xFFFFFFF0;
                ax = 49;
            }
            uint32 supportImageColourFlags = IMAGE_TYPE_REMAP;
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_PAINT_SUPPORTS))
            {
                supportImageColourFlags = SPRITE_ID_PALETTE_COLOUR_1(scenery_small_get_primary_colour(tileElement));
            }
            if (dword_F64EB0 != 0) {
                supportImageColourFlags = dword_F64EB0;
            }
            if (direction & 1) {
                wooden_b_supports_paint_setup(session, 1, ax, supportHeight, supportImageColourFlags, nullptr);
            } else {
                wooden_b_supports_paint_setup(session, 0, ax, supportHeight, supportImageColourFlags, nullptr);
            }
        }
    }
    // 6E05D1:
    height += entry->small_scenery.height;

    paint_util_set_general_support_height(session, ceil2(height, 8), 0x20);
    // 6E05FF:
    if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_BUILD_DIRECTLY_ONTOP)) {
        if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_FULL_TILE)) {
            // 6E0825:
            paint_util_set_segment_support_height(session, SEGMENT_C4, height, 0x20);
            if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VOFFSET_CENTRE)) {
                paint_util_set_segment_support_height(session, SEGMENTS_ALL & ~SEGMENT_C4, height, 0x20);
            }
            return;
        }
        if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VOFFSET_CENTRE)) {
            // 6E075C:
            direction = (tile_element_get_scenery_quadrant(tileElement) + rotation) % 4;
            paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC, direction), height, 0x20);
            return;
        }
        return;
    }
    if (scenery_small_entry_has_flag(entry,  (SMALL_SCENERY_FLAG27 | SMALL_SCENERY_FLAG_FULL_TILE))) {
        paint_util_set_segment_support_height(session, SEGMENT_C4, 0xFFFF, 0);
        if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VOFFSET_CENTRE)) {
            paint_util_set_segment_support_height(session, SEGMENTS_ALL & ~SEGMENT_C4, 0xFFFF, 0);
        }
        return;
    }
    if (scenery_small_entry_has_flag(entry,  SMALL_SCENERY_FLAG_VOFFSET_CENTRE)) {
        direction = (tile_element_get_scenery_quadrant(tileElement) + rotation) % 4;
        paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        return;
    }
}
