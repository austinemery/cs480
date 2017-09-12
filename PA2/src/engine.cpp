
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

  // No errors
  return true;
}

void Engine::Run()
{
  float tempRotation = 0.0f, tempOrbit = 0.0f;

  m_running = true;

  while(m_running)
  {
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard(tempRotation, tempOrbit);
    }

    // Update the DT
    m_DT = getDT();

    // Update and render the graphics
    m_graphics->Update(m_DT, tempRotation, tempOrbit);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard(float& rotationDir, float& orbitDir)
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
    else if(m_event.key.keysym.scancode == 20)//Q
    {
      orbitDir = 1;
    }
    else if(m_event.key.keysym.scancode == 26)//W
    {
      orbitDir = -1;
    }
    else if(m_event.key.keysym.scancode == 8)//E
    {
      orbitDir = 0;
    }
    else if(m_event.key.keysym.scancode == 12)//I
    {
      rotationDir = 1;
    }
    else if(m_event.key.keysym.scancode == 18)//O
    {
      rotationDir = -1;
    }
    else if(m_event.key.keysym.scancode == 19)//P
    {
      rotationDir = 0;
    }
    else if(m_event.key.keysym.scancode == 44)//Space
    {
      orbitDir = 0;
      rotationDir = 0;
    }
    //std::cout << m_event.key.keysym.scancode << std::endl;
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
  	//for left mouse button, switches bot rotation and orbit direction
  	if (m_event.button.button == SDL_BUTTON_LEFT)
  	{
  		orbitDir *= -1;
  		rotationDir *= -1;
  	}
  }
  std::cout << m_event.key.keysym.scancode << std::endl;
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
