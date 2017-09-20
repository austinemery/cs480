#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt, float, float, float, float);
    void Render();
    void changeToMoon();
    void copyPlanetValues(Object*);


    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle;
    float p_rotationValue;
    float p_orbitValue;

    float m_rotationValue;
    float m_orbitValue;

    bool isMoon;
};

#endif /* OBJECT_H */
