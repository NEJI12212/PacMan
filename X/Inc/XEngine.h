//====================================================================================================
// Filename:	XEngine.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_H
#define INCLUDED_XENGINE_H

#include "XCore.h"
#include "XColors.h"
#include "XMath.h"
#include "XTypes.h"

namespace X {

void Start(const char* configFileName = nullptr);
void Run(bool (*GameLoop)(float));
void Stop();

// Config Functions
int ConfigGetInt(const char* key, int defaultValue = 0);
bool ConfigGetBool(const char* key, bool defaultValue = false);
float ConfigGetFloat(const char* key, float defaultValue = 0.0f);
const char* ConfigGetString(const char* key, const char* defaultValue = "");

// Time Functions
float GetTime();

// Camera Functions
void SetCameraPosition(const Math::Vector3& position);
void SetCameraDirection(const Math::Vector3& direction);
void SetCameraTarget(const Math::Vector3& target);

// Resource Functions
SoundId LoadSound(const char* fileName);
void ClearAllSounds();
TextureId LoadTexture(const char* fileName);
void ClearAllTextures();

// File Dialog Functions
// Note: filter string has format "<Description>\0*.<extension>\0", with multiple entries separated by ;
// e.g
//   "Text Files (*.txt)\0*.txt"
//   "PNG Files (*.png)\0*.png;\0JPG Files (*.jpg)\0*.jpg"
bool OpenFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);
bool SaveFileDialog(char fileName[MAX_PATH], const char* title, const char* filter);

// Audio Functions
void PlaySoundOneShot(SoundId soundId);
void PlaySoundLoop(SoundId soundId);
bool IsSoundPlaying(SoundId id);
void StopSoundLoop(SoundId soundId);

void SetBackgroundColor(const Color& color);

// 2D Transform
void Zoom(float zoom);

// Debug Draw Functions
void DrawLine(float x0, float y0, float z0, float x1, float y1, float z1, const Color& color);
void DrawAABB(const Math::AABB& aabb, const Color& color);
void DrawAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color);
void DrawOBB(const Math::OBB& obb, const Color& color);
void DrawSphere(const Math::Vector3& center, float radius, const Color& color, uint32_t slices = 8, uint32_t rings = 4);
void DrawSphere(float x, float y, float z, float radius, const Color& color, uint32_t slices = 8, uint32_t rings = 4);
void DrawScreenLine(const Math::Vector2& v0, const Math::Vector2& v1, const Color& color);
void DrawScreenLine(float x0, float y0, float x1, float y1, const Color& color);
void DrawScreenRect(const Math::Rect& rect, const Color& color);
void DrawScreenRect(float l, float t, float r, float b, const Color& color);
void DrawScreenCircle(const Math::Vector2& center, float r, const Color& color);
void DrawScreenCircle(float x, float y, float r, const Color& color);
void DrawScreenArc(const Math::Vector2& center, float r, float fromAngle, float toAngle, const Color& color);
void DrawScreenDiamond(const Math::Vector2& center, float size, const Color& color);
void DrawScreenDiamond(float x, float y, float size, const Color& color);
void DrawScreenText(const char* str, float x, float y, float size, const Color& color);

// Sprite Functions
void DrawSprite(TextureId textureId, const Math::Vector2& position, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
void DrawSprite(TextureId textureId, const Math::Vector2& position, float rotation, Pivot pivot = Pivot::Center, Flip flip = Flip::None);
void DrawSprite(TextureId textureId, const Math::Rect& sourceRect, const Math::Vector2& position);
uint32_t GetSpriteWidth(TextureId textureId);
uint32_t GetSpriteHeight(TextureId textureId);
void* GetSprite(TextureId textureId);

uint32_t GetScreenWidth();
uint32_t GetScreenHeight();

// Random Functions
int Random();
int Random(int min, int max);
float RandomFloat();
float RandomFloat(float min, float max);
Math::Vector2 RandomVector2();
Math::Vector2 RandomVector2(const Math::Vector2& min, const Math::Vector2& max);
Math::Vector2 RandomUnitCircle(bool normalized = true);
Math::Vector3 RandomVector3();
Math::Vector3 RandomVector3(const Math::Vector3& min, const Math::Vector3& max);
Math::Vector3 RandomUnitSphere();

// Keyboard Functions
bool IsKeyDown(int key);
bool IsKeyPressed(int key);

// Mouse Functions
bool IsMouseDown(int button);
bool IsMousePressed(int button);
int GetMouseMoveX();
int GetMouseMoveY();
float GetMouseMoveZ();
int GetMouseScreenX();
int GetMouseScreenY();
bool IsMouseLeftEdge();
bool IsMouseRightEdge();
bool IsMouseTopEdge();
bool IsMouseBottomEdge();

void ShowSystemCursor(bool show);
void SetMouseClipToWindow(bool clip);
bool IsMouseClipToWindow();

Math::Ray GetMouseRay();
Math::Ray GetScreenRay(int screenX, int screenY);

// GamePad Functions
bool IsGamePadConnected(int player = 0);

bool IsAPressed(int player = 0);
bool IsBPressed(int player = 0);
bool IsXPressed(int player = 0);
bool IsYPressed(int player = 0);

bool IsLeftShoulderPressed(int player = 0);
bool IsLeftTriggerPressed(int player = 0);
bool IsRightShoulderPressed(int player = 0);
bool IsRightTriggerPressed(int player = 0);

bool IsDPadUp(int player = 0);
bool IsDPadDown(int player = 0);
bool IsDPadLeft(int player = 0);
bool IsDPadRight(int player = 0);

bool IsLeftThumbStickUp(int player = 0);
bool IsLeftThumbStickDown(int player = 0);
bool IsLeftThumbStickLeft(int player = 0);
bool IsLeftThumbStickRight(int player = 0);

bool IsRightThumbStickUp(int player = 0);
bool IsRightThumbStickDown(int player = 0);
bool IsRightThumbStickLeft(int player = 0);
bool IsRightThumbStickRight(int player = 0);

float GetLeftAnalogX(int player = 0);
float GetLeftAnalogY(int player = 0);
float GetRightAnalogX(int player = 0);
float GetRightAnalogY(int player = 0);

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_H