#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  angle = 0.0f;
  p_orbitValue = 0.0f;
  p_rotationValue = 0.0f;

  m_orbitValue = 0.0f;
  m_rotationValue = 0.0f;

  isMoon = 0;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, float p_rotationDir, float p_orbitDir, float m_rotationDir, float m_orbitDir)
{
  if (isMoon == 0)
  {
    if (p_orbitDir != 0)
    {
      if (p_orbitDir == 1)
      {
        p_orbitValue += dt * M_PI/1000;
      }
      else
      {
        p_orbitValue -= dt * M_PI/1000;
      }
    }

    if (p_rotationDir != 0)
    {
      if (p_rotationDir == 1)
      {
        p_rotationValue += dt * M_PI/600;
      }
      else
      {
        p_rotationValue -= dt * M_PI/600;
      }
    }
  }
  else
  {
    if (m_orbitDir != 0)
    {
      if (m_orbitDir == 1)
      {
        m_orbitValue += dt * M_PI/800;
      }
      else
      {
        m_orbitValue -= dt * M_PI/800;
      }
    }

    if (m_rotationDir != 0)
    {
      if (m_rotationDir == 1)
      {
        m_rotationValue += dt * M_PI/600;
      }
      else
      {
        m_rotationValue -= dt * M_PI/600;
      }
    }
  }

  //std::cout << "Orbit: " << orbitValue << " Rotate: " << rotationValue << std::endl;

  //Handles the translation of the object
  glm::mat4 p_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
  //Handles the motion for the orbit
  glm::mat4 p_orbitMatrix = glm::rotate(glm::mat4(1.0f), (p_orbitValue), glm::vec3(0.0, 1.0, 0.0));
  //Handles the rotation of the object itself
  glm::mat4 p_rotateMatrix = glm::rotate(glm::mat4(1.0f), (p_rotationValue), glm::vec3(0.0, 1.0, 0.0));

  model = p_orbitMatrix * p_translateMatrix * p_rotateMatrix;

  if (isMoon == 1)
  {
    //std::cout << "Moon: " << isMoon << std::endl;
    glm::mat4 m_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 0.0f));
    glm::mat4 m_orbitMatrix = glm::rotate(glm::mat4(1.0f), (m_orbitValue), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 m_rotateMatrix = glm::rotate(glm::mat4(1.0f), (m_rotationValue), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 m_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.8f));

    model = m_scaleMatrix * p_orbitMatrix * p_translateMatrix * m_orbitMatrix * m_translateMatrix * m_rotateMatrix;
  }
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::changeToMoon()
{
  isMoon = 1;
}

void Object::copyPlanetValues(Object* originObj)
{
  p_rotationValue = originObj->p_rotationValue;
  p_orbitValue = originObj->p_orbitValue;
}