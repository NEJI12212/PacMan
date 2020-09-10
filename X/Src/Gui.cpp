//====================================================================================================
// Filename:	Gui.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "Gui.h"

#include "GraphicsSystem.h"
#include <ImGui/Inc/imgui.h>
#include <ImGui/Inc/imgui_impl_dx11.h>
#include <ImGui/Inc/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace X;

namespace
{
	HWND sWindow;
	WindowMessageHandler sPreviousWndProc = 0;

	bool IsMouseInput(UINT message)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			return true;
		}
		return false;
	}

	bool IsKeyboardInput(UINT message)
	{
		switch (message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_CHAR:
			return true;
		}
		return false;
	}
}

LRESULT CALLBACK GuiMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();

	// When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	if (io.WantCaptureMouse && IsMouseInput(message))
		return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);

	// When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	if (io.WantCaptureKeyboard && IsKeyboardInput(message))
		return ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);

	LRESULT result = ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam);
	if (result != 0)
		return result;

	return sPreviousWndProc(window, message, wParam, lParam);
}

void Gui::Initialize(HWND window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto device = GraphicsSystem::Get()->GetDevice();
	auto context = GraphicsSystem::Get()->GetContext();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(device, context);
	ImGui::StyleColorsDark();
	
	// Hook application to window's procedure
	sWindow = window;
	sPreviousWndProc = (WindowMessageHandler)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)GuiMessageHandler);
}

void Gui::Terminate()
{
	// Restore original window's procedure
	SetWindowLongPtrA(sWindow, GWLP_WNDPROC, (LONG_PTR)sPreviousWndProc);
	sWindow = nullptr;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Gui::BeginRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Gui::EndRender()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}