//====================================================================================================
// Filename:	Camera.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "Camera.h"

using namespace X;

namespace
{
	Camera* sCamera = nullptr;
}

//----------------------------------------------------------------------------------------------------

void Camera::Set(Camera* camera)
{
	sCamera = camera;
}

//----------------------------------------------------------------------------------------------------

Camera* Camera::Get()
{
	return sCamera;
}

//----------------------------------------------------------------------------------------------------

Camera::Camera()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mLook(0.0f, 0.0f, 1.0f)
	, mFOV(60.0f * Math::kDegToRad)
	, mNearPlane(0.01f)
	, mFarPlane(1000.0f)
{
}

//----------------------------------------------------------------------------------------------------

Camera::~Camera()
{
	if (sCamera == this)
	{
		sCamera = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetDirection(const Math::Vector3& direction)
{
	mLook = Normalize(direction);
}

//----------------------------------------------------------------------------------------------------

void Camera::SetLookAt(const Math::Vector3& target)
{
	mLook = Normalize(target - mPosition);
}

//----------------------------------------------------------------------------------------------------

void Camera::Walk(float distance)
{
	mPosition += mLook * distance;
}

//----------------------------------------------------------------------------------------------------

void Camera::Strafe(float distance)
{
	const Math::Vector3 right = Normalize(Cross(Math::Vector3::YAxis(), mLook));
	mPosition += right * distance;
}

//----------------------------------------------------------------------------------------------------

void Camera::Rise(float distance)
{
	mPosition += Math::Vector3::YAxis() * distance;
}

//----------------------------------------------------------------------------------------------------

void Camera::Yaw(float degree)
{
	Math::Matrix4 matRotate = Math::Matrix4::RotationY(degree * Math::kDegToRad);
	mLook = TransformNormal(mLook, matRotate);
}

//----------------------------------------------------------------------------------------------------

void Camera::Pitch(float degree)
{
	const Math::Vector3 right = Normalize(Cross(Math::Vector3::YAxis(), mLook));
	const Math::Matrix4 matRotate = Math::MatrixRotationAxis(right, degree * Math::kDegToRad);
	const Math::Vector3 newLook = TransformNormal(mLook, matRotate);
	const float dot = Dot(newLook, Math::Vector3::YAxis());

	// Avoid looking straight up or down
	if (Math::Abs(dot) < 0.995f)
	{
		mLook = newLook;
	}
}

//----------------------------------------------------------------------------------------------------

void Camera::SetFOV(float fov)
{
	const float kMinFOV = 10.0f * Math::kDegToRad;
	const float kMaxFOV = 150.0f * Math::kDegToRad;
	mFOV = Math::Clamp(fov, kMinFOV, kMaxFOV);
}

//----------------------------------------------------------------------------------------------------

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

//----------------------------------------------------------------------------------------------------

Math::Matrix4 Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mLook;
	const Math::Vector3 r = Normalize(Cross(Math::Vector3::YAxis(), l));
	const Math::Vector3 u = Normalize(Cross(l, r));
	const float dx = -Dot(r, mPosition);
	const float dy = -Dot(u, mPosition);
	const float dz = -Dot(l, mPosition);

	return Math::Matrix4
	(
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx, dy, dz, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix4 Camera::GetProjectionMatrix(uint32_t screenWidth, uint32_t screenHeight) const
{
	const float aspect = (float)screenWidth / (float)screenHeight;
	const float h = 1 / tan(mFOV * 0.5f);
	const float w = h / aspect;
	const float f = mFarPlane;
	const float n = mNearPlane;
	const float d = f / (f - n);

	return Math::Matrix4
	(
		w, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, d, 1.0f,
		0.0f, 0.0f, -n * d, 0.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Ray Camera::ScreenPointToRay(int screenX, int screenY, uint32_t screenWidth, uint32_t screenHeight) const
{
	const float aspect = (float)screenWidth / (float)screenHeight;
	const float halfWidth = screenWidth * 0.5f;
	const float halfHeight = screenHeight * 0.5f;
	const float tanFOV = tanf(mFOV * 0.5f);
	const float dx = tanFOV * ((float)screenX / halfWidth - 1.0f) * aspect;
	const float dy = tanFOV * (1.0f - (float)screenY / halfHeight);

	Math::Ray ray;
	ray.org = Math::Vector3::Zero();
	ray.dir = Normalize(Math::Vector3(dx, dy, 1.0f));

	Math::Matrix4 invMatView = Inverse(GetViewMatrix());
	ray.org = TransformCoord(ray.org, invMatView);
	ray.dir = TransformNormal(ray.dir, invMatView);
	return ray;
}