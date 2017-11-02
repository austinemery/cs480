#include "object.h"

Object::Object(std::string objectName, btTriangleMesh* objTriMesh)
{
  configRead(objectName);
  loadModel(objTriMesh);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, glm::mat4 m)
{
  model = m;
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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data() );
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::configRead(std::string objectName)
{
	std::ifstream configFile;
	std::string tempString = "";
	std::string tempModelName = "../objects/";
	std::string tempTextureName = "../textures/";

	configFile.open("../config/config.txt");

	if (configFile.is_open())
	{
		while(configFile >> tempString)
		{
			if (tempString == "objName")
			{
				configFile >> tempString;
				if (tempString == objectName)
				{
					//std::cout << tempString << std::endl;
					configFile >> tempString;
					if (tempString == "objFileName")
					{
						configFile >> tempString;
						tempModelName.append(tempString);
						modelName = tempModelName;
					}
					//std::cout << tempString << std::endl;
					configFile >> tempString;
					if (tempString == "objTextureName")
					{
						configFile >> tempString;
						tempTextureName.append(tempString);
						textureName = tempTextureName;
					}
					//std::cout << tempString << std::endl;
				}
			}
		}
	}
	else
	{
		std::printf("Error, could not open config file!\nMake sure there is a config file in the config folder!\n");
		exit(EXIT_FAILURE);
	}
	configFile.close();
}

void Object::loadModel(btTriangleMesh* objTriMesh)
{
  Assimp::Importer Importer;

  btVector3 triArray[3];

  const aiScene* pScene = Importer.ReadFile(modelName.c_str(), aiProcess_Triangulate);
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
    for (int jindex = 0; jindex < 3; jindex++)
    {
    	aiVector3D position = pScene->mMeshes[0]->mVertices[face.mIndices[jindex]];
    	triArray[jindex] = btVector3(position.x, position.y, position.z);
    	Indices.push_back( face.mIndices[jindex] );
    }
    objTriMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
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

  image.read(textureName);
  image.write(&blob, "RGBA");
}

void Object::copyPlanetValues(Object* originObj)
{
  p_rotationValue = originObj->p_rotationValue;
  p_orbitValue = originObj->p_orbitValue;
}

void Object::updatePhysics()
{
	//dynamicsWorld->stepSimulation(getDT(), 10); 
	//boardRigidBody->getMotionState()->getWorldTransform(trans);
	//trans.getOpenGLMatrix(m); 
	//model = glm::make_mat4(m);
}