#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <assimp/vector3.h>

#include <Magick++.h>

#include <SDL2/SDL.h>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Vertex
{
  glm::vec3 vertex;
  glm::vec2 uv;

  Vertex(glm::vec3 v, glm::vec2 u): vertex(v), uv(u) {}
};

struct objectStruct
{
    std::string objectName = "";
    std::string objectModel = "";
    std::string objectTexture = "";
    float distanceFromSun = 0;
    float diameter = 0;
    float rotationPeriodHours = 0;
    float orbitPeriodDays = 0;
    int moonOf = -1;
};

#endif /* GRAPHICS_HEADERS_H */
