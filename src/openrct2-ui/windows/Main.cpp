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

#include <openrct2/Context.h>
#include <openrct2-ui/windows/Window.h>

#include <openrct2-ui/interface/Viewport.h>
#include <openrct2-ui/interface/Widget.h>
#include <openrct2/localisation/StringIds.h>
#include <openrct2/world/Footpath.h>

static rct_widget window_main_widgets[] = {
    { WWT_VIEWPORT, 0, 0x0000, -1, 0x0000, -1, STR_VIEWPORT, 0xFFFF },
    { WIDGETS_END },
};

void window_main_paint(rct_window *w, rct_drawpixelinfo *dpi);

static rct_window_event_list window_main_events = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    window_main_paint,
    nullptr
};

/**
 * Creates the main window that holds the main viewport.
 *  rct2: 0x0066B3E8
 */
rct_window * window_main_open()
{
    window_main_widgets[0].right = context_get_width();
    window_main_widgets[0].bottom = context_get_height();
    rct_window * window = window_create(
        0, 0,
        window_main_widgets[0].right, window_main_widgets[0].bottom,
        &window_main_events,
        WC_MAIN_WINDOW,
        WF_STICK_TO_BACK
    );
    window->widgets = window_main_widgets;

    viewport_create(window, window->x, window->y, window->width, window->height, 0,0x0FFF,0x0FFF, 0, 0x1, -1);
    window->viewport->flags |= VIEWPORT_FLAG_SOUND_ON;
    gCurrentRotation = 0;
    gShowGridLinesRefCount = 0;
    gShowLandRightsRefCount = 0;
    gShowConstuctionRightsRefCount = 0;
    gFootpathSelectedType = 0;

    return window;
}


/**
 *
 *  rct2: 0x66CCAE
 * This function immediately jumps to 0x00685BE1 this is the second function
 * decompiled.
 */
void window_main_paint(rct_window *w, rct_drawpixelinfo *dpi)
{
    viewport_render(dpi, w->viewport, dpi->x, dpi->y, dpi->x + dpi->width, dpi->y + dpi->height);
}
