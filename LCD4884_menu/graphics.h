/* -*- encoding: utf-8; indent-tabs-mode: nil -*- */

/***************************************************************
 *
 * Copyright (c) 2012, Tan Menglong <tanmenglong@gmail.com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GPL licence
 *
 **************************************************************/

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

/**
 * 
 *
 * @file graphics.h
 * @author Tan Menglong <tanmenglong@gmail.com>
 * @date Mon Oct 15 11:04:39 2012
 *
 **/

#include "LCD4884.h"
#include "common.h"

#define DISPLAY_NORMAL 0
#define DISPLAY_HIGHLIGHT 1

#define GRAPH_WIDTH  84
#define GRAPH_HEIGHT 48

typedef struct graph {
    byte video_mem[GRAPH_HEIGHT / 8][GRAPH_WIDTH];
    byte video_mem_status[GRAPH_HEIGHT / 8][GRAPH_WIDTH];
    LCD4884 lcd;
    int backlight_on;
} graph_t;

void graph_init(graph_t *graph_ptr);
void graph_clear(graph_t *graph_ptr);
void graph_backlight_on(graph_t *graph_ptr);
void graph_backlight_off(graph_t *graph_ptr);
int graph_print_string(graph_t *graph_ptr,
                       unsigned char x, unsigned char y, char *str,
                       char mode);
void graph_clear_status(graph_t *graph_ptr);
int graph_flush(graph_t *graph_ptr);
int graph_isxy_on(graph_t *graph_ptr, const int row, const int col);
int graph_setxy_on(graph_t *graph_ptr, const int x, const int y);
int graph_setxy_off(graph_t *graph_ptr, const int x, const int y);

#endif /* _GRAPHICS_H_ */

/* vim: set expandtab shiftwidth=4 tabstop=4: */
