#include "object.h"

Object::Object(std::string objectName)
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

  objLoad(objectName);

  /*Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };*/

  /*Indices = {
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
  };*/

  //std::cout << Vertices[0].vertex.x << " " << Vertices[0].vertex.y << " " << Vertices[0].vertex.z << std::endl;
  //std::cout << Vertices[0].color.x << " " << Vertices[0].color.y << " " << Vertices[0].color.z << std::endl;

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

  //glGenBuffers(1, &TB);
  //glBindBuffer(GL_TEXTURE_BUFFER, TB);
  //glBufferData(GL_TEXTURE_BUFFER, sizeof(glm::vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);

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
        p_orbitValue += dt * M_PI/2000;
      }
      else
      {
        p_orbitValue -= dt * M_PI/2000;
      }
    }

    if (p_rotationDir != 0)
    {
      if (p_rotationDir == 1)
      {
        p_rotationValue += dt * M_PI/2000;
      }
      else
      {
        p_rotationValue -= dt * M_PI/2000;
      }
    }
  }

  //std::cout << "Orbit: " << orbitValue << " Rotate: " << rotationValue << std::endl;
  glm::mat4 p_rotateMatrix = glm::rotate(glm::mat4(1.0f), (p_rotationValue), glm::vec3(0.0, 1.0, 0.0));

  model = p_rotateMatrix;
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

void Object::objLoad(std::string objectName)
{
	std::ifstream configFile;
	std::string tempString = "";
	std::string objectFileName = "../objects/";

	configFile.open("../config/config.txt");

	if (configFile.is_open())
	{
		//To find information in the config file
		if(configFile.is_open())
		{
			while(configFile >> tempString)
			{
				if (tempString == "objName")
				{
					configFile >> tempString;
					if (tempString == objectName)
					{
						configFile >> tempString;
						if (tempString == "objFileName")
						{
							configFile >> tempString;
							objectFileName.append(tempString);
							//std::cout << objectFileName << std::endl;
						}
					}
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


	//To read in the obj file
	bool uvFlag = 0;//used to determine if need to read in vt data for faces
	bool normFlag = 0;//used to determine if need to read in vn data for faces

	std::ifstream objectFile;
	int numberOfVs = 0;
	//need to count the v lines to get good color
	objectFile.open(objectFileName);
	if (objectFile.is_open())
	{
		while(objectFile >> tempString)
		{
			if(strcmp(tempString.c_str(), "v") == 0)
			{
				numberOfVs++;
			}
		}
	}
	else
	{
		std::printf("Error, could not open obj file!\nMake sure the obj file is correct in the config file!\n");
		exit(EXIT_FAILURE);
	}

	objectFile.close();

	objectFile.open(objectFileName);

	if(objectFile.is_open())
	{
		glm::vec3 colorVec = {0,0,0};

		while(objectFile >> tempString)
		{
			if(strcmp(tempString.c_str(), "v") == 0)
			{
				glm::vec3 zeroVec = {0,0,0};
	    		Vertex tempVertex(zeroVec, colorVec);
	    		objectFile >> tempVertex.vertex.x >> tempVertex.vertex.y >> tempVertex.vertex.z;
	    		//std::cout << tempVertex.vertex.x << " " << tempVertex.vertex.y << " " << tempVertex.vertex.z << std::endl;
	    		Vertices.push_back(tempVertex);
	    	}
	    	else if(strcmp(tempString.c_str(), "vt") == 0)
			{
				uvFlag = 1;
				glm::vec2 tempUvs;
	    		objectFile >> tempUvs.x >> tempUvs.y;
	    		//std::cout << tempUvs.x << " " << tempUvs.y << " " << std::endl;
	    		uvs.push_back(tempUvs);
	    	}
	    	else if(strcmp(tempString.c_str(), "vn") == 0)
			{
				normFlag = 1;
				glm::vec3 tempNorm;
	    		objectFile >> tempNorm.x >> tempNorm.y >> tempNorm.z;
	    		//std::cout << tempNorm.x << " " << tempNorm.y << " " << tempNorm.z << std::endl;
	    		normals.push_back(tempNorm);
	    	}
	    	else if(strcmp(tempString.c_str(), "f") == 0)
	    	{
	    		unsigned int tempInt = 0;

	    		//if faces are: f # # #
	    		if(uvFlag == 0 && normFlag == 0)
	    		{
					objectFile >> tempInt;
					Indices.push_back(tempInt);
					//std::cout << tempInt << " ";
					objectFile >> tempInt;
					Indices.push_back(tempInt);
					//std::cout << tempInt << " ";
					objectFile >> tempInt;
					Indices.push_back(tempInt);
					//std::cout << tempInt << " ";
	    		}

	    		//if faces are: f #/#
	    		if(uvFlag == 1 && normFlag == 0)
	    		{
	    			for(int i = 0; i < 3; i++)
	    			{
	    				objectFile >> tempInt;
	    				Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    				objectFile.get();
	    				objectFile >> tempInt;
	    				Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    			}
	    			//std::cout << std::endl;
	    		}

	    		//if faces are: #/#/#
	    		if(uvFlag == 1 && normFlag == 1)
	    		{
	    			for(int i = 0; i < 3; i++)
	    			{
	    				objectFile >> tempInt;
	    				Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    				objectFile.get();
	    				objectFile >> tempInt;
	    				Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    				objectFile.get();
	    				objectFile >> tempInt;
	    				Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    			}
	    			//std::cout << std::endl;
	    		}

	    		//if faces are: #//#
	    		if(uvFlag == 0 && normFlag == 1)
	    		{
	    			for(int i = 0; i < 3; i++)
	    			{
	    				objectFile >> tempInt;
	    				Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    				objectFile.get();
	    				objectFile.get();
	    				objectFile >> tempInt;
	    				//Indices.push_back(tempInt);
	    				//std::cout << tempInt << " ";
	    			}
	    			//std::cout << std::endl;
	    		}
	    	}

	    	//std::cout << colorVec.x << ' ' << colorVec.y << ' ' << colorVec.z << std::endl;

	    	//The equations below are for the color of the object, it scales with how many verticies there are to color.
	    	colorVec.x = std::fmod((colorVec.x + (1.0f/(numberOfVs * 1.5))), 1.0f);
	    	colorVec.y = std::fmod((colorVec.y + (1.0f/(numberOfVs * 1.5))), 1.0f);
	    	colorVec.z = std::fmod((colorVec.z + (1.0f/(numberOfVs * 1.5))), 1.0f);
		}
	}
	else
	{
		std::printf("Error, could not open obj file!\nMake sure the obj file is correct in the config file!\n");
		exit(EXIT_FAILURE);
	}
	objectFile.close();
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