#include "pch.h"
#include "KeyStateManager.h"
#include "Camera.h"


KeyStateManager::KeyStateManager()
{
}


KeyStateManager::~KeyStateManager()
{
}

void KeyStateManager::KeyInput()
{

	if (GetKeyboardState(m_ByKey))
	{
		// DOWN ¹× UP°è»ê
		for (int i = 0; i < 256; i++)
		{
			if (m_ByKey[i] & HOLDKEY)
			{
				if (!m_ByOldKey[i])
				{
					m_ByOldKey[i] = 1;
					m_ByKey[i] |= PUSHKEY;
				}
			}
			else
			{
				if (m_ByOldKey[i])
				{
					m_ByOldKey[i] = 0;
					m_ByKey[i] = PULLKEY;
				}
			}
		}
	}

	
}



BYTE* KeyStateManager::GetOldKey()
{
	return m_ByOldKey;
}

BYTE* KeyStateManager::GetCurrentKey()
{
	return m_ByKey;
}
