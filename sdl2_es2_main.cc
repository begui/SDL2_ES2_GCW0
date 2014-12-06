#include <SDL2/SDL.h>
#include "rotate_triangle.h"

#ifdef USE_GCW0
static const int WIDTH = 320;
static const int HEIGHT = 240;
#else
static const int WIDTH = 800;
static const int HEIGHT = 600;
#endif

SDL_Window *gscreen = nullptr;
SDL_GLContext gcontext;

SDL_Window *init_window(Uint32 subsystems, Uint32 windowFlags)
{
	SDL_Window *window = nullptr;
	if ( SDL_Init(subsystems) >=0 )
	{
		//
		// Set core profile
		//
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		// Enables double buffering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		// 24 bit depth buffer  
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		window = SDL_CreateWindow(	"An SDL2 window", 
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				WIDTH, HEIGHT, 
				windowFlags );
		if(window) 
		{
			// Create OpenGL Context
			gcontext = SDL_GL_CreateContext(window);
			if(nullptr == gcontext)
			{
				std::cerr << "Failed  to create gl context.\nError: " <<  SDL_GetError() << std::endl;
				SDL_DestroyWindow(window);
				window = nullptr;
			}
			// enable vsync
			SDL_GL_SetSwapInterval(1);
		}
	}
	else 
	{
		std::cerr << "Failed to init sdl: " << SDL_GetError() << std::endl;
		exit(1);
	}
	return window;
}

inline bool inputHandler()
{
	SDL_Event event;
	bool rtn = true;
	while(SDL_PollEvent(&event))
	{
		if (event.type==SDL_QUIT) {return false;}
		if (event.type==SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym) 
			{
				case SDLK_RETURN:
					std::cout << "Start button pressed" << std::endl;
					break;
				case SDLK_ESCAPE:
					std::cout << "Esc Key pressed" << std::endl;
					rtn = false;
					break;
				case SDLK_LEFT:  
					std::cout << "Left Key pressed" << std::endl;
					break;
				case SDLK_RIGHT: 
					std::cout << "Right Key pressed" << std::endl;
					break;
				case SDLK_UP:    
					std::cout << "Up Key pressed" << std::endl;
					break;
				case SDLK_DOWN:  
					std::cout << "Down Key pressed" << std::endl;
					break;
				case SDLK_LCTRL:
					std::cout << "A Key pressed" << std::endl;
					break;
				case SDLK_LALT:
					std::cout << "B Key pressed" << std::endl;
					break;
				case SDLK_LSHIFT:
					std::cout << "X Key pressed" << std::endl;
					break;
				case SDLK_SPACE:
					std::cout << "Y Key pressed" << std::endl;
					break;
				default: 
					std::cout << "Unknown Key pressed " << event.key.keysym.sym << std::endl;
			}
		}
	}
	return rtn;
}

void cleanup()
{
	SDL_GL_DeleteContext(gcontext);
	SDL_DestroyWindow(gscreen);
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	Uint32 subsystem = 0; 
	Uint32 windowFlags = 0;
	subsystem |= SDL_INIT_VIDEO | SDL_INIT_TIMER;
	windowFlags |= SDL_WINDOW_OPENGL;

	gscreen = init_window(subsystem, windowFlags); 
	if (!gscreen) 
	{
		std::cerr << "Could not create window " <<  SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	printGLVersion();

	RotateTriangle tri;
	bool running=true;
	glViewport(0,0,WIDTH,HEIGHT);
	while(running)
	{
		glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear(GL_COLOR_BUFFER_BIT);
		tri.render();
		SDL_GL_SwapWindow(gscreen);
		running = inputHandler();

	}
	cleanup();
	return 0;
}
