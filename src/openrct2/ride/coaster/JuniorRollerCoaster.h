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

#ifndef _JUNIOR_ROLLER_COASTER_H
#define _JUNIOR_ROLLER_COASTER_H

#include "../../common.h"
#include "../../paint/tile_element/TileElement.h"

enum JUNIOR_RC_CHAINTYPE
{
    JUNIOR_RC_CHAIN_NONE,
    JUNIOR_RC_CHAIN_FRICTION_WHEELS,
    JUNIOR_RC_CHAIN_CHAIN_LIFT,
};

void junior_rc_paint_station(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    uint8                    rideType);

void junior_rc_paint_track_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_60_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_flat_to_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_25_deg_up_to_60_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_60_deg_up_to_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_25_deg_up_to_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);

void junior_rc_paint_track_left_quarter_turn_5_tiles_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_right_quarter_turn_5_tiles_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);

void junior_rc_paint_track_right_quarter_turn_3_tiles_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_right_quarter_turn_3_tiles_25_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);

void junior_rc_paint_track_diag_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_60_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_flat_to_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_flat_to_60_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_25_deg_up_to_60_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_60_deg_up_to_25_deg_up(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_25_deg_up_to_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_60_deg_up_to_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_25_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_60_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_flat_to_25_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_flat_to_60_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_25_deg_down_to_60_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_60_deg_down_to_25_deg_down(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_25_deg_down_to_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);
void junior_rc_paint_track_diag_60_deg_down_to_flat(
    paint_session *          session,
    uint8                    rideIndex,
    uint8                    trackSequence,
    uint8                    direction,
    uint16                   height,
    const rct_tile_element * tileElement,
    JUNIOR_RC_CHAINTYPE      chainType);

#endif // _JUNIOR_ROLLER_COASTER_H
