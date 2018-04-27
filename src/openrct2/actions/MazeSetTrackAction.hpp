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

#include "../core/MemoryStream.h"
#include "../localisation/StringIds.h"
#include "GameAction.h"

#include "../Cheats.h"
#include "../interface/Window.h"
#include "../localisation/Localisation.h"
#include "../management/Finance.h"
#include "../ride/Track.h"
#include "../ride/TrackData.h"
#include "../world/Footpath.h"
#include "../world/Park.h"

/** rct2: 0x00993CE9 */
static constexpr const uint8 byte_993CE9[] =
{
    0xFF, 0xE0, 0xFF,
    14, 0, 1, 2,
    6, 2, 4, 5,
    9, 10, 6, 8,
    12, 13, 14, 10,
};

/** rct2: 0x00993CFC */
static constexpr const uint8 byte_993CFC[] =
{
    5, 12, 0xFF, 0xFF, 9, 0, 0xFF, 0xFF, 13, 4, 0xFF, 0xFF, 1, 8, 0xFF, 0xFF
};

/** rct2: 0x00993D0C */
static constexpr const uint8 byte_993D0C[] =
{
    3, 0, 0xFF, 0xFF, 0, 1, 0xFF, 0xFF, 1, 2, 0xFF, 0xFF, 2, 3, 0xFF, 0xFF
};


struct MazeSetTrackAction : public GameActionBase<GAME_COMMAND_SET_MAZE_TRACK, GameActionResult>
{
private:
    uint16 _x;
    uint16 _y;
    uint16 _z;
    bool _initialPlacement;
    uint8 _direction;
    uint8 _rideIndex;
    uint8 _mode;

public:
    MazeSetTrackAction() {}
    MazeSetTrackAction(uint16 x, uint16 y, uint16 z, bool initialPlacement, uint8 direction, uint8 rideIndex, uint8 mode)
        : _x(x),
        _y(y),
        _z(z),
        _initialPlacement(initialPlacement),
        _direction(direction),
        _rideIndex(rideIndex),
        _mode(mode)
    {
    }

    uint16 GetActionFlags() const override
    {
        return GameAction::GetActionFlags() | GA_FLAGS::ALLOW_WHILE_PAUSED;
    }

    void Serialise(DataSerialiser& stream) override
    {
        GameAction::Serialise(stream);

        stream << _x << _y << _z << _initialPlacement << _direction << _rideIndex << _mode;
    }

    GameActionResult::Ptr Query() const override
    {
        auto res = std::make_unique<GameActionResult>();

        res->Position.x = _x + 8;
        res->Position.y = _y + 8;
        res->Position.z = _z;
        res->ExpenditureType = RCT_EXPENDITURE_TYPE_RIDE_CONSTRUCTION;
        res->ErrorTitle = STR_RIDE_CONSTRUCTION_CANT_CONSTRUCT_THIS_HERE;

        if (!map_check_free_elements_and_reorganise(1))
        {
            res->Error = GA_ERROR::NO_FREE_ELEMENTS;
            res->ErrorMessage = STR_NONE;
            return res;
        }

        if ((_z & 0xF) != 0)
        {
            res->Error = GA_ERROR::UNKNOWN;
            res->ErrorMessage = STR_CONSTRUCTION_ERR_UNKNOWN;
            return res;
        }

        if (!map_is_location_owned(floor2(_x, 32), floor2(_y, 32), _z) && !gCheatsSandboxMode)
        {
            res->Error = GA_ERROR::NOT_OWNED;
            res->ErrorMessage = STR_NONE;
            return res;
        }

        rct_tile_element * tileElement = map_get_surface_element_at(_x / 32, _y / 32);
        if (tileElement == nullptr)
        {
            res->Error = GA_ERROR::UNKNOWN;
            res->ErrorMessage = STR_INVALID_SELECTION_OF_OBJECTS;
            return res;
        }

        uint8 baseHeight = _z >> 3;
        uint8 clearanceHeight = (_z + 32) >> 3;

        sint8 heightDifference = baseHeight - tileElement->base_height;
        if (heightDifference >= 0 && !gCheatsDisableSupportLimits)
        {
            heightDifference = heightDifference >> 1;

            if (heightDifference > RideData5[RIDE_TYPE_MAZE].max_height)
            {
                res->Error = GA_ERROR::TOO_HIGH;
                res->ErrorMessage = STR_TOO_HIGH_FOR_SUPPORTS;
                return res;
            }
        }

        tileElement = map_get_track_element_at_of_type_from_ride(_x, _y, baseHeight, TRACK_ELEM_MAZE, _rideIndex);
        if (tileElement == nullptr)
        {
            if (_mode != GC_SET_MAZE_TRACK_BUILD)
            {
                res->Error = GA_ERROR::UNKNOWN;
                res->ErrorMessage = STR_INVALID_SELECTION_OF_OBJECTS;
                return res;
            }

            if (!map_can_construct_at(floor2(_x, 32), floor2(_y, 32), baseHeight, clearanceHeight, 0x0F))
            {
                res->Error = GA_ERROR::NO_CLEARANCE;
                res->ErrorMessage = STR_NONE;
                return res;
            }

            if (gMapGroundFlags & ELEMENT_IS_UNDERWATER)
            {
                res->Error = GA_ERROR::NO_CLEARANCE;
                res->ErrorMessage = STR_RIDE_CANT_BUILD_THIS_UNDERWATER;
                return res;
            }

            if (gMapGroundFlags & ELEMENT_IS_UNDERGROUND)
            {
                res->Error = GA_ERROR::NO_CLEARANCE;
                res->ErrorMessage = STR_CAN_ONLY_BUILD_THIS_ABOVE_GROUND;
                return res;
            }

            Ride * ride = get_ride(_rideIndex);
            if (ride == nullptr || ride->type == RIDE_CRASH_TYPE_NONE)
            {
                res->Error = GA_ERROR::NO_CLEARANCE;
                res->ErrorMessage = STR_INVALID_SELECTION_OF_OBJECTS;
                return res;
            }

            money32 price = (((RideTrackCosts[ride->type].track_price * TrackPricing[TRACK_ELEM_MAZE]) >> 16));
            res->Cost = price / 2 * 10;

            return res;
        }

        return std::make_unique<GameActionResult>();
    }

    GameActionResult::Ptr Execute() const override
    {
        auto res = std::make_unique<GameActionResult>();

        res->Position.x = _x + 8;
        res->Position.y = _y + 8;
        res->Position.z = _z;
        res->ExpenditureType = RCT_EXPENDITURE_TYPE_RIDE_CONSTRUCTION;
        res->ErrorTitle = STR_RIDE_CONSTRUCTION_CANT_CONSTRUCT_THIS_HERE;

        if (!map_check_free_elements_and_reorganise(1))
        {
            res->Error = GA_ERROR::NO_FREE_ELEMENTS;
            res->ErrorMessage = STR_NONE;
            return res;
        }

        uint32 flags = GetFlags();
        if (!(flags & GAME_COMMAND_FLAG_GHOST) && !(flags & GAME_COMMAND_FLAG_2))
        {
            footpath_remove_litter(_x, _y, _z);
            wall_remove_at(floor2(_x, 32), floor2(_y, 32), _z, _z + 32);
        }

        uint8 baseHeight = _z >> 3;
        uint8 clearanceHeight = (_z + 32) >> 3;

        rct_tile_element * tileElement = map_get_track_element_at_of_type_from_ride(_x, _y, baseHeight, TRACK_ELEM_MAZE, _rideIndex);
        if (tileElement == nullptr)
        {
            Ride * ride = get_ride(_rideIndex);
            openrct2_assert(ride != nullptr, "Invalid ride index: %d\n", _rideIndex);

            money32 price = (((RideTrackCosts[ride->type].track_price * TrackPricing[TRACK_ELEM_MAZE]) >> 16));
            res->Cost = price / 2 * 10;

            uint16 flooredX = floor2(_x, 32);
            uint16 flooredY = floor2(_y, 32);

            tileElement = tile_element_insert(_x / 32, _y / 32, baseHeight, 0xF);
            assert(tileElement != nullptr);

            tileElement->clearance_height = clearanceHeight;
            tileElement->type = TILE_ELEMENT_TYPE_TRACK;

            track_element_set_type(tileElement, TRACK_ELEM_MAZE);
            track_element_set_ride_index(tileElement, _rideIndex);
            tileElement->properties.track.maze_entry = 0xFFFF;

            if (flags & GAME_COMMAND_FLAG_GHOST)
            {
                tileElement->flags |= TILE_ELEMENT_FLAG_GHOST;
            }

            map_invalidate_tile_full(flooredX, flooredY);

            ride->maze_tiles++;
            ride->station_heights[0] = tileElement->base_height;
            ride->station_starts[0].xy = 0;

            if (_initialPlacement && !(flags & GAME_COMMAND_FLAG_GHOST))
            {
                ride->overall_view.x = flooredX / 32;
                ride->overall_view.y = flooredY / 32;
            }
        }

        switch (_mode)
        {
        case GC_SET_MAZE_TRACK_BUILD:
        {
            uint8 segmentOffset = MazeGetSegmentBit(_x, _y);

            tileElement->properties.track.maze_entry &= ~(1 << segmentOffset);

            if (!_initialPlacement)
            {
                segmentOffset = byte_993CE9[(_direction + segmentOffset)];
                tileElement->properties.track.maze_entry &= ~(1 << segmentOffset);

                uint8 temp_edx = byte_993CFC[segmentOffset];
                if (temp_edx != 0xFF)
                {
                    uint16 previousElementX = floor2(_x, 32) - TileDirectionDelta[_direction].x;
                    uint16 previousElementY = floor2(_y, 32) - TileDirectionDelta[_direction].y;

                    rct_tile_element * previousTileElement = map_get_track_element_at_of_type_from_ride(
                        previousElementX,
                        previousElementY,
                        baseHeight,
                        TRACK_ELEM_MAZE,
                        _rideIndex);

                    if (previousTileElement != nullptr)
                    {
                        previousTileElement->properties.track.maze_entry &= ~(1 << temp_edx);
                    }
                    else
                    {
                        tileElement->properties.track.maze_entry |= (1 << segmentOffset);
                    }
                }
            }

            break;
        }

        case GC_SET_MAZE_TRACK_MOVE:
            break;

        case GC_SET_MAZE_TRACK_FILL:
            if (!_initialPlacement)
            {
                uint16 previousSegmentX = _x - TileDirectionDelta[_direction].x / 2;
                uint16 previousSegmentY = _y - TileDirectionDelta[_direction].y / 2;

                tileElement = map_get_track_element_at_of_type_from_ride(
                    previousSegmentX,
                    previousSegmentY,
                    baseHeight,
                    TRACK_ELEM_MAZE,
                    _rideIndex);

                map_invalidate_tile_full(floor2(previousSegmentX, 32), floor2(previousSegmentY, 32));
                if (tileElement == nullptr)
                {
                    log_error("No surface found\n");
                    res->Error = GA_ERROR::UNKNOWN;
                    res->ErrorMessage = STR_NONE;
                    return res;
                }

                uint32 segmentBit = MazeGetSegmentBit(previousSegmentX, previousSegmentY);

                tileElement->properties.track.maze_entry |= (1 << segmentBit);
                segmentBit--;
                tileElement->properties.track.maze_entry |= (1 << segmentBit);
                segmentBit = (segmentBit - 4) & 0x0F;
                tileElement->properties.track.maze_entry |= (1 << segmentBit);
                segmentBit = (segmentBit + 3) & 0x0F;

                do
                {
                    tileElement->properties.track.maze_entry |= (1 << segmentBit);

                    uint32 direction1 = byte_993D0C[segmentBit];
                    uint16 nextElementX = floor2(previousSegmentX, 32) + TileDirectionDelta[direction1].x;
                    uint16 nextElementY = floor2(previousSegmentY, 32) + TileDirectionDelta[direction1].y;

                    rct_tile_element * tmp_tileElement = map_get_track_element_at_of_type_from_ride(
                        nextElementX,
                        nextElementY,
                        baseHeight,
                        TRACK_ELEM_MAZE,
                        _rideIndex);

                    if (tmp_tileElement != nullptr)
                    {
                        uint8 edx11 = byte_993CFC[segmentBit];
                        tmp_tileElement->properties.track.maze_entry |= 1 << (edx11);
                    }

                    segmentBit--;
                } while ((segmentBit & 0x3) != 0x3);
            }
            break;
        }

        map_invalidate_tile(floor2(_x, 32), floor2(_y, 32), tileElement->base_height * 8, tileElement->clearance_height * 8);

        if ((tileElement->properties.track.maze_entry & 0x8888) == 0x8888)
        {
            Ride * ride = get_ride(_rideIndex);
            tile_element_remove(tileElement);
            sub_6CB945(_rideIndex);
            ride->maze_tiles--;
        }

        return res;
    }

private:
    uint8 MazeGetSegmentBit(uint16 x, uint16 y) const
    {
        uint8 minorX = x & 0x1F; // 0 or 16
        uint8 minorY = y & 0x1F; // 0 or 16

        if (minorX == 0 && minorY == 0)
        {
            return 3;
        }

        if (minorY == 16 && minorX == 16)
        {
            return 11;
        }

        if (minorY == 0)
        {
            return 15;
        }

        return 7;
    }
};
