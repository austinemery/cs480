#include "shader.h"

Shader::Shader()
{
  m_vertexLightProg = 0;
  m_fragmentLightProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_vertexLightProg != 0)
  {
    glDeleteProgram(m_vertexLightProg);
    m_vertexLightProg = 0;
  }

  if (m_fragmentLightProg != 0)
  {
    glDeleteProgram(m_fragmentLightProg);
    m_fragmentLightProg = 0;
  }
}

bool Shader::Initialize()
{
  m_vertexLightProg = glCreateProgram();

  if (m_vertexLightProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  m_fragmentLightProg = glCreateProgram();

  if (m_fragmentLightProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType, std::string shaderFile)
{
  std::string s;
  s = LoadShader(ShaderType, shaderFile);

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  if (shaderFile.find("norm") != std::string::npos)
  {
    glAttachShader(m_vertexLightProg, ShaderObj);
  }
  else
  {
    glAttachShader(m_fragmentLightProg, ShaderObj);
  }

  return true;
}

//Function to load the shaders from a file. 
//Called in AddShader()
std::string Shader::LoadShader(GLenum ShaderType, std::string shaderFile)
{
  std::string s;
  std::string line = "";

  std::string tempString = "../shaders/";
  tempString.append(shaderFile);

  if(ShaderType == GL_VERTEX_SHADER)
  {
    std::ifstream myFile;
    myFile.open(tempString);

    if (myFile.is_open())
    {
      while(std::getline(myFile, line))
      {
        s.append(line);
        s.append("\n");
      }
    }
    else
    {
      printf("Unable to open Vertex Shader File\n");
      return 0;
    }

    myFile.close();
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
    std::ifstream myFile;
    myFile.open(tempString);

    if (myFile.is_open())
    {
      while(std::getline(myFile, line))
      {
        s.append(line);
        s.append("\n");
      }
    }
    else
    {
      printf("Unable to open Fragment Shader File\n");
      return 0;
    }

    myFile.close();
  }

  return s;
}

// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_vertexLightProg);

  glGetProgramiv(m_vertexLightProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_vertexLightProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_vertexLightProg);
  glGetProgramiv(m_vertexLightProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_vertexLightProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  glLinkProgram(m_fragmentLightProg);

  glGetProgramiv(m_fragmentLightProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_fragmentLightProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_fragmentLightProg);
  glGetProgramiv(m_fragmentLightProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_fragmentLightProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}

//selector is 0 or 1
void Shader::Enable(int selector)
{
    if (selector == 0)
    {
      glUseProgram(m_vertexLightProg);
    }
    else 
    {
      glUseProgram(m_fragmentLightProg);
    }
}


GLint Shader::GetUniformLocation(const char* pUniformName, int selector)
{
    GLuint Location;

    if (selector == 0)
    {
      Location = glGetUniformLocation(m_vertexLightProg, pUniformName);
    }
    else
    {
      Location = glGetUniformLocation(m_fragmentLightProg, pUniformName);
    }

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}