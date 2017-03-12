#include "graphics.h"
#include "lifegame.h"

graph_t g_graph;
world_t g_world;

void setup() {
  graph_init(&g_graph);
  graph_clear(&g_graph);
  graph_backlight_on(&g_graph);
  init_world(&g_world, &g_graph, 3500);
  plot_world(&g_world);
}

void loop() {
  graph_clear_status(&g_graph);
  reproduce(&g_world);
  plot_world(&g_world);
  if (g_world.count == 0) {
    init_world(&g_world, &g_graph, 3500);
    plot_world(&g_world);
  }
  delay(1000);
}
