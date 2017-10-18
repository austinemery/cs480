#include <iostream>
#include <fstream>

#include "engine.h"
#include <string.h>

int main(int argc, char **argv)
{
  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Solar System - PA7 Lee Emery Miley ", 1200, 900);

  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}