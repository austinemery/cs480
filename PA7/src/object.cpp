#include "object.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>  

#define ORBITAL_SPEED_RATIO .1
#define ORBIT_R 0

using namespace std;

Object::Object(objectStruct objectData)
{  

  angle_orbit = 0.0f;
  angle_rot = 0.0f;

  orbit_rev = false;
  rot_rev = false;
  orbit_stop = false;
  rot_stop = false;

  loadModel(objectData.objectModel, objectData.objectTexture);

}

void Object::loadModel(std::string model, std::string texture)
{
  
  
  Assimp::Importer Importer;
  const aiScene* pScene = Importer.ReadFile(model.c_str(), aiProcess_Triangulate);
  const aiMaterial* pMtl = pScene->mMaterials[pScene->mMeshes[0]->mMaterialIndex];
  
  for (int index = 0; index < pScene->mMeshes[0]->mNumVertices; index++) 
  {   
    aiVector3D uv = pScene->mMeshes[0]->mTextureCoords[0][ index ];

	  Vertices.push_back( 
	    {
	      {pScene->mMeshes[0]->mVertices[index].x,pScene->mMeshes[0]->mVertices[index].y,pScene->mMeshes[0]->mVertices[index].z},
	      {uv.x,1 - uv.y}
	    });
  }

  for(int index = 0; index < pScene->mMeshes[0]->mNumFaces; index++)
  {
    const aiFace face = pScene->mMeshes[0]->mFaces[index];
    Indices.push_back( face.mIndices[0] );
    Indices.push_back( face.mIndices[1] );
    Indices.push_back( face.mIndices[2] );
  }
  
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
  
  glGenTextures(1, &aTexture);
  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, aTexture );
  
}


Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, objectStruct objectData)
{
  
  if( !orbit_stop )
  {
    if( !orbit_rev )
    {
      angle_orbit += dt * M_PI/1000;
    }
    else
    {
      angle_orbit -= dt * M_PI/1000;
    }
  }

  if( !rot_stop )
  {
    if( !rot_rev )
    {
      angle_rot += dt * M_PI/4000;
    }
    else
    {
      angle_rot -= dt * M_PI/4000;
    }
  }

  origin.x = ORBIT_R * cos( angle_orbit * ORBITAL_SPEED_RATIO  );
  origin.y = ORBIT_R * sin( angle_orbit * ORBITAL_SPEED_RATIO  );
  
  //std::cout << origin.x << " " << origin.y << std::endl;

  model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2) );

  model = glm::translate(model, glm::vec3(origin.x, 0.0, origin.y));

  if (objectData.objectName.find("Earth") != std::string::npos)
  {
  	model = glm::translate(model, glm::vec3(2.0, 0.0, 2.0));
  }

  if (objectData.objectName.find("Venus") != std::string::npos)
  {
    model = glm::translate(model, glm::vec3(-2.0, 0.0, 2.0));
  }
                            
  model = glm::rotate(model, (angle_rot), glm::vec3(0.0, 1.0, 0.0));

  std::vector<float> v;
  
  
  
  
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(std::string texture)
{ 
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

   //Assimp Stuff

  Magick::Image image;
  Magick::Blob blob;

  image.read(texture);
  image.write(&blob, "RGBA");

  //cout << image.columns() << endl;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data() );
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  
}

void Object::invertOrbitRev()
{
  if( orbit_rev == 0 )
      {
        orbit_rev = 1;
      }
      else
      {
        orbit_rev = 0;
      }
}

void Object::invertOrbitStop()
{
  if( orbit_stop == 0 )
      {
        orbit_stop = 1;
      }
      else
      {
        orbit_stop = 0;
      }
}

void Object::invertRotRev()
{
  if( rot_rev == 0 )
      {
        rot_rev = 1;
      }
      else
      {
        rot_rev = 0;
      }
}

void Object::invertRotStop()
{
  if( rot_stop == 0 )
      {
        rot_stop = 1;
      }
      else
      {
        rot_stop = 0;
      }
}

glm::vec2 Object::getOrigin()
{
  return origin;
}

void Object::rotateLeft()
{
  rot_rev = false;
}

void Object::rotateRight()
{
  rot_rev = true;
}

int Object::getNumberOfSlashes( std::string inString )
{
  int index, counter;

  for( index = counter = 0; index < inString.size(); ++index )
  {
    if( inString[ index ] == '/' )
    {
      counter++;
    }
  }

  return counter;
}

int Object::getNumber( std::string &inString )
{
  std::string temp;
  int index = 0;
  while( 1 )
  {
    if( inString.size() == index || inString[ index ] == '/' )
    {
      std::string newString(  inString.begin() + index + 1, inString.end() + 1);
      //cout << newString << ' ';
      inString = newString;
      return atoi( temp.c_str() );
    }
    temp.push_back( inString[ index ]);
    index++;
  }
}

void Object::setPlanetVals( float a, float b, float c, float d )
{
  distanceFromSun = a;
  diameter = b;
  orbitPeriod = c;
  rotationPeriod = d;
}