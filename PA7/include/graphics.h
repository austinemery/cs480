#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "graphics_headers.h"
#include "mycamera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt);
    void Render();
    void readConfig();

    Object* getCube();

    
  private:
    std::string ErrorString(GLenum error);

    MyCamera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_modelMatrix2;

    GLuint aTexture;

    Object *Sun;
    Object *Earth;
    
    std::string vShader;
    std::string fShader;

    int numbObjects;

    std::vector<objectStruct> objectVector;
};

#endif /* GRAPHICS_H */
