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

#include "../localisation/Date.h"
#include "../world/MapGen.h"
#include "../world/Map.h"
#include "../world/Location.hpp"

namespace Seasons 
{
    float gSnowLevel = 0.0f;

    const sint32 winterStart = MONTH_JANUARY;
    const sint32 winterEnd = MONTH_FEBRUARY;

    float winter_level(sint32 months, sint32 monthTicks)
    {
        sint32 month = date_get_month(months);
        float monthFraction = monthTicks / (float)0x10000;

        float winterLevel = 0.0f;
        bool invalidate = false;

        if (month == winterStart)
        {
            winterLevel =  monthFraction * 100.0f;
            if (winterLevel > 1.0f)
                winterLevel = 1.0f;
            else
                invalidate = true;
        }
        else if (month == winterEnd)
        {
            winterLevel =  1.0f - monthFraction;
            invalidate = true;
        }
        else if (month > winterStart && (month < winterEnd || winterStart > winterEnd))
        {
            winterLevel =  1.0f;
        }
        else if (month < winterEnd && (month > winterStart || winterStart > winterEnd))
        {
            winterLevel =  1.0f;
        }

        if (invalidate)
        {
            LocationXY16 prevMins = { 0, 0 };
            LocationXY16 prevMaxs = { gMapSizeUnits, gMapSizeUnits };
            map_invalidate_region(prevMins, prevMaxs);
        }

        return winterLevel;
    }

    void update()
    {
#ifdef OLI414_SEASONS_SNOW
        gSnowLevel = winter_level(gDateMonthsElapsed, gDateMonthTicks);
#endif // OLI414_SEASONS_SNOW
    }

    bool should_have_snow(sint32 x, sint32 y)
    {
        if (date_is_winter(gDateMonthsElapsed))
        {
            return mapgen_has_snow(x, y, gSnowLevel);
        }
        return false;
    }

}
