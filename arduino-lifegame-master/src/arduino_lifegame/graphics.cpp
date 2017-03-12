/* -*- encoding: utf-8; indent-tabs-mode: nil -*- */

/***************************************************************
 *
 * Copyright (c) 2012, crackcell <tanmenglong@gmail.com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GPL licence
 *
 **************************************************************/

/**
 * 
 *
 * @file graphics.cpp
 * @author Tan Menglong <tanmenglong@gmail.com>
 * @date Mon Oct 15 10:02:01 2012
 *
 **/

#include "graphics.h"

#include <stdio.h>
#include <string.h>

void graph_init(graph_t *graphics_ptr) {
    graphics_ptr->lcd.LCD_init();
    memset(graphics_ptr->video_mem, 0,
           sizeof(graphics_ptr->video_mem));
    memset(graphics_ptr->video_mem_status, 0,
           sizeof(graphics_ptr->video_mem_status));
    graphics_ptr->backlight_on = 0;
}

void graph_clear(graph_t *graphics_ptr) {
    graphics_ptr->lcd.LCD_clear();
}

void graph_backlight_on(graph_t *graphics_ptr) {
    graphics_ptr->lcd.backlight(ON);
    graphics_ptr->backlight_on = 1;
}

void graph_backlight_off(graph_t *graph_ptr) {
    graph_ptr->lcd.backlight(OFF);
    graph_ptr->backlight_on = 0;
}

int graph_print_string(graph_t *graph_ptr,
                        unsigned char x, unsigned char y, char *str,
                        char mode) {
    graph_ptr->lcd.LCD_write_string(x, y, str, mode);
    return 0;
}

void graph_clear_status(graph_t *graph_ptr) {
    memset(graph_ptr->video_mem_status, 0,
           sizeof(graph_ptr->video_mem_status));
}

int graph_flush(graph_t *graph_ptr) {
    if (graph_ptr == NULL) {
        return ERR_PARAM;
    }

    int x, y;
    for (x = 0; x < GRAPH_HEIGHT / 8; x ++) {
        for (y = 0; y < GRAPH_WIDTH; y++) {
            if (graph_ptr->video_mem_status[x][y]) {
                graph_ptr->lcd.LCD_set_XY(y, x);
                graph_ptr->lcd.LCD_write_byte(graph_ptr->video_mem[x][y], 1);
            }
        }
    }

    return ERR_OK;
}

int graph_isxy_on(graph_t *graph_ptr, const int row, const int col) {
    byte offset = row % 8;
    int x2 = int(row / 8);
    return graph_ptr->video_mem[x2][col] & (1 << offset);
}

int graph_setxy_on(graph_t *graph_ptr, const int row, const int col) {
    if (graph_ptr == NULL ||
        row >= GRAPH_HEIGHT || col >= GRAPH_WIDTH) {
        return ERR_PARAM;
    }

    byte offset = row % 8;
    int x2 = int(row / 8);
    graph_ptr->video_mem[x2][col] |= (1 << offset);
    graph_ptr->video_mem_status[x2][col] = 1; // changed

    return ERR_OK;
}

int graph_setxy_off(graph_t *graph_ptr,
                     const int x, const int y) {
    if (graph_ptr == NULL ||
        x >= GRAPH_HEIGHT || y >= GRAPH_WIDTH) {
        return ERR_PARAM;
    }

    byte offset = x % 8;
    int x2 = int(x / 8);
    graph_ptr->video_mem[x2][y] &= ~(1 << offset);
    graph_ptr->video_mem_status[x2][y] = 1; // changed

    return ERR_OK;
}

/* vim: set expandtab shiftwidth=4 tabstop=4: */
