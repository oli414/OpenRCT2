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

#include "../../common.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../Track.h"
#include "../TrackPaint.h"
#include "../../world/Sprite.h"

enum
{
    SPR_SPACE_RINGS_FENCE_NE = 22146,
    SPR_SPACE_RINGS_FENCE_SE = 22147,
    SPR_SPACE_RINGS_FENCE_SW = 22148,
    SPR_SPACE_RINGS_FENCE_NW = 22149,
};

static constexpr const uint32 space_rings_fence_sprites[] = {
    SPR_SPACE_RINGS_FENCE_NE,
    SPR_SPACE_RINGS_FENCE_SE,
    SPR_SPACE_RINGS_FENCE_SW,
    SPR_SPACE_RINGS_FENCE_NW,
};

/** rct2: 0x00768A3B */
static void paint_space_rings_structure(paint_session * session, Ride * ride, uint8 direction, uint32 segment, sint32 height)
{
    const rct_tile_element * savedTileElement = static_cast<const rct_tile_element *>(session->CurrentlyDrawnItem);

    uint32 vehicleIndex = (segment - direction) & 0x3;

    if (ride->num_stations == 0 || vehicleIndex < ride->num_vehicles)
    {
        rct_ride_entry * rideEntry = get_ride_entry(ride->subtype);
        rct_vehicle *    vehicle   = nullptr;

        sint32 frameNum = direction;

        uint32 baseImageId = rideEntry->vehicles[0].base_image_id;

        if (ride->lifecycle_flags & RIDE_LIFECYCLE_ON_TRACK && ride->vehicles[0] != SPRITE_INDEX_NULL)
        {
            session->InteractionType    = VIEWPORT_INTERACTION_ITEM_SPRITE;
            vehicle                     = GET_VEHICLE(ride->vehicles[vehicleIndex]);
            session->CurrentlyDrawnItem = vehicle;
            frameNum += (sint8)vehicle->vehicle_sprite_type * 4;
        }

        uint32 imageColourFlags = session->TrackColours[SCHEME_MISC];
        if ((ride->colour_scheme_type & 3) != RIDE_COLOUR_SCHEME_DIFFERENT_PER_TRAIN)
        {
            vehicleIndex = 0;
        }

        if (imageColourFlags == IMAGE_TYPE_REMAP)
        {
            imageColourFlags = SPRITE_ID_PALETTE_COLOUR_2(ride->vehicle_colours[vehicleIndex].body_colour,
                                                          ride->vehicle_colours[vehicleIndex].trim_colour);
        }

        uint32 imageId = (baseImageId + frameNum) | imageColourFlags;
        sub_98197C(session, imageId, 0, 0, 20, 20, 23, height, -10, -10, height);

        if (vehicle != nullptr && vehicle->num_peeps > 0)
        {
            rct_peep * rider = GET_PEEP(vehicle->peep[0]);
            imageColourFlags = SPRITE_ID_PALETTE_COLOUR_2(rider->tshirt_colour, rider->trousers_colour);
            imageId          = ((baseImageId & 0x7FFFF) + 352 + frameNum) | imageColourFlags;
            sub_98199C(session, imageId, 0, 0, 20, 20, 23, height, -10, -10, height);
        }
    }

    session->CurrentlyDrawnItem = savedTileElement;
    session->InteractionType    = VIEWPORT_INTERACTION_ITEM_RIDE;
}

/** rct2: 0x00767C40 */
static void paint_space_rings(
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

    uint32 imageId;

    wooden_a_supports_paint_setup(session, (direction & 1), 0, height, session->TrackColours[SCHEME_MISC], nullptr);

    track_paint_util_paint_floor(session, edges, session->TrackColours[SCHEME_TRACK], height, floorSpritesCork);

    switch (trackSequence)
    {
    case 7:
        if (track_paint_util_has_fence(EDGE_SW, position, tileElement, ride, session->CurrentRotation))
        {
            imageId = SPR_SPACE_RINGS_FENCE_SW | session->TrackColours[SCHEME_MISC];
            sub_98197C(session, imageId, 0, 0, 1, 28, 7, height, 29, 0, height + 2);
        }
        if (track_paint_util_has_fence(EDGE_SE, position, tileElement, ride, session->CurrentRotation))
        {
            imageId = SPR_SPACE_RINGS_FENCE_SE | session->TrackColours[SCHEME_MISC];
            sub_98197C(session, imageId, 0, 0, 28, 1, 7, height, 0, 29, height + 2);
        }
        break;
    default:
        track_paint_util_paint_fences(
            session, edges, position, tileElement, ride, session->TrackColours[SCHEME_MISC], height, space_rings_fence_sprites,
            session->CurrentRotation);
        break;
    }

    switch (trackSequence)
    {
    case 0:
        paint_space_rings_structure(session, ride, direction, 0, height + 3);
        break;
    case 5:
        paint_space_rings_structure(session, ride, direction, 1, height + 3);
        break;
    case 7:
        paint_space_rings_structure(session, ride, direction, 2, height + 3);
        break;
    case 8:
        paint_space_rings_structure(session, ride, direction, 3, height + 3);
        break;
    }

    sint32 cornerSegments = 0;
    switch (trackSequence)
    {
    case 0:
        cornerSegments = 0;
        break;
    case 1:
        cornerSegments = SEGMENT_B8 | SEGMENT_C8 | SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC;
        break;
    case 2:
        cornerSegments = SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC;
        break;
    case 3:
        cornerSegments = SEGMENT_B4 | SEGMENT_CC | SEGMENT_BC | SEGMENT_D4 | SEGMENT_C0;
        break;
    case 4:
        cornerSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_B8;
        break;
    case 5:
        cornerSegments = SEGMENT_BC | SEGMENT_D4 | SEGMENT_C0;
        break;
    case 6:
        cornerSegments = SEGMENT_B4 | SEGMENT_C8 | SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0;
        break;
    case 7:
        cornerSegments = SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0 | SEGMENT_D4 | SEGMENT_BC;
        break;
    case 8:
        cornerSegments = SEGMENT_B8 | SEGMENT_D0 | SEGMENT_C0;
        break;
    }
    paint_util_set_segment_support_height(session, cornerSegments, height + 2, 0x20);
    paint_util_set_segment_support_height(session, SEGMENTS_ALL & ~cornerSegments, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 48, 0x20);
}

/**
 * rct2: 0x0x00767A40
 */
TRACK_PAINT_FUNCTION get_track_paint_function_space_rings(sint32 trackType, sint32 direction)
{
    if (trackType != FLAT_TRACK_ELEM_3_X_3)
    {
        return nullptr;
    }

    return paint_space_rings;
}
