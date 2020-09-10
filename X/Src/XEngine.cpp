//====================================================================================================
// Filename:	XEngine.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "XEngine.h"

#include "AudioSystem.h"
#include "Config.h"
#include "Camera.h"
#include "Font.h"
#include "GraphicsSystem.h"
#include "Gui.h"
#include "InputSystem.h"
#include "SimpleDraw.h"
#include "SoundEffectManager.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Timer.h"

#pragma comment(lib, "FW1FontWrapper.lib")

using namespace X;

namespace
{
	struct SpriteCommand
	{
		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Vector2& inPosiiton, float inRotation, Pivot pivot, Flip flip)
			: textureId(inTextureId)
			, sourceRect({ 0.0f, 0.0f, 0.0f, 0.0f })
			, position(inPosiiton)
			, rotation(inRotation)
			, pivot(pivot)
			, flip(flip)
		{}

		SpriteCommand(TextureId inTextureId, const Math::Rect& inSourceRect, const Math::Vector2& inPosiiton, float inRotation)
			: textureId(inTextureId)
			, sourceRect(inSourceRect)
			, position(inPosiiton)
			, rotation(inRotation)
		{}

		TextureId textureId = 0;
		Math::Rect sourceRect{};
		Math::Vector2 position{ 0.0f };
		float rotation{ 0.0f };
		Pivot pivot = Pivot::Center;
		Flip flip = Flip::None;
	};

	struct TextCommand
	{
		TextCommand(std::wstring inStr, float inSize, float inX, float inY, uint32_t inColor)
			: str(std::move(inStr))
			, size(inSize)
			, x(inX)
			, y(inY)
			, color(inColor)
		{}

		std::wstring str;
		float size, x, y;
		uint32_t color;
	};
	
	HWND myWindow = nullptr;
	bool initialized = false;

	std::random_device myRandomDevice{};
	std::mt19937 myRandomEngine{ myRandomDevice() };

	Color myBackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	Camera myCamera;
	Font myFont;
	Timer myTimer;
	float myZoom = 1.0f;

	std::vector<SpriteCommand> mySpriteCommands;
	std::vector<TextCommand> myTextCommands;

	inline uint32_t ToColor(const Color& color)
	{
		uint8_t r = (uint8_t)(color.r * 255);
		uint8_t g = (uint8_t)(color.g * 255);
		uint8_t b = (uint8_t)(color.b * 255);
		return 0xff000000 | (b << 16) | (g << 8) | r;
	}
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		// TODO: Resize logic
		//if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		//{
		//	ImGui_ImplDX11_InvalidateDeviceObjects();
		//	CleanupRenderTarget();
		//	g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
		//	CreateRenderTarget();
		//	ImGui_ImplDX11_CreateDeviceObjects();
		//}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

namespace X {

void Start(const char* configFileName)
{
	XASSERT(!initialized, "[XEngine] Engine already started.");

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	Config::StaticInitialize(configFileName);
	
	HINSTANCE instance = GetModuleHandleA(nullptr);
	const char* appName = Config::Get()->GetString("AppName", "X");
	const int clientWidth = Config::Get()->GetInt("WinWidth", 1280);
	const int clientHeight = Config::Get()->GetInt("WinHeight", 720);
	
	// Every Windows Window requires at least oen window object. Three things are involved:
	// 1)	Register a window class.
	// 2)	Create a window object.
	// 3)	Retrieve and dispatch messages for this window.

	// Register class
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursorA(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);

	XVERIFY(RegisterClassExA(&wcex), "[Window] Failed to register window class.");

	// Compute the correct window dimension
	RECT rc = { 0, 0, static_cast<LONG>(clientWidth), static_cast<LONG>(clientHeight) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) >> 1;
	const int top = (screenHeight - winHeight) >> 1;

	// Create window
	myWindow = CreateWindowA
	(
		appName,
		appName,
		WS_OVERLAPPEDWINDOW,
		left,
		top,
		winWidth,
		winHeight,
		nullptr,
		nullptr,
		instance,
		nullptr
	);

	XASSERT(myWindow != nullptr, "[XEngine] Failed to create window.");

	ShowWindow(myWindow, true);

	XVERIFY(SetCursorPos(screenWidth >> 1, screenHeight >> 1), "[Window] Failed to set cursor position.");
	
	// Initialize all engine systems
	AudioSystem::StaticInitialize();
	GraphicsSystem::StaticInitialize(myWindow, Config::Get()->GetBool("FullScreen", false));
	InputSystem::StaticInitialize(myWindow);
	SimpleDraw::Initialize(1024 * 1024);
	SpriteRenderer::StaticInitialize();
	SoundEffectManager::StaticInitialize(Config::Get()->GetString("SoundPath", "../Assets/Sounds"));
	TextureManager::StaticInitialize(Config::Get()->GetString("TexturePath", "../Assets/Images"));
	Gui::Initialize(myWindow);

	// Initialize camera
	myCamera.SetFOV(60.0f * Math::kDegToRad);
	myCamera.SetNearPlane(0.01f);
	myCamera.SetFarPlane(10000.0f);

	// Initialize font
	myFont.Initialize();

	// Engine initialized
	initialized = true;
}

//----------------------------------------------------------------------------------------------------

void Run(bool (*GameLoop)(float))
{
	XASSERT(initialized, "[XEngine] Engine not started.");

	myTimer.Initialize();

	// Start the main loop
	bool quit = false;
	while (!quit)
	{
		MSG msg = {};
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				quit = true;
		}

		if (quit)
			break;

		// Update input and timer
		InputSystem::Get()->Update();
		myTimer.Update();

		const float kDeltaTime = myTimer.GetElapsedTime();

		// Update audio
		AudioSystem::Get()->Update();

		// Begin Gui
		Gui::BeginRender();

		// Run game loop
		if (GameLoop(kDeltaTime))
		{
			PostQuitMessage(0);
		}

		// Begin scene
		GraphicsSystem::Get()->BeginRender(myBackgroundColor);

		TextureId id = 0;
		Texture* texture = nullptr;

		// Sprites
		SpriteRenderer::Get()->BeginRender();
		for (const auto& command : mySpriteCommands)
		{
			if (id != command.textureId)
			{
				texture = TextureManager::Get()->GetTexture(command.textureId);
				id = command.textureId;
			}
			if (texture)
			{
				if (Math::IsEmpty(command.sourceRect))
				{
					SpriteRenderer::Get()->Draw(*texture, command.position, command.rotation, command.pivot, command.flip);
				}
				else
				{
					SpriteRenderer::Get()->Draw(*texture, command.sourceRect, command.position, command.rotation, command.pivot, command.flip);
				}
			}
		}
		mySpriteCommands.clear();
		SpriteRenderer::Get()->EndRender();

		// Text
		for (const auto& command : myTextCommands)
		{
			myFont.Draw(command.str.c_str(), command.size, command.x, command.y, command.color);
		}
		myTextCommands.clear();

		// Render
		SimpleDraw::Render(myCamera);
			
		// End Gui
		Gui::EndRender();

		// End scene
		GraphicsSystem::Get()->EndRender();
	}
}

//----------------------------------------------------------------------------------------------------

void Stop()
{
	XASSERT(initialized, "[XEngine] Engine not started.");

	// Destroy font
	myFont.Terminate();

	// Shutdown all engine systems
	Gui::Terminate();
	TextureManager::StaticTerminate();
	SoundEffectManager::StaticTerminate();
	SpriteRenderer::StaticTerminate();
	SimpleDraw::Terminate();
	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	AudioSystem::StaticTerminate();

	// Destroy the window
	DestroyWindow(myWindow);

	// Unregister window class
	HINSTANCE instance = GetModuleHandleA(nullptr);
	const char* appName = Config::Get()->GetString("AppName", "X");
	UnregisterClassA(appName, instance);

	Config::StaticTerminate();

	CoUninitialize();

	// Engine shutdown
	initialized = false;
}

//----------------------------------------------------------------------------------------------------

int ConfigGetInt(const char* key, int defaultValue)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return Config::Get()->GetInt(key, defaultValue);
}

//----------------------------------------------------------------------------------------------------

bool ConfigGetBool(const char* key, bool defaultValue)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return Config::Get()->GetBool(key, defaultValue);
}

//----------------------------------------------------------------------------------------------------

float ConfigGetFloat(const char* key, float defaultValue)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return Config::Get()->GetFloat(key, defaultValue);
}

//----------------------------------------------------------------------------------------------------

const char* ConfigGetString(const char* key, const char* defaultValue)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return Config::Get()->GetString(key, defaultValue);
}

//----------------------------------------------------------------------------------------------------

float GetTime()
{
	return myTimer.GetTotalTime();
}

//----------------------------------------------------------------------------------------------------

void SetCameraPosition(const Math::Vector3& position)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	myCamera.SetPosition(position);
}

//----------------------------------------------------------------------------------------------------

void SetCameraDirection(const Math::Vector3& direction)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	myCamera.SetDirection(direction);
}

//----------------------------------------------------------------------------------------------------

void SetCameraTarget(const Math::Vector3& target)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	myCamera.SetLookAt(target);
}

//----------------------------------------------------------------------------------------------------

X::SoundId LoadSound(const char* fileName)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return SoundEffectManager::Get()->Load(fileName);
}

//----------------------------------------------------------------------------------------------------

void ClearAllSounds()
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return SoundEffectManager::Get()->Clear();
}

//----------------------------------------------------------------------------------------------------

X::TextureId LoadTexture(const char* fileName)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return TextureManager::Get()->Load(fileName);
}

//----------------------------------------------------------------------------------------------------

void ClearAllTextures()
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return TextureManager::Get()->Clear();
}

//----------------------------------------------------------------------------------------------------

bool OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter)
{
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = myWindow;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	return GetOpenFileNameA(&ofn);
}

//----------------------------------------------------------------------------------------------------

bool SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter)
{
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = myWindow;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = title;
	return GetSaveFileNameA(&ofn);
}

//----------------------------------------------------------------------------------------------------

void PlaySoundOneShot(SoundId soundId)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SoundEffectManager::Get()->Play(soundId);
}

//----------------------------------------------------------------------------------------------------

void PlaySoundLoop(SoundId soundId)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SoundEffectManager::Get()->Play(soundId, true);
}

//----------------------------------------------------------------------------------------------------

bool IsSoundPlaying(SoundId soundId)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	return SoundEffectManager::Get()->IsPlaying(soundId);
}

//----------------------------------------------------------------------------------------------------

void StopSoundLoop(SoundId soundId)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SoundEffectManager::Get()->Stop(soundId);
}

//----------------------------------------------------------------------------------------------------

void SetBackgroundColor(const Color& color)
{
	myBackgroundColor = color;
}

//----------------------------------------------------------------------------------------------------

void Zoom(float zoom)
{
	myZoom = zoom;
	auto transform = Math::Matrix4::Scaling(zoom);
	SimpleDraw::SetTransform(transform);
	SpriteRenderer::Get()->SetTransform(transform);
}

//----------------------------------------------------------------------------------------------------

void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddLine(x0, y0, z0, x1, y1, z1, color);
}

//----------------------------------------------------------------------------------------------------

void DrawAABB(const Math::AABB& aabb, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddAABB(aabb, color);
}

//----------------------------------------------------------------------------------------------------

void DrawAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddAABB(minX, minY, minZ, maxX, maxY, maxZ, color);
}

//----------------------------------------------------------------------------------------------------

void DrawOBB(const Math::OBB& obb, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddOBB(obb, color);
}

//----------------------------------------------------------------------------------------------------

void DrawSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddSphere(center, radius, color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void DrawSphere(float x, float y, float z, float radius, const Color& color, uint32_t slices, uint32_t rings)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddSphere(Math::Vector3(x, y, z), radius, color, slices, rings);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenLine(v0, v1, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenLine(float x0, float y0, float x1, float y1, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenLine(x0, y0, x1, y1, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenRect(const Math::Rect& rect, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenRect(rect.left, rect.top, rect.right, rect.bottom, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenRect(float l, float t, float r, float b, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenRect(l, t, r, b, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenCircle(const Math::Circle& circle, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(circle, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenCircle(const Math::Vector2& center, float r, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(center, r, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenCircle(float x, float y, float r, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenCircle(x, y, r, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenArc(const Math::Vector2& center, float r, float fromAngle, float toAngle, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenArc(center, r, fromAngle, toAngle, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenDiamond(const Math::Vector2& center, float size, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenDiamond(center, size, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenDiamond(float x, float y, float size, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	SimpleDraw::AddScreenDiamond(x, y, size, color);
}

//----------------------------------------------------------------------------------------------------

void DrawScreenText(const char* str, float x, float y, float size, const Color& color)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	myTextCommands.emplace_back(converter.from_bytes(str), size, x, y, ToColor(color));
}

//----------------------------------------------------------------------------------------------------

void DrawSprite(TextureId textureId, const Math::Vector2& position, Pivot pivot, Flip flip)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, 0.0f, pivot, flip);
}

//----------------------------------------------------------------------------------------------------

void DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, Pivot pivot, Flip flip)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, position, rotation, pivot, flip);
}

//----------------------------------------------------------------------------------------------------

void DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position)
{
	XASSERT(initialized, "[XEngine] Engine not started.");
	mySpriteCommands.emplace_back(textureId, sourceRect, position, 0.0f);
}

//----------------------------------------------------------------------------------------------------

uint32_t GetSpriteWidth(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetWidth() : 0u;
}

//----------------------------------------------------------------------------------------------------

uint32_t GetSpriteHeight(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetHeight() : 0u;
}

//----------------------------------------------------------------------------------------------------

void* GetSprite(TextureId textureId)
{
	Texture* texture = TextureManager::Get()->GetTexture(textureId);
	return texture ? texture->GetShaderResourceView() : nullptr;
}

//----------------------------------------------------------------------------------------------------

uint32_t GetScreenWidth()
{
	return GraphicsSystem::Get()->GetWidth();
}

//----------------------------------------------------------------------------------------------------

uint32_t GetScreenHeight()
{
	return GraphicsSystem::Get()->GetHeight();
}

//----------------------------------------------------------------------------------------------------

int Random()
{
	return std::uniform_int_distribution<>{ 0, (std::numeric_limits<int>::max)() }(myRandomEngine);
}

//----------------------------------------------------------------------------------------------------

int Random(int min, int max)
{
	return std::uniform_int_distribution<>{ min, max }(myRandomEngine);
}

//----------------------------------------------------------------------------------------------------

float RandomFloat()
{
	return std::uniform_real_distribution<float>{ 0, 1.0f }(myRandomEngine);
}

//----------------------------------------------------------------------------------------------------

float RandomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>{ min, max }(myRandomEngine);
}

//----------------------------------------------------------------------------------------------------

Math::Vector2 RandomVector2()
{
	return Math::Vector2
	(
		RandomFloat(),
		RandomFloat()
	);
}

//----------------------------------------------------------------------------------------------------

Math::Vector2 RandomVector2(const Math::Vector2& min, const Math::Vector2& max)
{
	return Math::Vector2
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y)
	);
}

//----------------------------------------------------------------------------------------------------

Math::Vector2 RandomUnitCircle(bool normalized)
{
	Math::Vector2 randVal{
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f)
	};
	if (normalized)
		return Math::Normalize(randVal);
	return randVal;
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 RandomVector3()
{
	return Math::Vector3
	(
		RandomFloat(),
		RandomFloat(),
		RandomFloat()
	);
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 RandomVector3(const Math::Vector3& min, const Math::Vector3& max)
{
	return Math::Vector3
	(
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y),
		RandomFloat(min.z, max.z)
	);
}

//----------------------------------------------------------------------------------------------------

Math::Vector3 RandomUnitSphere()
{
	return Math::Normalize(Math::Vector3(
		RandomFloat(-1.0f, 1.0f) + 0.001f,
		RandomFloat(-1.0f, 1.0f),
		RandomFloat(-1.0f, 1.0f))
	);
}

//----------------------------------------------------------------------------------------------------

bool IsKeyDown(int key)
{
	return InputSystem::Get()->IsKeyDown(key);
}

//----------------------------------------------------------------------------------------------------

bool IsKeyPressed(int key)
{
	return InputSystem::Get()->IsKeyPressed(key);
}

//----------------------------------------------------------------------------------------------------

bool IsMouseDown(int button)
{
	return InputSystem::Get()->IsMouseDown(button);
}

//----------------------------------------------------------------------------------------------------

bool IsMousePressed(int button)
{
	return InputSystem::Get()->IsMousePressed(button);
}

//----------------------------------------------------------------------------------------------------

int GetMouseMoveX()
{
	return InputSystem::Get()->GetMouseMoveX();
}

//----------------------------------------------------------------------------------------------------

int GetMouseMoveY()
{
	return InputSystem::Get()->GetMouseMoveY();
}

//----------------------------------------------------------------------------------------------------

float GetMouseMoveZ()
{
	return InputSystem::Get()->GetMouseMoveZ();
}

//----------------------------------------------------------------------------------------------------

int GetMouseScreenX()
{
	return static_cast<int>(InputSystem::Get()->GetMouseScreenX() / myZoom);
}

//----------------------------------------------------------------------------------------------------

int GetMouseScreenY()
{
	return static_cast<int>(InputSystem::Get()->GetMouseScreenY() / myZoom);
}

//----------------------------------------------------------------------------------------------------

bool IsMouseLeftEdge()
{
	return InputSystem::Get()->IsMouseLeftEdge();
}

//----------------------------------------------------------------------------------------------------

bool IsMouseRightEdge()
{
	return InputSystem::Get()->IsMouseRightEdge();
}

//----------------------------------------------------------------------------------------------------

bool IsMouseTopEdge()
{
	return InputSystem::Get()->IsMouseTopEdge();
}

//----------------------------------------------------------------------------------------------------

bool IsMouseBottomEdge()
{
	return InputSystem::Get()->IsMouseBottomEdge();
}

//----------------------------------------------------------------------------------------------------

void ShowSystemCursor(bool show)
{
	InputSystem::Get()->ShowSystemCursor(show);
}

//----------------------------------------------------------------------------------------------------

void SetMouseClipToWindow(bool clip)
{
	InputSystem::Get()->SetMouseClipToWindow(clip);
}

//----------------------------------------------------------------------------------------------------

bool IsMouseClipToWindow()
{
	return InputSystem::Get()->IsMouseClipToWindow();
}

//----------------------------------------------------------------------------------------------------

Math::Ray GetMouseRay()
{
	const int screenX = InputSystem::Get()->GetMouseScreenX();
	const int screenY = InputSystem::Get()->GetMouseScreenY();
	return GetScreenRay(screenX, screenY);
}

//----------------------------------------------------------------------------------------------------

Math::Ray GetScreenRay(int screenX, int screenY)
{
	const uint32_t screenWidth = GraphicsSystem::Get()->GetWidth();
	const uint32_t screenHeight = GraphicsSystem::Get()->GetHeight();
	return myCamera.ScreenPointToRay(screenX, screenY, screenWidth, screenHeight);
}

//----------------------------------------------------------------------------------------------------

bool IsGamePadConnected(int player)
{
	return InputSystem::Get()->IsGamePadConnected(player);
}

//----------------------------------------------------------------------------------------------------

bool IsAPressed(int player)
{
	return InputSystem::Get()->IsAPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsBPressed(int player)
{
	return InputSystem::Get()->IsBPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsXPressed(int player)
{
	return InputSystem::Get()->IsXPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsYPressed(int player)
{
	return InputSystem::Get()->IsYPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsLeftShoulderPressed(int player)
{
	return InputSystem::Get()->IsLeftShoulderPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsLeftTriggerPressed(int player)
{
	return InputSystem::Get()->IsLeftTriggerPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsRightShoulderPressed(int player)
{
	return InputSystem::Get()->IsRightShoulderPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsRightTriggerPressed(int player)
{
	return InputSystem::Get()->IsRightTriggerPressed(player);
}

//----------------------------------------------------------------------------------------------------

bool IsDPadUp(int player)
{
	return InputSystem::Get()->IsDPadUp(player);
}

//----------------------------------------------------------------------------------------------------

bool IsDPadDown(int player)
{
	return InputSystem::Get()->IsDPadDown(player);
}

//----------------------------------------------------------------------------------------------------

bool IsDPadLeft(int player)
{
	return InputSystem::Get()->IsDPadLeft(player);
}

//----------------------------------------------------------------------------------------------------

bool IsDPadRight(int player)
{
	return InputSystem::Get()->IsDPadRight(player);
}

//----------------------------------------------------------------------------------------------------

bool IsLeftThumbStickUp(int player)
{
	return InputSystem::Get()->IsLeftThumbStickUp(player);
}

//----------------------------------------------------------------------------------------------------

bool IsLeftThumbStickDown(int player)
{
	return InputSystem::Get()->IsLeftThumbStickDown(player);
}

//----------------------------------------------------------------------------------------------------

bool IsLeftThumbStickLeft(int player)
{
	return InputSystem::Get()->IsLeftThumbStickLeft(player);
}

//----------------------------------------------------------------------------------------------------

bool IsLeftThumbStickRight(int player)
{
	return InputSystem::Get()->IsLeftThumbStickRight(player);
}

//----------------------------------------------------------------------------------------------------

bool IsRightThumbStickUp(int player)
{
	return InputSystem::Get()->IsRightThumbStickUp(player);
}

//----------------------------------------------------------------------------------------------------

bool IsRightThumbStickDown(int player)
{
	return InputSystem::Get()->IsRightThumbStickDown(player);
}

//----------------------------------------------------------------------------------------------------

bool IsRightThumbStickLeft(int player)
{
	return InputSystem::Get()->IsRightThumbStickLeft(player);
}

//----------------------------------------------------------------------------------------------------

bool IsRightThumbStickRight(int player)
{
	return InputSystem::Get()->IsRightThumbStickRight(player);
}

//----------------------------------------------------------------------------------------------------

float GetLeftAnalogX(int player)
{
	return InputSystem::Get()->GetLeftAnalogX(player);
}

//----------------------------------------------------------------------------------------------------

float GetLeftAnalogY(int player)
{
	return InputSystem::Get()->GetLeftAnalogY(player);
}

//----------------------------------------------------------------------------------------------------

float GetRightAnalogX(int player)
{
	return InputSystem::Get()->GetRightAnalogX(player);
}

//----------------------------------------------------------------------------------------------------

float GetRightAnalogY(int player)
{
	return InputSystem::Get()->GetRightAnalogY(player);
}

} // namespace X