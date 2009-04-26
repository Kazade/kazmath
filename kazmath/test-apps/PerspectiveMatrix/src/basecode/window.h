#ifndef _NEHE_WINDOW
#define _NEHE_WINDOW

#include <string>
#include <iostream>

#include "GLee/GLee.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>

using std::string;

namespace NeHe
{
	class Window
	{
		private:
			int m_Width;
			int m_Height;
			int m_Bpp;
			bool m_Fullscreen;
			string m_Title;
			unsigned int m_Time;
			
		public:
			Window();
			~Window();
			
			bool createWindow(int width, int height, int bpp, bool fullscreen, const string& title);
			void setSize(int width, int height);
			int getHeight();
			int getWidth();
			unsigned int getElapsedMillisecs();
	};
};

#endif

