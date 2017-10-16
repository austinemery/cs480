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
    void Enable();
    bool AddShader(std::string shader, GLenum);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    std::string file2string( std::string in );

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
