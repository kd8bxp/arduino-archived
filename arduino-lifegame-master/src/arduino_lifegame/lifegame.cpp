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
 * @file lifegame.cpp
 * @author Tan Menglong <tanmenglong@gmail.com>
 * @date Mon Oct 15 11:03:21 2012
 *
 **/

#include "lifegame.h"

#include <stdlib.h>

static int __get_neighbour_count(world_t *world_ptr, int row, int col);

void plot_world(world_t *world_ptr) {
  graph_flush(world_ptr->graph_ptr);
}

void init_world(world_t *world_ptr, graph_t *graph_ptr, const int x) {
  int row, col, k, i;
  world_ptr->graph_ptr = graph_ptr;
  world_ptr->count = 0;
  for (k = 0; k < x; k++) {
    i = rand() % (GRAPH_WIDTH * GRAPH_HEIGHT);
    row = int(i / GRAPH_WIDTH);
    col = i % GRAPH_WIDTH;
    graph_setxy_on(world_ptr->graph_ptr, row, col);
    world_ptr->count++;
  }
}

void reproduce(world_t *world_ptr) {
  int r, c, n;
  if (world_ptr->count == 0) {
    return;
  }
  for (r = 0; r < GRAPH_HEIGHT; r++) {
    for (c = 0; c < GRAPH_WIDTH; c++) {
      n = __get_neighbour_count(world_ptr, r, c);
      if (graph_isxy_on(world_ptr->graph_ptr, r, c)) {
        if (n == 2 || n ==3) {
          continue;
        }
        if (n < 2 || n > 3) {
          graph_setxy_off(world_ptr->graph_ptr, r, c);
          world_ptr->count--;
        }
      } else {
        if (n == 3) {
          graph_setxy_on(world_ptr->graph_ptr, r, c);
          world_ptr->count++;
        }
      }
    }
  }
}


static int __get_neighbour_count(world_t *world_ptr, int row, int col) {
  int upleft_row = row == 0 ? 0 : row - 1;
  int upleft_col = col == 0 ? 0 : col - 1;
  int downright_row = row == GRAPH_HEIGHT ? GRAPH_HEIGHT : row + 1;
  int downright_col = col == GRAPH_WIDTH ? GRAPH_WIDTH : col + 1;
  int n_row, n_col;
  int count = 0;
  for (n_row = upleft_row; n_row <= downright_row; n_row++) {
    for (n_col = upleft_col; n_col < downright_col; n_col++) {
      if (n_row == row && n_col == col) {
        continue;
      }
      if (graph_isxy_on(world_ptr->graph_ptr, n_row, n_col)) {
        count++;
      }
    }
  }
  return count;
}

/* vim: set expandtab shiftwidth=4 tabstop=4: */
