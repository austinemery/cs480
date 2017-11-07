#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include "graphics_headers.h"
using namespace std;

class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void RunMenu();
    void Swap();

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
  bool show_test_window;
  bool show_another_window;
  ImVec4 clear_color;
};

#endif /* WINDOW_H */
