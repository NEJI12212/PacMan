//====================================================================================================
// Filename:	InputSystem.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_INPUTSYSTEM_H
#define INCLUDED_XENGINE_INPUTSYSTEM_H

#include "XTypes.h"
#include <DirectXTK/Inc/GamePad.h>

namespace X {

class InputSystem
{
public:
	static void StaticInitialize(HWND window);
	static void StaticTerminate();
	static InputSystem* Get();

public:
	InputSystem();
	~InputSystem();

	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;

	void Initialize(HWND window);
	void Terminate();

	void Update();

	bool IsKeyDown(uint32_t key) const;
	bool IsKeyPressed(uint32_t key) const;

	bool IsMouseDown(uint32_t button) const;
	bool IsMousePressed(uint32_t button) const;

	int GetMouseMoveX() const;
	int GetMouseMoveY() const;
	float GetMouseMoveZ() const;

	int GetMouseScreenX() const;
	int GetMouseScreenY() const;

	bool IsMouseLeftEdge() const;
	bool IsMouseRightEdge() const;
	bool IsMouseTopEdge() const;
	bool IsMouseBottomEdge() const;

	void ShowSystemCursor(bool show);
	void SetMouseClipToWindow(bool clip);
	bool IsMouseClipToWindow() const;
	
	bool IsGamePadConnected(int player) const;

	bool IsAPressed(int player) const;
	bool IsBPressed(int player) const;
	bool IsXPressed(int player) const;
	bool IsYPressed(int player) const;

	bool IsLeftShoulderPressed(int player) const;
	bool IsLeftTriggerPressed(int player) const;
	bool IsRightShoulderPressed(int player) const;
	bool IsRightTriggerPressed(int player) const;

	bool IsDPadUp(int player) const;
	bool IsDPadDown(int player) const;
	bool IsDPadLeft(int player) const;
	bool IsDPadRight(int player) const;

	bool IsLeftThumbStickUp(int player) const;
	bool IsLeftThumbStickDown(int player) const;
	bool IsLeftThumbStickLeft(int player) const;
	bool IsLeftThumbStickRight(int player) const;

	bool IsRightThumbStickUp(int player) const;
	bool IsRightThumbStickDown(int player) const;
	bool IsRightThumbStickLeft(int player) const;
	bool IsRightThumbStickRight(int player) const;

	float GetLeftAnalogX(int player) const;
	float GetLeftAnalogY(int player) const;
	float GetRightAnalogX(int player) const;
	float GetRightAnalogY(int player) const;

private:
	friend LRESULT CALLBACK InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	HWND mWindow;

	std::unique_ptr<DirectX::GamePad> mGamePad;
	DirectX::GamePad::State mGamePadState[4];

	bool mCurrKeys[512];
	bool mPrevKeys[512];
	bool mPressedKeys[512];

	bool mClipMouseToWindow;

	int mCurrMouseX;
	int mCurrMouseY;
	int mPrevMouseX;
	int mPrevMouseY;
	int mMouseMoveX;
	int mMouseMoveY;

	float mMouseWheel;

	bool mCurrMouseButtons[3];
	bool mPrevMouseButtons[3];
	bool mPressedMouseButtons[3];

	bool mMouseLeftEdge;
	bool mMouseRightEdge;
	bool mMouseTopEdge;
	bool mMouseBottomEdge;

	bool mInitialized;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_INPUTSYSTEM_H
