#ifndef HW2_VISUALIZER
#define HW2_VISUALIZER

#include <iostream>
#include <array>
#include <map>
#include <string>
#include "SDL.h"

#define WIDTH 1366
#define HEIGHT 803

struct plane_draw
{
  SDL_Vertex v1, v2, v3;
};

class Plane_Viz
{
public:
  std::map<std::pair<std::string, std::string>, double> airspace = { { {"SCE","ORD"}, 640}, // miles
                                                                   { {"ORD","SCE"}, 640},
                                                                   { {"SCE","PHL"}, 160},
                                                                   { {"PHL","SCE"}, 160},
                                                                   { {"SCE","EWR"}, 220},
                                                                   { {"EWR","SCE"}, 220},
  };
  std::map<std::string, std::pair<int, int>> airport_location = { {"SCE", {870,400}}, 
                                                                  {"ORD", {235,475}},
                                                                  {"PHL", {1030,425}},
                                                                  {"EWR", {1080,345}},
  };
  std::map<std::string, SDL_Color> plane_colors = { {"AA", {255,0,0,255}}, {"UA", {0,0,255,255}}, {"NA", {0,255,0,255}} };
  std::map<std::array<std::string,3>, plane_draw> planes_info;
};

class HW2_VIZ
{
  bool quit = false;
  SDL_Event event;
  SDL_Window* win;
  SDL_Renderer* renderer;
  SDL_Surface* image;
  SDL_Texture* texture;
  SDL_Rect dest;
  Plane_Viz p_viz;
public:
	HW2_VIZ ();

  void visualize_plane (std::string airline, std::string origin, std::string des, double pos);
  void update (double dt);
  bool close();

  ~HW2_VIZ();
};

#endif // !HW2_VISUALIZER