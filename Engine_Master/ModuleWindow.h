#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	void ModuleWindow::ResizeWindow(const unsigned int width, const unsigned int height);

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	int width;
	int height;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
};

#endif __ModuleWindow_H__