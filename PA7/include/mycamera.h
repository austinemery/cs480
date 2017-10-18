#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class MyCamera
{
  public:
    MyCamera();
    ~MyCamera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  	void updateCameraPosition(objectStruct objectData, glm::vec2);
    void updateCameraPosition(glm::vec3 data);

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 currentPos;
};

#endif /* CAMERA_H */
