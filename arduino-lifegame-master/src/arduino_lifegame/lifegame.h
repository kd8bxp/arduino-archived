/* -*- encoding: utf-8; indent-tabs-mode: nil -*- */

/***************************************************************
 *
 * Copyright (c) 2012, Tan Menglong <tanmenglong@gmail.com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GPL licence
 *
 **************************************************************/

#ifndef _LIFEGAME_H_
#define _LIFEGAME_H_

/**
 * 
 *
 * @file lifegame.h
 * @author Tan Menglong <tanmenglong@gmail.com>
 * @date Mon Oct 15 11:03:09 2012
 *
 **/

#include "graphics.h"

typedef struct {
  graph_t *graph_ptr;
  int count;
} world_t;

void plot_world(world_t *world_ptr);
void init_world(world_t *world_ptr, graph_t *graph_ptr, const int x);
void reproduce(world_t *graph_ptr);

#endif /* _LIFEGAME_H_ */

/* vim: set expandtab shiftwidth=4 tabstop=4: */
