#include <SDL/SDL.h>

class SDLInputSystem 
{
	public:
		SDLInputSystem();
		~SDLInputSystem();

		void update();

		inline bool isKeyDown(SDLKey k) 
		{
			return (!m_LastKeyState[k] && m_CurrentKeyState[k]);
		}

		inline bool isKeyUp(SDLKey k)
		{
			return (m_LastKeyState[k] && !m_CurrentKeyState[k]);
		}

		inline bool isKeyStillDown(SDLKey k) 
		{
			return (m_LastKeyState[k] && m_CurrentKeyState[k]);
		}

		inline bool isKeyStillUp(SDLKey k)
		{
			return (!m_LastKeyState[k] && !m_CurrentKeyState[k]);
		}
	
	private:
		unsigned char m_LastKeyState[SDLK_LAST];
		unsigned char m_CurrentKeyState[SDLK_LAST];
};
