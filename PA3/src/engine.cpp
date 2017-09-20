
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

    // Update and render the graphics
    m_graphics->Update(m_DT, p_tempRotation, p_tempOrbit, m_tempRotation, m_tempOrbit);
    m_graphics->Render();

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

    //Q and left arrow, orbits the planet counterclockwise
    else if(m_event.key.keysym.scancode == 20 || m_event.key.keysym.scancode == 80)
    {
      p_orbitDir = 1;
    }

    //W and right arrow, orbits the planet clockwise
    else if(m_event.key.keysym.scancode == 26 || m_event.key.keysym.scancode == 79)
    {
      p_orbitDir = -1;
    }

    //E, stops the orbit of the planet
    else if(m_event.key.keysym.scancode == 8)
    {
      p_orbitDir = 0;
    }

    //I and up arrow, rotates the planet counterclockwise
    else if(m_event.key.keysym.scancode == 12 || m_event.key.keysym.scancode == 82)
    {
      p_rotationDir = 1;
    }

    //O and down arrow, rotates the planet clockwise
    else if(m_event.key.keysym.scancode == 18 || m_event.key.keysym.scancode == 81)
    {
      p_rotationDir = -1;
    }

    //P, stops the rotation of the planet
    else if(m_event.key.keysym.scancode == 19)
    {
      p_rotationDir = 0;
    }

    //A, orbits the moon counterclockwise
    else if(m_event.key.keysym.scancode == 4)
    {
      m_orbitDir = 1;
    }

    //S, orbits the moon clockwise
    else if(m_event.key.keysym.scancode == 22)
    {
      m_orbitDir = -1;
    }

    //D, stops the orbit of the moon
    else if(m_event.key.keysym.scancode == 7)
    {
      m_orbitDir = 0;
    }

    //J, rotates the moon counterclockwise
    else if(m_event.key.keysym.scancode == 13)
    {
      m_rotationDir = 1;
    }

    //K, rotates the moon clockwise
    else if(m_event.key.keysym.scancode == 14)
    {
      m_rotationDir = -1;
    }

    //L, stops the rotation of the moon
    else if(m_event.key.keysym.scancode == 15)
    {
      m_rotationDir = 0;
    }

    //Space, stops all rotations and orbits
    else if(m_event.key.keysym.scancode == 44)
    {
      p_orbitDir = 0;
      p_rotationDir = 0;
      m_orbitDir = 0;
      m_rotationDir = 0;
    }
    //std::cout << m_event.key.keysym.scancode << std::endl;
  }
  else if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
  	//for left mouse button, switches both rotation and orbit direction
  	if (m_event.button.button == SDL_BUTTON_LEFT)
  	{
  		p_orbitDir *= -1;
  		p_rotationDir *= -1;
      m_orbitDir *= -1;
      m_rotationDir *= -1;
  	}
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
