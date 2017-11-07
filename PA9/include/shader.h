#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <fstream>
#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable(int);
    bool AddShader(GLenum ShaderType, std::string);
    std::string LoadShader(GLenum ShaderType, std::string);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName, int);

  private:
      
    GLuint m_vertexLightProg; 
    GLuint m_fragmentLightProg;
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
