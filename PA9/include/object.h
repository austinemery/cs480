#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <fstream>
#include "graphics_headers.h"

class Object
{
  public:
    Object(std::string, btTriangleMesh*);
    ~Object();
    void Update(unsigned int dt, glm::mat4);
    void Render();
    void configRead(std::string);
    void copyPlanetValues(Object*);
    void loadModel(btTriangleMesh*);
    void updatePhysics();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint aTexture;
    GLuint IB;

    float angle;
    float p_rotationValue;
    float p_orbitValue;

    float m_rotationValue;
    float m_orbitValue;

    std::string textureName;
    std::string modelName;

    Magick::Image image;
    Magick::Blob blob;
};

#endif /* OBJECT_H */
