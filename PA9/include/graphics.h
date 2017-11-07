#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "physics.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render(int);
    Camera* getCamera();
    Physics* getPhysics();
    bool shaderSwap(int);

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    Physics *m_physics;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *planet_1;
    Object *moon_1;
    Object *cube_1;
    Object *cylinder_1;
    Object *ground_1;
};

#endif /* GRAPHICS_H */
