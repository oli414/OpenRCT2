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

#include <cstring>

#include "../../common.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../Track.h"
#include "../TrackPaint.h"
#include "../VehiclePaint.h"

enum
{
    SPR_ROTO_DROP_TOWER_SEGMENT             = 14558,
    SPR_ROTO_DROP_TOWER_SEGMENT_TOP         = 14559,
    SPR_ROTO_DROP_TOWER_BASE                = 14560,
    SPR_ROTO_DROP_TOWER_BASE_SEGMENT        = 14561,
    SPR_ROTO_DROP_TOWER_BASE_90_DEG         = 14562,
    SPR_ROTO_DROP_TOWER_BASE_SEGMENT_90_DEG = 14563,
};

/**
 *
 *  rct2: 0x006D5DA9
 */
void vehicle_visual_roto_drop(paint_session * session, sint32 x, sint32 imageDirection, sint32 y, sint32 z,
                              rct_vehicle * vehicle, const rct_ride_entry_vehicle * vehicleEntry)
{
    sint32 image_id;
    sint32 baseImage_id = (vehicleEntry->base_image_id + 4) + ((vehicle->animation_frame / 4) & 0x3);
    if (vehicle->restraints_position >= 64)
    {
        baseImage_id += 7;
        baseImage_id += (vehicle->restraints_position / 64);
    }

    // Draw back:
    image_id = baseImage_id | SPRITE_ID_PALETTE_COLOUR_2(vehicle->colours.body_colour, vehicle->colours.trim_colour);
    sub_98197C(session, image_id, 0, 0, 2, 2, 41, z, -11, -11, z + 1);

    // Draw front:
    image_id = (baseImage_id + 4) | SPRITE_ID_PALETTE_COLOUR_2(vehicle->colours.body_colour, vehicle->colours.trim_colour);
    sub_98197C(session, image_id, 0, 0, 16, 16, 41, z, -5, -5, z + 1);

    uint8 riding_peep_sprites[64];
    memset(riding_peep_sprites, 0xFF, sizeof(riding_peep_sprites));
    for (sint32 i = 0; i < vehicle->num_peeps; i++)
    {
        uint8 cl = (i & 3) * 16;
        cl += (i & 0xFC);
        cl += vehicle->animation_frame / 4;
        cl += (imageDirection / 8) * 16;
        cl &= 0x3F;
        riding_peep_sprites[cl] = vehicle->peep_tshirt_colours[i];
    }

    // Draw riding peep sprites in back to front order:
    for (sint32 j = 0; j <= 48; j++)
    {
        sint32 i = (j % 2) ? (48 - (j / 2)) : (j / 2);
        if (riding_peep_sprites[i] != 0xFF)
        {
            baseImage_id = vehicleEntry->base_image_id + 20 + i;
            if (vehicle->restraints_position >= 64)
            {
                baseImage_id += 64;
                baseImage_id += vehicle->restraints_position / 64;
            }
            image_id = baseImage_id | SPRITE_ID_PALETTE_COLOUR_1(riding_peep_sprites[i]);
            sub_98199C(session, image_id, 0, 0, 16, 16, 41, z, -5, -5, z + 1);
        }
    };

    assert(vehicleEntry->effect_visual == 1);
    // Although called in original code, effect_visual (splash effects) are not used for many rides and does not make sense so
    // it was taken out
}

/** rct2: 0x00886194 */
static void paint_roto_drop_base(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    trackSequence = track_map_3x3[direction][trackSequence];

    sint32   edges    = edges_3x3[trackSequence];
    Ride *   ride     = get_ride(rideIndex);
    LocationXY16 position = session->MapPosition;

    wooden_a_supports_paint_setup(session, (direction & 1), 0, height, session->TrackColours[SCHEME_MISC], nullptr);

    uint32 imageId = SPR_FLOOR_METAL_B | session->TrackColours[SCHEME_SUPPORTS];
    sub_98197C(session, imageId, 0, 0, 32, 32, 1, height, 0, 0, height);

    track_paint_util_paint_fences(
        session, edges, position, tileElement, ride, session->TrackColours[SCHEME_TRACK], height, fenceSpritesMetalB,
        session->CurrentRotation);

    if (trackSequence == 0)
    {
        imageId =
            (direction & 1 ? SPR_ROTO_DROP_TOWER_BASE_90_DEG : SPR_ROTO_DROP_TOWER_BASE) | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 2, 2, 27, height, 8, 8, height + 3);

        imageId = (direction & 1 ? SPR_ROTO_DROP_TOWER_BASE_SEGMENT_90_DEG : SPR_ROTO_DROP_TOWER_BASE_SEGMENT) |
                  session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 2, 2, 30, height + 32, 8, 8, height + 32);

        imageId = (direction & 1 ? SPR_ROTO_DROP_TOWER_BASE_SEGMENT_90_DEG : SPR_ROTO_DROP_TOWER_BASE_SEGMENT) |
                  session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 2, 2, 30, height + 64, 8, 8, height + 64);

        paint_util_set_vertical_tunnel(session, height + 96);
        paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);

#ifdef __TESTPAINT__
        paint_util_set_general_support_height(session, height + 32, 0x20);
#else
        paint_util_set_general_support_height(session, height + 96, 0x20);
#endif

        return;
    }

    sint32 blockedSegments = 0;
    switch (trackSequence)
    {
    case 1:
        blockedSegments = SEGMENT_B8 | SEGMENT_C8 | SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC;
        break;
    case 2:
        blockedSegments = SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC;
        break;
    case 3:
        blockedSegments = SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC | SEGMENT_D4 | SEGMENT_C0;
        break;
    case 4:
        blockedSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_B8;
        break;
    case 5:
        blockedSegments = SEGMENT_BC | SEGMENT_D4 | SEGMENT_C0;
        break;
    case 6:
        blockedSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0;
        break;
    case 7:
        blockedSegments = SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0 | SEGMENT_D4 | SEGMENT_BC;
        break;
    case 8:
        blockedSegments = SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0;
        break;
    }
    paint_util_set_segment_support_height(session, blockedSegments, 0xFFFF, 0);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL & ~blockedSegments, height + 2, 0x20);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x008861A4 */
static void paint_roto_drop_tower_section(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    sint32                   height,
    const rct_tile_element * tileElement)
{
    if (trackSequence == 1)
    {
        return;
    }

    uint32 imageId = SPR_ROTO_DROP_TOWER_SEGMENT | session->TrackColours[SCHEME_TRACK];
    sub_98197C(session, imageId, 0, 0, 2, 2, 30, height, 8, 8, height);

    const rct_tile_element * nextTileElement = tileElement + 1;
    if (tile_element_is_last_for_tile(tileElement) || tileElement->clearance_height != nextTileElement->base_height)
    {
        imageId = SPR_ROTO_DROP_TOWER_SEGMENT_TOP | session->TrackColours[SCHEME_TRACK];
        sub_98199C(session, imageId, 0, 0, 2, 2, 30, height, 8, 8, height);
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);

    paint_util_set_vertical_tunnel(session, height + 32);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/**
 * rct2: 0x00886074
 */
TRACK_PAINT_FUNCTION get_track_paint_function_roto_drop(sint32 trackType, sint32 direction)
{
    switch (trackType)
    {
    case TRACK_ELEM_TOWER_BASE:
        return paint_roto_drop_base;

    case TRACK_ELEM_TOWER_SECTION:
        return paint_roto_drop_tower_section;
    }

    return nullptr;
}
