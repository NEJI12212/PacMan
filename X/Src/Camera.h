//====================================================================================================
// Filename:	Camera.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef	INCLUDED_XENGINE_CAMERA_H
#define INCLUDED_XENGINE_CAMERA_H

#include "XMath.h"

namespace X {

// This camera assumes YAxis as up direction always
class Camera
{
public:
	static void Set(Camera* camera);
	static Camera* Get();

	Camera();
	~Camera();

	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);
	void SetLookAt(const Math::Vector3& target);

	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);

	void Yaw(float degree);
	void Pitch(float degree);
	
	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);

	const Math::Vector3& GetPosition() const	{ return mPosition; }

	float GetFOV() const						{ return mFOV; }
	float GetNearPlane() const					{ return mNearPlane; }
	float GetFarPlane() const					{ return mFarPlane; }
	
	Math::Matrix4 GetViewMatrix() const;
	Math::Matrix4 GetProjectionMatrix(uint32_t screenWidth, uint32_t screenHeight) const;

	Math::Ray ScreenPointToRay(int screenX, int screenY, uint32_t screenWidth, uint32_t screenHeight) const;

private:
	Math::Vector3 mPosition;
	Math::Vector3 mLook;

	float mFOV;
	float mNearPlane;
	float mFarPlane;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_CAMERA_H