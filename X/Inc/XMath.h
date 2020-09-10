//====================================================================================================
// Filename:	XMath.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_XENGINE_MATH_H
#define INCLUDED_XENGINE_MATH_H

#include <math.h>
#include <vector>

namespace X {
namespace Math {

struct Vector2
{
	float x, y;

	constexpr Vector2() noexcept : Vector2{ 0.0f } {}
	constexpr Vector2(float f) noexcept : Vector2{ f, f } {}
	constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

	static Vector2 Zero()						{ return Vector2(); }
	static Vector2 One()						{ return Vector2(1.0f, 1.0f); }
	static Vector2 XAxis()						{ return Vector2(1.0f, 0.0f); }
	static Vector2 YAxis()						{ return Vector2(0.0f, 1.0f); }

	Vector2 operator-() const					{ return Vector2(-x, -y); }
	Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator*(float s) const			{ return Vector2(x * s, y * s); }
	Vector2 operator/(float s) const			{ return Vector2(x / s, y / s); }

	Vector2& operator+=(const Vector2& rhs)		{ x += rhs.x; y += rhs.y; return *this; }
	Vector2& operator-=(const Vector2& rhs)		{ x -= rhs.x; y -= rhs.y; return *this; }
	Vector2& operator*=(float s)				{ x *= s; y *= s; return *this; }
	Vector2& operator/=(float s)				{ x /= s; y /= s; return *this; }
};

//----------------------------------------------------------------------------------------------------

struct Vector3
{
	float x, y, z;

	constexpr Vector3() noexcept : Vector3{ 0.0f } {}
	constexpr Vector3(float f) noexcept : Vector3{ f, f, f } {}
	constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

	static Vector3 Zero()						{ return Vector3(); }
	static Vector3 One()						{ return Vector3(1.0f, 1.0f, 1.0f); }
	static Vector3 XAxis()						{ return Vector3(1.0f, 0.0f, 0.0f); }
	static Vector3 YAxis()						{ return Vector3(0.0f, 1.0f, 0.0f); }
	static Vector3 ZAxis()						{ return Vector3(0.0f, 0.0f, 1.0f); }

	Vector3 operator-() const					{ return Vector3(-x, -y, -z); }
	Vector3 operator+(const Vector3& rhs) const	{ return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator-(const Vector3& rhs) const	{ return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator*(float s) const			{ return Vector3(x * s, y * s, z * s); }
	Vector3 operator/(float s) const			{ return Vector3(x / s, y / s, z / s); }

	Vector3& operator+=(const Vector3& rhs)		{ x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vector3& operator-=(const Vector3& rhs)		{ x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	Vector3& operator*=(float s)				{ x *= s; y *= s; z *= s; return *this; }
	Vector3& operator/=(float s)				{ x /= s; y /= s; z /= s; return *this; }
};

//----------------------------------------------------------------------------------------------------

struct Vector4
{
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };
	union { float w; float a; };

	constexpr Vector4() noexcept : Vector4{ 0.0f } {}
	constexpr Vector4(float f) noexcept : Vector4{ f, f, f, f } {}
	constexpr Vector4(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{ w } {}
	
	Vector4 operator-() const					{ return Vector4(-x, -y, -z, -w); }
	Vector4 operator+(const Vector4& rhs) const	{ return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Vector4 operator-(const Vector4& rhs) const	{ return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Vector4 operator*(float s) const			{ return Vector4(x * s, y * s, z * s, w * s); }
	Vector4 operator/(float s) const			{ return Vector4(x / s, y / s, z / s, w / s); }

	Vector4& operator+=(const Vector4& rhs)		{ x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	Vector4& operator-=(const Vector4& rhs)		{ x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
	Vector4& operator*=(float s)				{ x *= s; y *= s; z *= s; w *= s; return *this; }
	Vector4& operator/=(float s)				{ x /= s; y /= s; z /= s; w /= s; return *this; }
};

//----------------------------------------------------------------------------------------------------

struct Quaternion
{
	float x, y, z, w;

	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	static Quaternion Zero()							{ return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); }
	static Quaternion Identity()						{ return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

	Quaternion operator+(const Quaternion& rhs) const	{ return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Quaternion operator*(float s) const					{ return Quaternion(x * s, y * s, z * s, w * s); }
	Quaternion operator/(float s) const					{ return Quaternion(x / s, y / s, z / s, w / s); }
};

//----------------------------------------------------------------------------------------------------

struct Matrix3
{
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3()
		: _11(1.0f), _12(0.0f), _13(0.0f)
		, _21(0.0f), _22(1.0f), _23(0.0f)
		, _31(0.0f), _32(0.0f), _33(1.0f)
	{}

	Matrix3(
		float _11, float _12, float _13,
		float _21, float _22, float _23,
		float _31, float _32, float _33)
		: _11(_11), _12(_12), _13(_13)
		, _21(_21), _22(_22), _23(_23)
		, _31(_31), _32(_32), _33(_33)
	{}

	static Matrix3 Zero()							{ return Matrix3(); }
	static Matrix3 Identity()						{ return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Translation(float x, float y)	{ return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, 1.0f); }
	static Matrix3 Translation(const Vector2& v)	{ return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, 1.0f); }
	static Matrix3 Rotation(float rad)				{ return Matrix3(cosf(rad), sinf(rad), 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Scaling(float s)					{ return Matrix3(s, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Scaling(float sx, float sy)		{ return Matrix3(sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix3 Scaling(const Vector2& s)		{ return Matrix3(s.x, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 1.0f); }

	Matrix3 operator-() const
	{
		return Matrix3(
			-_11, -_12, -_13,
			-_21, -_22, -_23,
			-_31, -_32, -_33);
	}
	Matrix3 operator+(const Matrix3& rhs) const
	{
		return Matrix3(
			_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
			_21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
			_31 + rhs._31, _32 + rhs._32, _33 + rhs._33);
	}
	Matrix3 operator-(const Matrix3& rhs) const
	{
		return Matrix3(
			_11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
			_21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
			_31 - rhs._31, _32 - rhs._32, _33 - rhs._33);
	}
	Matrix3 operator*(const Matrix3& rhs) const
	{
		return Matrix3(
			(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31),
			(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32),
			(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33),

			(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31),
			(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32),
			(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33),

			(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31),
			(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32),
			(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33));
	}
	Matrix3 operator*(float s) const
	{
		return Matrix3(
			_11 * s, _12 * s, _13 * s,
			_21 * s, _22 * s, _23 * s,
			_31 * s, _32 * s, _33 * s);
	}
	Matrix3 operator/(float s) const
	{
		return Matrix3(
			_11 / s, _12 / s, _13 / s,
			_21 / s, _22 / s, _23 / s,
			_31 / s, _32 / s, _33 / s);
	}
	Matrix3 operator+=(const Matrix3& rhs)
	{
		_11 += rhs._11; _12 += rhs._12; _13 += rhs._13;
		_21 += rhs._21; _22 += rhs._22; _23 += rhs._23;
		_31 += rhs._31; _32 += rhs._32; _33 += rhs._33;
		return *this;
	}
};

//----------------------------------------------------------------------------------------------------

struct Matrix4
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	Matrix4()
		: _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
		, _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
		, _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
		, _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
	{}

	Matrix4(
		float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44)
		: _11(_11), _12(_12), _13(_13), _14(_14)
		, _21(_21), _22(_22), _23(_23), _24(_24)
		, _31(_31), _32(_32), _33(_33), _34(_34)
		, _41(_41), _42(_42), _43(_43), _44(_44)
	{}

	static Matrix4 Zero()									{ return Matrix4(); }
	static Matrix4 Identity()								{ return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ); }
	static Matrix4 Translation(float x, float y, float z)	{ return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f ); }
	static Matrix4 Translation(const Vector3& v)			{ return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, v.x, v.y, v.z, 1.0f); }
	static Matrix4 RotationX(float rad)						{ return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 RotationY(float rad)						{ return Matrix4(cosf(rad), 0.0f, -sinf(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sinf(rad), 0.0f, cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 RotationZ(float rad)						{ return Matrix4(cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Scaling(float s)							{ return Matrix4(s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Scaling(float sx, float sy, float sz)	{ return Matrix4(sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
	static Matrix4 Scaling(const Vector3& s)				{ return Matrix4(s.x, 0.0f, 0.0f, 0.0f, 0.0f, s.y, 0.0f, 0.0f, 0.0f, 0.0f, s.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

	Matrix4 operator-() const
	{
		return Matrix4(
			-_11, -_12, -_13, -_14,
			-_21, -_22, -_23, -_24,
			-_31, -_32, -_33, -_34,
			-_41, -_42, -_43, -_44);
	}
	Matrix4 operator+(const Matrix4& rhs) const
	{
		return Matrix4(
			_11 + rhs._11, _12 + rhs._12, _13 + rhs._13, _14 + rhs._14,
			_21 + rhs._21, _22 + rhs._22, _23 + rhs._23, _24 + rhs._24,
			_31 + rhs._31, _32 + rhs._32, _33 + rhs._33, _34 + rhs._34,
			_41 + rhs._41, _42 + rhs._42, _43 + rhs._43, _44 + rhs._44);
	}
	Matrix4 operator-(const Matrix4& rhs) const
	{
		return Matrix4(
			_11 - rhs._11, _12 - rhs._12, _13 - rhs._13, _14 - rhs._14,
			_21 - rhs._21, _22 - rhs._22, _23 - rhs._23, _24 - rhs._24,
			_31 - rhs._31, _32 - rhs._32, _33 - rhs._33, _34 - rhs._34,
			_41 - rhs._41, _42 - rhs._42, _43 - rhs._43, _44 - rhs._44);
	}
	Matrix4 operator*(const Matrix4& rhs) const
	{
		return Matrix4(
			(_11 * rhs._11) + (_12 * rhs._21) + (_13 * rhs._31) + (_14 * rhs._41),
			(_11 * rhs._12) + (_12 * rhs._22) + (_13 * rhs._32) + (_14 * rhs._42),
			(_11 * rhs._13) + (_12 * rhs._23) + (_13 * rhs._33) + (_14 * rhs._43),
			(_11 * rhs._14) + (_12 * rhs._24) + (_13 * rhs._34) + (_14 * rhs._44),

			(_21 * rhs._11) + (_22 * rhs._21) + (_23 * rhs._31) + (_24 * rhs._41),
			(_21 * rhs._12) + (_22 * rhs._22) + (_23 * rhs._32) + (_24 * rhs._42),
			(_21 * rhs._13) + (_22 * rhs._23) + (_23 * rhs._33) + (_24 * rhs._43),
			(_21 * rhs._14) + (_22 * rhs._24) + (_23 * rhs._34) + (_24 * rhs._44),

			(_31 * rhs._11) + (_32 * rhs._21) + (_33 * rhs._31) + (_34 * rhs._41),
			(_31 * rhs._12) + (_32 * rhs._22) + (_33 * rhs._32) + (_34 * rhs._42),
			(_31 * rhs._13) + (_32 * rhs._23) + (_33 * rhs._33) + (_34 * rhs._43),
			(_31 * rhs._14) + (_32 * rhs._24) + (_33 * rhs._34) + (_34 * rhs._44),

			(_41 * rhs._11) + (_42 * rhs._21) + (_43 * rhs._31) + (_44 * rhs._41),
			(_41 * rhs._12) + (_42 * rhs._22) + (_43 * rhs._32) + (_44 * rhs._42),
			(_41 * rhs._13) + (_42 * rhs._23) + (_43 * rhs._33) + (_44 * rhs._43),
			(_41 * rhs._14) + (_42 * rhs._24) + (_43 * rhs._34) + (_44 * rhs._44));
	}
	Matrix4 operator*(float s) const
	{
		return Matrix4(
			_11 * s, _12 * s, _13 * s, _14 * s,
			_21 * s, _22 * s, _23 * s, _24 * s,
			_31 * s, _32 * s, _33 * s, _34 * s,
			_41 * s, _42 * s, _43 * s, _44 * s);
	}
	Matrix4 operator/(float s) const
	{
		return Matrix4(
			_11 / s, _12 / s, _13 / s, _14 / s,
			_21 / s, _22 / s, _23 / s, _24 / s,
			_31 / s, _32 / s, _33 / s, _34 / s,
			_41 / s, _42 / s, _43 / s, _44 / s);
	}
	Matrix4 operator+=(const Matrix4& rhs)
	{
		_11 += rhs._11; _12 += rhs._12; _13 += rhs._13; _14 += rhs._14;
		_21 += rhs._21; _22 += rhs._22; _23 += rhs._23; _24 += rhs._24;
		_31 += rhs._31; _32 += rhs._32; _33 += rhs._33; _34 += rhs._34;
		_41 += rhs._41; _42 += rhs._42; _43 += rhs._43; _44 += rhs._44;
		return *this;
	}
};

//----------------------------------------------------------------------------------------------------

struct LineSegment
{
	Vector2 from;
	Vector2 to;

	LineSegment() : from(0.0f, 0.0f), to(0.0f, 0.0f) {}
	LineSegment(float x0, float y0, float x1, float y1) : from(x0, y0), to(x1, y1) {}
	LineSegment(const Vector2& v0, const Vector2& v1) : from(v0), to(v1) {}
};

//----------------------------------------------------------------------------------------------------

struct Rect
{
	#pragma warning(push)
	#pragma warning(disable: 4201)
	union
	{
		struct { float left, top; };
		X::Math::Vector2 min;
	};
	union
	{
		struct { float right, bottom; };
		X::Math::Vector2 max;
	};
	#pragma warning(pop)

	constexpr Rect() : Rect{ 0.0f, 0.0f, 1.0f, 1.0f } {}
	constexpr Rect(float left, float top, float right, float bottom) : left{ left }, top{ top }, right{ right }, bottom{ bottom } {}
};

//----------------------------------------------------------------------------------------------------

struct Circle
{
	Math::Vector2 center;
	float radius;

	Circle() : center(0.0f, 0.0f), radius(1.0f) {}
	Circle(const Math::Vector2& center, float radius) : center(center), radius(radius) {}
	Circle(float x, float y, float radius) : center(x, y), radius(radius) {}
};

//----------------------------------------------------------------------------------------------------

struct AABB
{
	Vector3 center;
	Vector3 extend;

	AABB() : center(0.0f, 0.0f, 0.0f), extend(0.0f, 0.0f, 0.0f) {}
	AABB(const Vector3& center, const Vector3& extend) : center(center), extend(extend) {}
};

//----------------------------------------------------------------------------------------------------

struct OBB
{
	Vector3 center;
	Vector3 extend;
	Quaternion orientation;

	OBB()
		: center(0.0f, 0.0f, 0.0f)
		, extend(1.0f, 1.0f, 1.0f)
		, orientation(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	OBB(float x, float y, float z, float sx, float sy, float sz)
		: center(x, y, z)
		, extend(sx, sy, sz)
		, orientation(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	OBB(float x, float y, float z, float sx, float sy, float sz, const Quaternion& orientation)
		: center(x, y, z)
		, extend(sx, sy, sz)
		, orientation(orientation)
	{}

	OBB(const Vector3& center, const Vector3& extend, const Quaternion& orientation)
		: center(center)
		, extend(extend)
		, orientation(orientation)
	{}
};

//----------------------------------------------------------------------------------------------------

struct Sphere
{
	Vector3 center;
	float radius;

	Sphere() : center(0.0f, 0.0f, 0.0f), radius(1.0f) {}
	Sphere(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
	Sphere(const Vector3& center, float radius) : center(center), radius(radius) {}
};

//----------------------------------------------------------------------------------------------------

struct Ray
{
	Vector3 org;
	Vector3 dir;

	Ray() : org(0.0f, 0.0f, 0.0f), dir(0.0f, 0.0f, 1.0f) {}
	Ray(float x, float y, float z, float dx, float dy, float dz) : org(x, y, z), dir(dx, dy, dz) {}
	Ray(const Vector3& org, const Vector3& dir) : org(org), dir(dir) {}
};

//----------------------------------------------------------------------------------------------------

struct Plane
{
	Vector3 n;
	float d;

	Plane() : n(0.0f, 1.0f, 0.0f), d(0.0f) {}
	Plane(float a, float b, float c, float d) : n(a, b, c), d(d) {}
};

//----------------------------------------------------------------------------------------------------

extern const float kPi;
extern const float kTwoPi;
extern const float kPiByTwo;
extern const float kRootTwo;
extern const float kRootThree;
extern const float kDegToRad;
extern const float kRadToDeg;

//----------------------------------------------------------------------------------------------------

template <typename T> inline T Min(T a, T b)						{ return (a > b) ? b : a; }
template <typename T> inline T Max(T a, T b)						{ return (a < b) ? b : a; }
template <typename T> inline T Clamp(T value, T min, T max)			{ return Max(min, Min(max, value)); }

template <typename T> inline T Abs(T value)							{ return (value >= 0) ? value : -value; }
inline float Sign(float value)										{ return (value >= 0.0f) ? 1.0f : -1.0f; }
inline float Sqr(float value)										{ return value * value; }
inline float Sqrt(float value)										{ return sqrtf(value); }

inline bool Compare(float a, float b, float epsilon = FLT_MIN)		{ return Abs(a - b) <= epsilon; }
inline bool IsZero(float value)										{ return Compare(value, 0.0f); }
inline bool IsZero(const Vector2& v)								{ return IsZero(v.x) && IsZero(v.y); }
inline bool IsZero(const Vector3& v)								{ return IsZero(v.x) && IsZero(v.y) && IsZero(v.z); }
inline bool IsEmpty(const Rect& rect)								{ return rect.right <= rect.left || rect.bottom <= rect.top; }

inline Vector2 PerpendicularLH(const Vector2& v)					{ return Vector2(-v.y, v.x); }
inline Vector2 PerpendicularRH(const Vector2& v)					{ return Vector2(v.y, -v.x); }

inline float MagnitudeSqr(const Vector2& v)							{ return (v.x * v.x) + (v.y * v.y); }
inline float MagnitudeSqr(const Vector3& v)							{ return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }
inline float Magnitude(const Vector2& v)							{ return Sqrt(MagnitudeSqr(v)); }
inline float Magnitude(const Vector3& v)							{ return Sqrt(MagnitudeSqr(v)); }
inline float MagnitudeXZSqr(const Vector3& v)						{ return (v.x * v.x) + (v.z * v.z); }
inline float MagnitudeXZ(const Vector3& v)							{ return Sqrt(MagnitudeXZSqr(v)); }
inline float Magnitude(const Quaternion& q)							{ return Sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w)); }

inline Vector2 Normalize(const Vector2& v)							{ return v / Magnitude(v); }
inline Vector3 Normalize(const Vector3& v)							{ return v / Magnitude(v); }
inline Quaternion Normalize(const Quaternion& q)					{ return q / Magnitude(q); }

inline float DistanceSqr(const Vector2& a, const Vector2& b)		{ return MagnitudeSqr(a - b); }
inline float DistanceSqr(const Vector3& a, const Vector3& b)		{ return MagnitudeSqr(a - b); }
inline float Distance(const Vector2& a, const Vector2& b)			{ return Sqrt(DistanceSqr(a, b)); }
inline float Distance(const Vector3& a, const Vector3& b)			{ return Sqrt(DistanceSqr(a, b)); }
inline float DistanceXZSqr(const Vector3& a, const Vector3& b)		{ return MagnitudeXZSqr(a - b); }
inline float DistanceXZ(const Vector3& a, const Vector3& b)			{ return Sqrt(DistanceXZSqr(a, b)); }
inline float Dot(const Vector2& a, const Vector2& b)				{ return (a.x * b.x) + (a.y * b.y); }
inline float Dot(const Vector3& a, const Vector3& b)				{ return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
inline Vector3 Cross(const Vector3& a, const Vector3& b)			{ return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)); }
inline Vector2 Project(const Vector2& v, const Vector2& n)			{ return n * (Dot(v, n) / Dot(n, n)); }
inline Vector3 Project(const Vector3& v, const Vector3& n)			{ return n * (Dot(v, n) / Dot(n, n)); }
inline Vector2 Reflect(const Vector2& v, const Vector2& normal)		{ return v - (normal * Dot(v, normal) * 2.0f); }
inline Vector3 Reflect(const Vector3& v, const Vector3& normal)		{ return v - (normal * Dot(v, normal) * 2.0f); }

inline Vector2 GetTranslation(const Matrix3& m)						{ return Vector2(m._31, m._32); }
inline Vector3 GetTranslation(const Matrix4& m)						{ return Vector3(m._41, m._42, m._43); }
inline Vector3 GetRight(const Matrix4& m)							{ return Vector3(m._11, m._12, m._13); }
inline Vector3 GetUp(const Matrix4& m)								{ return Vector3(m._21, m._22, m._23); }
inline Vector3 GetForward(const Matrix4& m)							{ return Vector3(m._31, m._32, m._33); }

inline Vector3 GetPoint(const Ray& ray, float distance)				{ return ray.org + (ray.dir * distance); }

inline Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t)	{ return v0 + ((v1 - v0) * t); }
inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t)	{ return v0 + ((v1 - v0) * t); }
inline Quaternion Lerp(Quaternion q0, Quaternion q1, float t)		{ return q0 * (1.0f - t) + (q1 * t); }

inline Vector2 Rotate(const Vector2& v, float rad)
{
	const float kCosAngle = cos(rad);
	const float kSinAngle = sin(rad);
	return Vector2
	(
		v.x * kCosAngle - v.y * kSinAngle,
		v.y * kCosAngle + v.x * kSinAngle
	);
}

inline float Determinant(const Matrix3& m)
{
	float det = 0.0f;
	det = (m._11 * (m._22 * m._33 - m._23 * m._32));
	det -= (m._12 * (m._21 * m._33 - m._23 * m._31));
	det += (m._13 * (m._21 * m._32 - m._22 * m._31));
	return det;
}

inline float Determinant(const Matrix4& m)
{
	float det = 0.0f;
	det  = (m._11 * (m._22 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._32 * m._44 - (m._42 * m._34)) + m._24 * (m._32 * m._43 - (m._42 * m._33))));
	det -= (m._12 * (m._21 * (m._33 * m._44 - (m._43 * m._34)) - m._23 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._43 - (m._41 * m._33))));
	det += (m._13 * (m._21 * (m._32 * m._44 - (m._42 * m._34)) - m._22 * (m._31 * m._44 - (m._41 * m._34)) + m._24 * (m._31 * m._42 - (m._41 * m._32))));
	det -= (m._14 * (m._21 * (m._32 * m._43 - (m._42 * m._33)) - m._22 * (m._31 * m._43 - (m._41 * m._33)) + m._23 * (m._31 * m._42 - (m._41 * m._32))));
	return det;
}

inline Matrix3 Adjoint(const Matrix3& m)
{
	return Matrix3
	(
		(m._22 * m._33 - m._23 * m._32),
		-(m._12 * m._33 - m._13 * m._32),
		(m._12 * m._23 - m._13 * m._22),

		-(m._21 * m._33 - m._23 * m._31),
		(m._11 * m._33 - m._13 * m._31),
		-(m._11 * m._23 - m._13 * m._21),

		(m._21 * m._32 - m._22 * m._31),
		-(m._11 * m._32 - m._12 * m._31), 
		(m._11 * m._22 - m._12 * m._21)
	);
}

inline Matrix4 Adjoint(const Matrix4& m)
{
	return Matrix4
	(
		 (m._22 * ((m._33 * m._44) - (m._43 * m._34)) - m._23 * ((m._32 * m._44) - (m._42 * m._34)) + m._24 * ((m._32 * m._43) - (m._42 * m._33))),
		-(m._12 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._32 * m._44) - (m._42 * m._34)) + m._14 * ((m._32 * m._43) - (m._42 * m._33))),
		 (m._12 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._22 * m._44) - (m._42 * m._24)) + m._14 * ((m._22 * m._43) - (m._42 * m._23))),
		-(m._12 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._22 * m._34) - (m._32 * m._24)) + m._14 * ((m._22 * m._33) - (m._32 * m._23))),

		-(m._21 * ((m._33 * m._44) - (m._43 * m._34)) - m._31 * ((m._23 * m._44) - (m._24 * m._43)) + m._41 * ((m._23 * m._34) - (m._24 * m._33))),
		 (m._11 * ((m._33 * m._44) - (m._43 * m._34)) - m._13 * ((m._31 * m._44) - (m._41 * m._34)) + m._14 * ((m._31 * m._43) - (m._41 * m._33))),
		-(m._11 * ((m._23 * m._44) - (m._43 * m._24)) - m._13 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._43) - (m._41 * m._23))),
		 (m._11 * ((m._23 * m._34) - (m._33 * m._24)) - m._13 * ((m._21 * m._34) - (m._31 * m._24)) + m._14 * ((m._21 * m._33) - (m._31 * m._23))),

		 (m._21 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._22 * m._44) - (m._42 * m._24)) + m._41 * ((m._22 * m._34) - (m._32 * m._24))),
		-(m._11 * ((m._32 * m._44) - (m._42 * m._34)) - m._31 * ((m._12 * m._44) - (m._42 * m._14)) + m._41 * ((m._12 * m._34) - (m._32 * m._14))),
		 (m._11 * ((m._22 * m._44) - (m._42 * m._24)) - m._12 * ((m._21 * m._44) - (m._41 * m._24)) + m._14 * ((m._21 * m._42) - (m._41 * m._22))),
		-(m._11 * ((m._22 * m._34) - (m._32 * m._24)) - m._21 * ((m._12 * m._34) - (m._32 * m._14)) + m._31 * ((m._12 * m._24) - (m._22 * m._14))),

		-(m._21 * ((m._32 * m._43) - (m._42 * m._33)) - m._31 * ((m._22 * m._43) - (m._42 * m._23)) + m._41 * ((m._22 * m._33) - (m._32 * m._23))),
		 (m._11 * ((m._32 * m._43) - (m._42 * m._33)) - m._12 * ((m._31 * m._43) - (m._41 * m._33)) + m._13 * ((m._31 * m._42) - (m._41 * m._32))),
		-(m._11 * ((m._22 * m._43) - (m._42 * m._23)) - m._12 * ((m._21 * m._43) - (m._41 * m._23)) + m._13 * ((m._21 * m._42) - (m._41 * m._22))),
		 (m._11 * ((m._22 * m._33) - (m._32 * m._23)) - m._12 * ((m._21 * m._33) - (m._31 * m._23)) + m._13 * ((m._21 * m._32) - (m._31 * m._22)))
	);
}

inline Matrix3 Inverse(const Matrix3& m)
{
	const float determinant = Determinant(m);
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

inline Matrix4 Inverse(const Matrix4& m)
{
	const float determinant = Determinant(m);
	const float invDet = 1.0f / determinant;
	return Adjoint(m) * invDet;
}

inline Matrix3 Transpose(const Matrix3& m)
{
	return Matrix3
	(
		m._11, m._21, m._31,
		m._12, m._22, m._32,
		m._13, m._23, m._33
	);
}

inline Matrix4 Transpose(const Matrix4& m)
{
	return Matrix4
	(
		m._11, m._21, m._31, m._41,
		m._12, m._22, m._32, m._42,
		m._13, m._23, m._33, m._43,
		m._14, m._24, m._34, m._44
	);
}

inline Vector2 TransformCoord(const Vector2& v, const Matrix3& m)
{
	return Vector2
	(
		v.x * m._11 + v.y * m._21 + m._31,
		v.x * m._12 + v.y * m._22 + m._32
	);
}

inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41,
		v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42,
		v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43
	);
}

inline Vector2 TransformNormal(const Vector2& v, const Matrix3& m)
{
	return Vector2
	(
		v.x * m._11 + v.y * m._21,
		v.x * m._12 + v.y * m._22
	);
}

inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
{
	return Vector3
	(
		v.x * m._11 + v.y * m._21 + v.z * m._31,
		v.x * m._12 + v.y * m._22 + v.z * m._32,
		v.x * m._13 + v.y * m._23 + v.z * m._33
	);
}

Quaternion QuaternionLookRotation(const Vector3& forward, const Vector3& up);
Quaternion QuaternionRotationAxis(const Vector3& axis, float rad);
Matrix4 MatrixRotationAxis(const Vector3& axis, float rad);
Matrix4 MatrixRotationQuaternion(const Quaternion& q);

Quaternion Slerp(Quaternion q0, Quaternion q1, float t);

X::Math::Vector3 GetBarycentric(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& point);

bool PointInTriangle(const Vector2& point, const Vector2& a, const Vector2& b, const Vector2& c);
bool PointInRect(const Vector2& point, const Rect& rect);
bool PointInCircle(const Vector2& point, const Circle& circle);

bool Intersect(const LineSegment& a, const LineSegment& b);
bool Intersect(const Circle& c0, const Circle& c1);
bool Intersect(const Rect& r0, const Rect& r1);

bool Intersect(const LineSegment& l, const Circle& c);
bool Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint = nullptr);

bool Intersect(const Circle& c, const Rect& r);
bool Intersect(const Rect& r, const Circle& c);

bool Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance);
bool Intersect(const Ray& ray, const Plane& plane, float& distance);
bool Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit);
bool Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit);
bool Intersect(const Vector3& point, const AABB& aabb);
bool Intersect(const Vector3& point, const OBB& obb);

void GetCorners(const OBB& obb, std::vector<Vector3>& corners);
bool GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal);

Vector3 GetClosestPoint(const Ray& ray, const Vector3& point);

Vector3 Mean(const Vector3* v, uint32_t count);

} // namespace Math
} // namespace X

#endif // #ifndef INCLUDED_XENGINE_MATH_H
