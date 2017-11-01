#include "graphics.h"

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

  //The ground plane
  ground_1 = new Object("plane", objTriMesh);
  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
  m_physics->addGroundPlane(groundShape);

  // Create the objects
  planet_1 = new Object("earth", objTriMesh);
  //btCollisionShape *sphereShape = new btBvhTriangleMeshShape(objTriMesh, true);
  btCollisionShape *sphereShape = new btSphereShape(1);  
  btDefaultMotionState* ballMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));
  m_physics->addObject(sphereShape, ballMotionState, 1);

  //Second Object
  moon_1 = new Object("earth", objTriMesh);
  btDefaultMotionState* secondBallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(1, 15, 0)));
  m_physics->addObject(sphereShape, secondBallMotionState, 1);

  //Add the Cube
  cube_1 = new Object("cube", objTriMesh);
  btCollisionShape *cubeShape = new btBoxShape(btVector3(1, 1, 1));
  btDefaultMotionState *cubeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-1, 1, 0)));
  m_physics->addObject(cubeShape, cubeMotionState, 1);

  //Add the cylinder
  cylinder_1 = new Object("cylinder", objTriMesh);
  btCollisionShape *cylinderShape = new btCylinderShape(btVector3(1, 1, 1));
  btDefaultMotionState *cylinderMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(5, 1, 0)));
  m_physics->addObject(cylinderShape, cylinderMotionState, 0);

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
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
  
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

////////////////////////////////////////////////////////////////////////////////
  // Locate the projection matrix in the shader
  /*
  p_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (p_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("p_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  p_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (p_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("p_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  p_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (p_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("p_modelMatrix not found\n");
    return false;
  }
  */
//////////////////////////////////////////////////////////////////////////////////

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

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

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