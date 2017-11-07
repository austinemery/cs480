#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  velocityX = 0.0f;
  velocityZ = 0.0f;
  rotation = 0.0f;
  zoom = 1.0f;

  shaderSelector = 0;

  // No errors
  return true;
}

void Engine::Run()
{
  float p_tempRotation = 0.0f, p_tempOrbit = 0.0f, m_tempRotation = 0.0f, m_tempOrbit = 0.0f;

  m_running = true;

  while(m_running)
  {
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard(p_tempRotation, p_tempOrbit, m_tempRotation, m_tempOrbit);
    }

    // Update the DT
    m_DT = getDT();

    //m_window->RunMenu();

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render(shaderSelector);

    m_window->RunMenu();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard(float& p_rotationDir, float& p_orbitDir, float& m_rotationDir, float& m_orbitDir)
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if(m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if(m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }

    //W A S D movements, change the velocity of the cube
    else if (m_event.key.keysym.scancode == SDL_SCANCODE_A || m_event.key.keysym.scancode == SDL_SCANCODE_D)
    {      
      float velocityScale;

      //std::cout << velocityX << std::endl;
      if (m_event.key.keysym.scancode == SDL_SCANCODE_A)
      {
        velocityScale = 1;
        velocityX = 3;
      }
      else 
      {
        velocityScale = -1;
        velocityX = -3;
      }
      
      //velocityX += velocityScale;

      if (velocityX > 10)
      {
        velocityX = 10.0f;
      }
      if (velocityX < -10)
      {
        velocityX = -10.0f;
      }
      m_graphics->getPhysics()->updateCubeVelocity(velocityX, velocityZ);
      velocityX = 0;
    }
    else if (m_event.key.keysym.scancode == SDL_SCANCODE_W || m_event.key.keysym.scancode == SDL_SCANCODE_S)
    {
      float velocityScale;

      //std::cout << velocityZ << std::endl;
      if (m_event.key.keysym.scancode == SDL_SCANCODE_W)
      {
        velocityScale = 1;
        velocityZ = 3;
      }
      else 
      {
        velocityScale = -1;
        velocityZ = -3;
      }
      
      //velocityZ += velocityScale;

      if (velocityZ > 10)
      {
        velocityZ = 10.0f;
      }
      if (velocityZ < -10)
      {
        velocityZ = -10.0f;
      }
      m_graphics->getPhysics()->updateCubeVelocity(velocityX, velocityZ);
      velocityZ = 0;
    }

    //left arrow and right arrow, rotates the camera to the left and to the right
    else if(m_event.key.keysym.scancode == 80 || m_event.key.keysym.scancode == 79)
    {
      float rotationScale = 360.0f / m_WINDOW_WIDTH;

      if (m_event.key.keysym.scancode == 80) //the left arrow
      {
        rotation += (rotationScale * -6);
      }
      else //the right arrow
      {
        rotation += (rotationScale * 6);
      }
      
      //to get back in 0 - 360 if needed
      while(rotation > 360)
      {
        rotation -= 360.0f;
      }
      while(rotation < 0)
      {
        rotation += 360.0f;
      }

      m_graphics->getCamera()->Update(rotation, zoom);
    }

    //up arrow and down arrow, zooms in and out
    else if(m_event.key.keysym.scancode == 82 || m_event.key.keysym.scancode == 81)
    {
      float zoomScale = 0.1f * zoom;

      //zoom in
      if(m_event.key.keysym.scancode == 82) // up arrow
      {
        zoom -= zoomScale;
      }
      else //down arrow
      {
        zoom += zoomScale;
      }

      //to lock distances
      if (zoom > 2)
      {
        zoom = 2;
      }
      if (zoom < 0)
      {
        zoom = 0;
      }

      m_graphics->getCamera()->Update(rotation, zoom);
    }

    //space
    else if(m_event.key.keysym.scancode == 44)
    {
      
    }
  }

  //For swaping shader programs
  if (m_event.key.keysym.scancode == SDL_SCANCODE_V)
  {
  	std::cout << "Vertex Lighting Mode" << std::endl;
  	shaderSelector = 0;
  	m_graphics->shaderSwap(shaderSelector);
  }
  if (m_event.key.keysym.scancode == SDL_SCANCODE_F)
  {
  	std::cout << "Fragment Lighting Mode" << std::endl;
  	shaderSelector = 1;
  	m_graphics->shaderSwap(shaderSelector);
  }

  //For rotating around a point, right mouse button and move the mouse
  if (m_event.type == SDL_MOUSEMOTION && m_event.button.button == SDL_BUTTON(SDL_BUTTON_RIGHT))
  {
      float rotationScale = 360.0f / m_WINDOW_WIDTH;

      rotation += (rotationScale * m_event.motion.xrel);

      //to get back in 0 - 360 if needed
      while(rotation > 360)
      {
        rotation -= 360.0f;
      }
      while(rotation < 0)
      {
        rotation += 360.0f;
      }

      m_graphics->getCamera()->Update(rotation, zoom);
  }

  //For zooming at a point, the mouse scroll wheel
  if (m_event.type == SDL_MOUSEWHEEL)
  {
      //zooming speed
      float zoomScale = 0.1f * zoom;

      //zoom in
      if(m_event.wheel.y == 1) // scroll up
      {
        //std::cout << "SCROLLING UP" << std::endl;
        zoom -= zoomScale;
      }
      //zoom out
      else if(m_event.wheel.y == -1) // scroll down
      {
        //std::cout << "SCROLLING DOWN" << std::endl;
        zoom += zoomScale;
      }

      //to lock distances
      if (zoom > 2)
      {
        zoom = 2;
      }
      if (zoom < 0)
      {
        zoom = 0;
      }

      m_graphics->getCamera()->Update(rotation, zoom);
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
