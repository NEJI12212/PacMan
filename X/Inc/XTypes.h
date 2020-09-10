//====================================================================================================
// Filename:	XTypes.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_TYPES_H
#define INCLUDED_XENGINE_TYPES_H

namespace X {

using SoundId = std::size_t;
using TextureId = std::size_t;

enum class Pivot
{
	TopLeft,
	Top,
	TopRight,
	Left,
	Center,
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};

enum class Flip
{
	None,
	Horizontal,
	Vertical,
	Both
};

namespace Keys {

// Keyboard roll 1
const int ESCAPE		= VK_ESCAPE;
const int F1			= VK_F1;
const int F2			= VK_F2;
const int F3			= VK_F3;
const int F4			= VK_F4;
const int F5			= VK_F5;
const int F6			= VK_F6;
const int F7			= VK_F7;
const int F8			= VK_F8;
const int F9			= VK_F9;
const int F10			= VK_F10;
const int F11			= VK_F11;
const int F12			= VK_F12;

// Keyboard roll 2
const int GRAVE			= VK_OEM_3;
const int ONE			= '1';
const int TWO			= '2';
const int THREE			= '3';
const int FOUR			= '4';
const int FIVE			= '5';
const int SIX			= '6';
const int SEVEN			= '7';
const int EIGHT			= '8';
const int NINE			= '9';
const int ZERO			= '0';
const int MINUS			= VK_OEM_MINUS;
const int EQUALS		= VK_OEM_PLUS;
const int BACKSPACE		= VK_BACK;

// Keyboard roll 3
const int TAB			= VK_TAB;
const int Q				= 'Q';
const int W				= 'W';
const int E				= 'E';
const int R				= 'R';
const int T				= 'T';
const int Y				= 'Y';
const int U				= 'U';
const int I				= 'I';
const int O				= 'O';
const int P				= 'P';
const int LBRACKET		= VK_OEM_4;
const int RBRACKET		= VK_OEM_6;
const int BACKSLASH		= VK_OEM_5;

// Keyboard roll 4
const int A				= 'A';
const int S				= 'S';
const int D				= 'D';
const int F				= 'F';
const int G				= 'G';
const int H				= 'H';
const int J				= 'J';
const int K				= 'K';
const int L				= 'L';
const int SEMICOLON		= VK_OEM_1;
const int APOSTROPHE	= VK_OEM_7;
const int ENTER			= VK_RETURN;

// Keyboard roll 5
const int Z				= 'Z';
const int X				= 'X';
const int C				= 'C';
const int V				= 'V';
const int B				= 'B';
const int N				= 'N';
const int M				= 'M';
const int COMMA			= VK_OEM_COMMA;
const int PERIOD		= VK_OEM_PERIOD;
const int SLASH			= VK_OEM_2;

// Lock keys
const int CAPSLOCK		= VK_CAPITAL;
const int NUMLOCK		= VK_NUMLOCK;
const int SCROLLLOCK	= VK_SCROLL;

// Numpad
const int NUMPAD1		= VK_NUMPAD1;
const int NUMPAD2		= VK_NUMPAD2;
const int NUMPAD3		= VK_NUMPAD3;
const int NUMPAD4		= VK_NUMPAD4;
const int NUMPAD5		= VK_NUMPAD5;
const int NUMPAD6		= VK_NUMPAD6;
const int NUMPAD7		= VK_NUMPAD7;
const int NUMPAD8		= VK_NUMPAD8;
const int NUMPAD9		= VK_NUMPAD9;
const int NUMPAD0		= VK_NUMPAD0;
const int NUM_ADD		= VK_ADD;
const int NUM_SUB		= VK_SUBTRACT;
const int NUM_MUL		= VK_MULTIPLY;
const int NUM_DIV		= VK_DIVIDE;
const int NUM_ENTER		= VK_RETURN;
const int NUM_DECIMAL	= VK_DECIMAL;

// Navigation keys
const int INS			= VK_INSERT;
const int DEL			= VK_DELETE;
const int HOME			= VK_HOME;
const int END			= VK_END;
const int PGUP			= VK_PRIOR;
const int PGDN			= VK_NEXT;

// Support keys
const int LSHIFT		= VK_SHIFT;
const int RSHIFT		= VK_SHIFT;
const int LCONTROL		= VK_CONTROL;
const int RCONTROL		= VK_CONTROL;
const int LALT			= VK_MENU;
const int RALT			= VK_MENU;
const int LWIN			= VK_LWIN;
const int RWIN			= VK_RWIN;
const int SPACE			= VK_SPACE;

// Arrow keys
const int UP			= VK_UP;
const int DOWN			= VK_DOWN;
const int LEFT			= VK_LEFT;
const int RIGHT			= VK_RIGHT;

} // namespace Keys

namespace Mouse {

const int LBUTTON		= 0;
const int RBUTTON		= 1;
const int MBUTTON		= 2;

} // namespace Mouse

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_TYPES_H