//====================================================================================================
// Filename:	XMath.cpp
// Created by:	Peter Chan
//====================================================================================================

#include "Precompiled.h"
#include "XMath.h"

using namespace X;
using namespace X::Math;

//====================================================================================================
// Constants
//====================================================================================================

const float Math::kPi			= 3.14159265358979f;
const float Math::kTwoPi		= 6.28318530717958f;
const float Math::kPiByTwo		= 1.57079632679489f;
const float Math::kRootTwo		= 1.41421356237309f;
const float Math::kRootThree	= 1.73205080756887f;
const float Math::kDegToRad		= kPi / 180.0f;
const float Math::kRadToDeg		= 180.0f / kPi;

//====================================================================================================
// Function Definitions
//====================================================================================================

Quaternion X::Math::QuaternionLookRotation(const Vector3& forward, const Vector3& up)
{
	Vector3 z = Normalize(forward);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Normalize(Cross(z, x));
	Matrix4 mat
	(
		x.x, x.y, x.z, 0.0f,
		y.x, y.y, y.z, 0.0f,
		z.x, z.y, z.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// TODO: Need matrix to quaternion
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	return Quaternion();
}

//----------------------------------------------------------------------------------------------------

Quaternion X::Math::QuaternionRotationAxis(const Vector3& axis, float rad)
{
	const float c = cos(rad * 0.5f);
	const float s = sin(rad * 0.5f);
	const Math::Vector3 a = Math::Normalize(axis);
	return Quaternion(a.x * s, a.y * s, a.z * s, c);
}

//----------------------------------------------------------------------------------------------------

Matrix4 X::Math::MatrixRotationAxis(const Vector3& axis, float rad)
{
	const Vector3 u = Normalize(axis);
	const float x = u.x;
	const float y = u.y;
	const float z = u.z;
	const float s = sin(rad);
	const float c = cos(rad);

	return Matrix4
	(
		c + (x * x * (1.0f - c)),
		x * y * (1.0f - c) + z * s,
		x * z * (1.0f - c) - y * s,
		0.0f,

		x * y * (1.0f - c) - z * s,
		c + (y * y * (1.0f - c)),
		y * z * (1.0f - c) + x * s,
		0.0f,

		x * z * (1.0f - c) + y * s,
		y * z * (1.0f - c) - x * s,
		c + (z * z * (1.0f - c)),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Matrix4 X::Math::MatrixRotationQuaternion(const Quaternion& q)
{
	return Matrix4
	(
		1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z),
		(2.0f * q.x * q.y) + (2.0f * q.z * q.w),
		(2.0f * q.x * q.z) - (2.0f * q.y * q.w),
		0.0f,

		(2.0f * q.x * q.y) - (2.0f * q.z * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z),
		(2.0f * q.y * q.z) + (2.0f * q.x * q.w),
		0.0f,

		(2.0f * q.x * q.z) + (2.0f * q.y * q.w),
		(2.0f * q.y * q.z) - (2.0f * q.x * q.w),
		1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Quaternion X::Math::Slerp(Quaternion q0, Quaternion q1, float t)
{
	// Find the dot product
	float dot = (q0.x * q1.x) + (q0.y * q1.y) + (q0.z * q1.z) + (q0.w * q1.w);

	// Determine the direction of the rotation.
	if (dot < 0.0f)
	{
		dot = -dot;
		q1.x = -q1.x;
		q1.y = -q1.y;
		q1.z = -q1.z;
		q1.w = -q1.w;
	}
	else if (dot > 0.999f)
	{
		return Normalize(Lerp(q0, q1, t));
	}

	float theta = acosf(dot);
	float sintheta = sinf(theta);
	float scale0 = sinf(theta * (1.0f - t)) / sintheta;
	float scale1 = sinf(theta * t) / sintheta;

	// Perform the slerp
	return Quaternion
	(
		(q0.x * scale0) + (q1.x * scale1),
		(q0.y * scale0) + (q1.y * scale1),
		(q0.z * scale0) + (q1.z * scale1),
		(q0.w * scale0) + (q1.w * scale1)
	);
}

//----------------------------------------------------------------------------------------------------

X::Math::Vector3 X::Math::GetBarycentric(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& point)
{
	X::Math::Vector3 lambda;
	float denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
	lambda.x = ((b.y - c.y) * (point.x - c.x) + (c.x - b.x) * (point.y - c.y)) / denom;
	lambda.y = ((c.y - a.y) * (point.x - c.x) + (a.x - c.x) * (point.y - c.y)) / denom;
	lambda.z = 1.0f - lambda.x - lambda.y;
	return lambda;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::PointInTriangle(const Vector2& point, const Vector2& a, const Vector2& b, const Vector2& c)
{
	X::Math::Vector3 barycentric = GetBarycentric(a, b, c, point);
	return
		barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
		barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
		barycentric.z >= 0.0f && barycentric.z <= 1.0f;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::PointInRect(const Vector2& point, const Rect& rect)
{
	if (point.x > rect.right || point.x < rect.left ||
		point.y > rect.bottom || point.y < rect.top)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::PointInCircle(const Vector2& point, const Circle& circle)
{
	const Vector2 centerToPoint = point - circle.center;
	const float distSqr = Dot(centerToPoint, centerToPoint);
	const float radiusSqr = circle.radius * circle.radius;
	return distSqr < radiusSqr;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const LineSegment& a, const LineSegment& b)
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

	float ua = ((a.to.x - a.from.x) * (b.from.y - a.from.y)) - ((a.to.y - a.from.y) * (b.from.x - a.from.x));
	float ub = ((b.to.x - b.from.x) * (b.from.y - a.from.y)) - ((b.to.y - b.from.y) * (b.from.x - a.from.x));
	float denom = ((a.to.y - a.from.y) * (b.to.x - b.from.x)) - ((a.to.x - a.from.x) * (b.to.y - b.from.y));

	// First check for special cases
	if (denom == 0.0f)
	{
		if (ua == 0.0f && ub == 0.0f)
		{
			// The line segments are the same
			return true;
		}
		else
		{
			// The line segments are parallel
			return false;
		}
	}

	ua /= denom;
	ub /= denom;

	if (ua < 0.0f || ua > 1.0f || ub < 0.0f || ub > 1.0f)
	{
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Circle& c0, const Circle& c1)
{
	const float fRadii = c0.radius + c1.radius;
	const float fRadiiSquared = fRadii * fRadii;
	const float fDistanceSquared = DistanceSqr(c0.center, c1.center);
	return (fDistanceSquared < fRadiiSquared);
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Rect& r0, const Rect& r1)
{
	if (r0.left > r1.right)
		return false;
	else if (r0.top > r1.bottom)
		return false;
	else if (r0.right < r1.left)
		return false;
	else if (r0.bottom < r1.top)
		return false;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const LineSegment& l, const Circle& c)
{
	return Intersect(c, l);
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Circle& c, const LineSegment& l, Vector2* closestPoint)
{
	Vector2 startToCenter = c.center - l.from;
	Vector2 startToEnd = l.to - l.from;
	float len = Magnitude(startToEnd);
	Vector2 dir = startToEnd / len;

	// Find the closest point to the line segment
	float projection = Dot(startToCenter, dir);
	Vector2 point;
	if (projection > len)
	{
		point = l.to;
	}
	else if (projection < 0.0f)
	{
		point = l.from;
	}
	else
	{
		point = l.from + (dir * projection);
	}

	// Check if the closest point is within the circle
	if (!PointInCircle(point, c))
		return false;

	if (closestPoint)
		*closestPoint = point;

	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Circle& c, const Rect& r)
{
	return Intersect(r, c);
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Rect& r, const Circle& c)
{
	Vector2 closestPoint;
	closestPoint.x = Clamp(c.center.x, r.left, r.right);
	closestPoint.y = Clamp(c.center.y, r.top, r.bottom);

	const float distance = Distance(closestPoint, c.center);
	if (distance > c.radius)
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Ray& ray, const Vector3& a, const Vector3& b, const Vector3& c, float& distance)
{
	// Reference: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	// Find vectors for two edges sharing V1
	Vector3 e1 = b - a;
	Vector3 e2 = c - a;

	// Begin calculating determinant - also used to calculate u parameter
	Vector3 P = Cross(ray.dir, e2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = Dot(e1, P);
	// NOT CULLING
	if (IsZero(det))
	{
		return false;
	}

	float inv_det = 1.0f / det;

	// Calculate distance from V1 to ray origin
	Vector3 T = ray.org - a;

	// Calculate u parameter and test bound
	float u = Dot(T, P) * inv_det;

	// The intersection lies outside of the triangle
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	// Prepare to test v parameter
	Vector3 Q = Cross(T, e1);

	// Calculate V parameter and test bound
	float v = Dot(ray.dir, Q) * inv_det;

	// The intersection lies outside of the triangle
	if (v < 0.0f || u + v  > 1.0f)
	{
		return false;
	}

	// Ray intersection
	float t = Dot(e2, Q) * inv_det;
	if (t <= 0.0f)
	{ 
		// No hit, no win
		return false;
	}

	distance = t;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Ray& ray, const Plane& plane, float& distance)
{
	const float orgDotN = Dot(ray.org, plane.n);
	const float dirDotN = Dot(ray.dir, plane.n);

	// Check if ray is parallel to the plane
	if (IsZero(dirDotN))
	{
		if (IsZero(orgDotN - plane.d))
		{
			distance = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Compute distance
	distance = (plane.d - orgDotN) / dirDotN;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Ray& ray, const AABB& aabb, float& distEntry, float& distExit)
{
	// https://truesculpt.googlecode.com/hg-history/Release%25200.8/Doc/ray_box_intersect.pdf

	// Returns true if ray intersects bounding box
	// Sets d1 to entry distance, d2 to exit distance (along ray.dir)

	Vector3 boxMin = aabb.center - aabb.extend;
	Vector3 boxMax = aabb.center + aabb.extend;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.dir.x;
	float divy = 1.0f / ray.dir.y;
	float divz = 1.0f / ray.dir.z;

	if (divx >= 0.0f)
	{
		tmin = (boxMin.x - ray.org.x) * divx;
		tmax = (boxMax.x - ray.org.x) * divx;
	}
	else
	{
		tmin = (boxMax.x - ray.org.x) * divx;
		tmax = (boxMin.x - ray.org.x) * divx;
	}
	if (divy >= 0.0f)
	{
		tymin = (boxMin.y - ray.org.y) * divy;
		tymax = (boxMax.y - ray.org.y) * divy;
	}
	else
	{
		tymin = (boxMax.y - ray.org.y) * divy;
		tymax = (boxMin.y - ray.org.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	if (divz >= 0.0f)
	{
		tzmin = (boxMin.z - ray.org.z) * divz;
		tzmax = (boxMax.z - ray.org.z) * divz;
	}
	else
	{
		tzmin = (boxMax.z - ray.org.z) * divz;
		tzmax = (boxMin.z - ray.org.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}

	distEntry = tmin;
	distExit = tmax;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Ray& ray, const OBB& obb, float& distEntry, float& distExit)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);

	AABB aabb(Vector3::Zero(), obb.extend);
	return Math::Intersect(Ray(org, dir), aabb, distEntry, distExit);
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Vector3& point, const AABB& aabb)
{
	const Vector3 test = point - aabb.center;
	if (abs(test.x) > aabb.extend.x) return false;
	if (abs(test.y) > aabb.extend.y) return false;
	if (abs(test.z) > aabb.extend.z) return false;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool X::Math::Intersect(const Vector3& point, const OBB& obb)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the point into the OBB's local space
	Vector3 localPoint = TransformCoord(point, matWorldInv);
	AABB aabb(Vector3::Zero(), obb.extend);

	// Test against local AABB
	return Math::Intersect(localPoint, aabb);
}

//----------------------------------------------------------------------------------------------------

void X::Math::GetCorners(const OBB& obb, std::vector<Vector3>& corners)
{
	// Compute the local-to-world matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;

	// Create a local AABB
	corners.clear();
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x, -obb.extend.y,  obb.extend.z));
	corners.push_back(Vector3( obb.extend.x, -obb.extend.y,  obb.extend.z));
	corners.push_back(Vector3( obb.extend.x, -obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x,  obb.extend.y, -obb.extend.z));
	corners.push_back(Vector3(-obb.extend.x,  obb.extend.y,  obb.extend.z));
	corners.push_back(Vector3( obb.extend.x,  obb.extend.y,  obb.extend.z));
	corners.push_back(Vector3( obb.extend.x,  obb.extend.y, -obb.extend.z));

	// Transform AABB into world space to form the OBB
	const uint32_t kNumCorners = (uint32_t)corners.size();
	for (uint32_t i = 0; i < kNumCorners; ++i)
	{
		corners[i] = TransformCoord(corners[i], matWorld);
	}
}

//----------------------------------------------------------------------------------------------------

bool X::Math::GetContactPoint(const Ray& ray, const OBB& obb, Vector3& point, Vector3& normal)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix4 matTrans = Matrix4::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix4 matRot = MatrixRotationQuaternion(obb.orientation);
	Matrix4 matWorld = matRot * matTrans;
	Matrix4 matWorldInv = Inverse(matWorld);

	// Transform the ray into the OBB's local space
	Vector3 org = TransformCoord(ray.org, matWorldInv);
	Vector3 dir = TransformNormal(ray.dir, matWorldInv);
	Ray localRay(org, dir);

	Plane planes[] =
	{
		Plane( 0.0f,  0.0f, -1.0f, obb.extend.z),
		Plane( 0.0f,  0.0f,  1.0f, obb.extend.z),
		Plane( 0.0f, -1.0f,  0.0f, obb.extend.y),
		Plane( 0.0f,  1.0f,  0.0f, obb.extend.y),
		Plane(-1.0f,  0.0f,  0.0f, obb.extend.x),
		Plane( 1.0f,  0.0f,  0.0f, obb.extend.x)
	};

	uint32_t numIntersections = 0;
	for (uint32_t i = 0; i < 6; ++i)
	{
		const float d = Dot(org, planes[i].n);
		if (d > planes[i].d)
		{
			float distance = 0.0f;
			if (Intersect(localRay, planes[i], distance) && distance >= 0.0f)
			{
				Vector3 pt = org + (dir * distance);
				if (abs(pt.x) <= obb.extend.x + 0.01f &&
					abs(pt.y) <= obb.extend.y + 0.01f &&
					abs(pt.z) <= obb.extend.z + 0.01f)
				{
					point = pt;
					normal = planes[i].n;
					++numIntersections;
				}
			}
		}
	}

	if (numIntersections == 0)
	{
		return false;
	}

	point = TransformCoord(point, matWorld);
	normal = TransformNormal(normal, matWorld);
	return true;
}

//----------------------------------------------------------------------------------------------------

Vector3 X::Math::GetClosestPoint(const Ray& ray, const Vector3& point)
{
	Vector3 orgToPoint = point - ray.org;
	float d = Dot(orgToPoint, ray.dir);
	return ray.org + (ray.dir * d);
}

//----------------------------------------------------------------------------------------------------

Vector3 X::Math::Mean(const Vector3* v, uint32_t count)
{
	Vector3 mean(0.0f, 0.0f, 0.0f);
	for (uint32_t i = 0; i < count; ++i)
	{
		mean += v[i];
	}
	return mean / (float)count;
}