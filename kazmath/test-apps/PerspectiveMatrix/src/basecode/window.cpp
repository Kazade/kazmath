#include "window.h"

namespace NeHe
{
    Window::Window():
            m_Width(0),
            m_Height(0),
            m_Bpp(0),
            m_Fullscreen(false),
            m_Time(0)
    {

    }

    Window::~Window()
    {
        SDL_Quit();
    }

    bool Window::createWindow(int width, int height, int bpp, bool fullscreen, const string& title)
    {
        if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
        {
            return false;
        }

        //Copy the values incase we need them
        m_Height = height;
        m_Width = width;
        m_Title = title;
        m_Fullscreen = fullscreen;
        m_Bpp = bpp;

        //all values are "at least"!
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Set the title.
        SDL_WM_SetCaption(title.c_str(), title.c_str());

        // Flags tell SDL about the type of window we are creating.
        int flags = SDL_OPENGL;

        if (fullscreen == true)
        {
            flags |= SDL_FULLSCREEN;
        }

        // Create the window
        SDL_Surface * screen = SDL_SetVideoMode( width, height, bpp, flags );

        if (screen == 0)
        {
            return false;
        }

        //SDL doesn't trigger off a ResizeEvent at startup, but as we need this for OpenGL, we do this ourself
        SDL_Event resizeEvent;
        resizeEvent.type = SDL_VIDEORESIZE;
        resizeEvent.resize.w = width;
        resizeEvent.resize.h = height;

        SDL_PushEvent(&resizeEvent);

        return true;
    }

    void Window::setSize(int width, int height)
    {
        m_Height = height;
        m_Width = width;
    }

    int Window::getHeight()
    {
        return m_Height;
    }

    int Window::getWidth()
    {
        return m_Width;
    }

    unsigned int Window::getElapsedMillisecs()
    {
        //SDL_GetTicks returns the time in milliseconds from the programs startup
        unsigned int now = SDL_GetTicks();
        unsigned int delta = now - m_Time;
        m_Time = now;

        return delta;
    }
};
