#pragma once
#include"pch.h"
#define KeyPressed(key) GetAsyncKeyState(key) & 0x8000
typedef void(*TKeyboardFn)(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
struct keyStates {
	DWORD time{};
	BOOL isWithAlt{};
	BOOL wasDownBefore{};
	BOOL isUpNow{};
};
void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);

class keyboard
{
public:
	keyboard();
	~keyboard();

	bool KeyDown(DWORD key);
	bool KeyJustUp(DWORD key, bool exclusive = true);
	bool KeyPressedOnce(bool& bIsPressed, DWORD vk);
	void ResetKeyState(DWORD key);

	void keyboardHandlerRegister(TKeyboardFn function);
	void keyboardHandlerUnregister(TKeyboardFn function);
	void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
	std::set<TKeyboardFn> g_keyboardFunctions;
	keyStates m_keys[255];
};
inline keyboard* g_keyBoard;
DWORD	strToVk(std::string str);
