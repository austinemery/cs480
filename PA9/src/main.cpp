#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
  //btCollisionShape* fallShape = new btSphereShape(1);

  for (int i = 0; i < 300; i++) {
          //std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
  }

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600);
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
