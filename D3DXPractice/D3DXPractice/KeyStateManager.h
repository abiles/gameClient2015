#pragma once

#define 	VK_A 	0x41 
#define 	VK_B 	0x42 
#define 	VK_C 	0x43 
#define 	VK_D 	0x44 
#define 	VK_E 	0x45 
#define 	VK_F 	0x46 
#define 	VK_G 	0x47 
#define 	VK_H 	0x48 
#define 	VK_I 	0x49 
#define 	VK_J 	0x4A 
#define 	VK_K 	0x4B 
#define 	VK_L 	0x4C 
#define 	VK_M 	0x4D 
#define 	VK_N 	0x4E 
#define 	VK_O 	0x4F 
#define 	VK_P 	0x50 
#define 	VK_Q 	0x51 
#define 	VK_R 	0x52 
#define 	VK_S 	0x53 
#define 	VK_T 	0x54 
#define 	VK_U 	0x55 
#define 	VK_V 	0x56 
#define 	VK_W 	0x57
#define 	VK_X 	0x58 
#define 	VK_Y 	0x59 
#define 	VK_Z 	0x5A

#define FREEKEY	0x10	// ������ ���� ����.
#define PULLKEY	0x20	// ������ �������
#define PUSHKEY	0x40	// ������ ����
#define HOLDKEY	0x80	// ������ �ִ� ��

class KeyStateManager
{
public:
	KeyStateManager();
	~KeyStateManager();


	BYTE* GetOldKey();
	BYTE* GetCurrentKey();

	void KeyInput();

	void  SetMouseX(float mouseX) { m_MouseX = mouseX; };
	float GetMouseX()const { return m_MouseX; };

	void  SetMouseY(float mouseY) { m_MouseY = mouseY; };
	float GetMouseY()const { return m_MouseY; };

	float GetOldMouseX() const { return m_OldMouseX; }
	void  SetOldMouseX() { m_OldMouseX = m_MouseX; }

	float GetOldMouseY() const { return m_OldMouseY; }
	void  SetOldMouseY() { m_OldMouseY = m_MouseY; }

	void SetLButtonDown() { LButtonClick = true; };
	void SetLButtonUp() { LButtonClick = false; };
	bool IsLButtonClick() const { return LButtonClick; };

	void SetRButtonDown() { RButtonClick = true; };
	void SetRButtonUp() { RButtonClick = false; };
	bool IsRButtonClick() const { return RButtonClick; };

	

private:
	BYTE m_ByOldKey[256];
	BYTE m_ByKey[256];

	float m_OldMouseX = 0.0f;
	float m_OldMouseY = 0.0f;
	
	float m_MouseX = 0.0f;
	float m_MouseY = 0.0f;
	
	bool LButtonClick = false;
	bool RButtonClick = false;
	
};

