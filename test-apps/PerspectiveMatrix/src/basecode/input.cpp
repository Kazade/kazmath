#include "input.h"

SDLInputSystem::SDLInputSystem() 
{
	memset(&m_LastKeyState, 0, sizeof(unsigned char) * SDLK_LAST);
	memset(&m_CurrentKeyState, 0, sizeof(unsigned char) * SDLK_LAST);
}

SDLInputSystem::~SDLInputSystem() 
{

}

void SDLInputSystem::update()
{
	//Copy the current state to the last state
	memcpy(&m_LastKeyState, &m_CurrentKeyState, sizeof(unsigned char) * SDLK_LAST);
	
	//Get a pointer to SDLs internal key state
	unsigned char* SDLInternalState = SDL_GetKeyState(NULL);

	//Copy SDLs internal keystate to our current array
	memcpy(&m_CurrentKeyState, SDLInternalState, sizeof(unsigned char) * SDLK_LAST);
}
