#include "graphics.h"

using namespace std;

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  //For physics
  m_physics = new Physics();
  if(!m_physics->Initialize())
  {
    printf("Physics Failed to Initialize\n");
    return false;
  }

  btTriangleMesh* objTriMesh = new btTriangleMesh();

  // Create the objects
  objTriMesh = new btTriangleMesh();

  planet_1 = new Object("earth", objTriMesh);
  btCollisionShape *sphereShape = new btConvexTriangleMeshShape(objTriMesh, true); 
  btDefaultMotionState* ballMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(3, 10, 0)));
  m_physics->addObject(sphereShape, ballMotionState, 1);

  //Second Object
  objTriMesh = new btTriangleMesh();

  moon_1 = new Object("earth", objTriMesh);
  btDefaultMotionState* secondBallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-4.5, 25, 0)));
  m_physics->addObject(sphereShape, secondBallMotionState, 1);

  //cout << objTriMesh[0] <<

  //Add the Cube
  objTriMesh = new btTriangleMesh();

  cube_1 = new Object("cube", objTriMesh);
  btCollisionShape *cubeShape = new btConvexTriangleMeshShape(objTriMesh, true);
  btDefaultMotionState *cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-5, 10, -5)));
  m_physics->addObject(cubeShape, cubeMotionState, 1);

  //Add the cylinder
  objTriMesh = new btTriangleMesh();

  cylinder_1 = new Object("cylinder", objTriMesh);
  btCollisionShape *cylinderShape = new btBvhTriangleMeshShape(objTriMesh, true);
  btDefaultMotionState *cylinderMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(5, 1, 0)));
  m_physics->addObject(cylinderShape, cylinderMotionState, 0);

  //The Table
  objTriMesh = new btTriangleMesh();
  
  ground_1 = new Object("table", objTriMesh);
  btCollisionShape *tableShape = new btBvhTriangleMeshShape(objTriMesh, true);
  btDefaultMotionState* tableMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
  m_physics->addObject(tableShape, tableMotionState, 0);

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, "normVShader.vert"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, "normFShader.frag"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  if(!m_shader->AddShader(GL_VERTEX_SHADER, "testShaderV.vert"))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, "testShaderF.frag"))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix", 0);
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix", 0);
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix", 0);
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt)
{
  glm::mat4 temp;

  //Update the physics

  //update the spheres
  temp = m_physics->Update(dt, 0);
  planet_1->Update(dt, temp);
  
  temp = m_physics->Update(dt, 1);
  moon_1->Update(dt, temp);

  //update the cube
  temp = m_physics->Update(dt, 2);
  cube_1->Update(dt, temp);

  //update the cylinder
  temp = m_physics->Update(dt, 3);
  cylinder_1->Update(dt, temp);
}

void Graphics::Render(int shaderSelector)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable(shaderSelector);

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

/////////////////////////////////////////////////////////////////////////

  // Render the objects
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ground_1->GetModel()));
  ground_1->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(planet_1->GetModel()));
  planet_1->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(moon_1->GetModel()));
  moon_1->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cube_1->GetModel()));
  cube_1->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cylinder_1->GetModel()));
  cylinder_1->Render();

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

Camera* Graphics::getCamera()
{
  return m_camera;
}

Physics* Graphics::getPhysics()
{
  return m_physics;
}

bool Graphics::shaderSwap(int shaderSelector)
{
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix", shaderSelector);
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix", shaderSelector);
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix", shaderSelector);
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  return true;
}