#ifndef _MATH3D_H
#define  _MATH3D_H

#include "ork/math/vec3.h"
#include "ork/math/vec4.h"

#include "global.h"

#include <stdlib.h>
#include <math.h>

class Math3D
{
public:
	static int RandIntToInt(int lower, int upper)
	{
		//note:不包括上边界
		return lower + rand() % (upper - lower);
	}

	static float RandZeroToOne()
	{
		return ((float)rand()) / RAND_MAX;
	}

	static float RandFloatToFloat(float lower, float upper)
	{
		return RandZeroToOne() * (upper - lower) + lower;
	}

	static ork::vec3f RandVec3ToVec3(const ork::vec3f &lower, const ork::vec3f &upper)
	{
		return ork::vec3f(RandFloatToFloat(lower.x, upper.x),
			RandFloatToFloat(lower.y, upper.y),
			RandFloatToFloat(lower.z, upper.z));
	}

	static ork::vec3f Vec3Lerp(const ork::vec3f& lower, const ork::vec3f& upper, float t)
	{
		return lower * (1 - t) + upper * t;
	}

	static ork::vec4f Vec4Lerp(const ork::vec4f& lower, const ork::vec4f& upper, float t)
	{
		return lower * (1 - t) + upper * t;
	}

	static ork::vec3f CalcTangentVelocity(const ork::vec3f &posDir, const ork::vec3f &ellipsoid, const ork::vec3f &tangentVelocity)
	{
		if (posDir.length() < FLOAT_EQUAL_EPSILON || ellipsoid.length() < FLOAT_EQUAL_EPSILON || tangentVelocity.length() < FLOAT_EQUAL_EPSILON)
		{
			return ork::vec3f::ZERO;
		}

		//椭球上的法向
		ork::vec3f normal = ork::vec3f(IsFloatEqual(ellipsoid.x, 0) ? 0 : posDir.x / (ellipsoid.x * ellipsoid.x),
			IsFloatEqual(ellipsoid.y, 0) ? 0 : posDir.y / (ellipsoid.y * ellipsoid.y),
			IsFloatEqual(ellipsoid.z, 0) ? 0 : posDir.z / (ellipsoid.z * ellipsoid.z)).normalize();

		ork::vec3f axisY = ork::vec3f(-normal.z, 0, normal.x);
		if (axisY.length() < FLOAT_EQUAL_EPSILON)
		{
			axisY = ork::vec3f(0, -normal.z, normal.y).normalize();
		}

		ork::vec3f axisZ = normal.crossProduct(axisY).normalize();

		ork::vec3f tV = normal * tangentVelocity.z + axisY * tangentVelocity.x + axisZ * tangentVelocity.y;

		return RandZeroToOne() < 0.5 ? -tV : tV;
	}

	static ork::mat4f CalcModelMatrix(const ork::vec3f & translate, const ork::vec3f & rotate, const ork::vec3f & scale)
	{
		ork::mat4f scaleMatrix = ork::mat4f(scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1);

		return ork::mat4f::translate(translate) *
			ork::mat4f::rotatex(rotate.x) * ork::mat4f::rotatez(rotate.z) * ork::mat4f::rotatey(rotate.y) *
			scaleMatrix;
	}

	static bool IsVec4Equal(const ork::vec4f &lhs, const ork::vec4f &rhs)
	{
		return IsFloatEqual(lhs.x, rhs.x) && IsFloatEqual(lhs.y, rhs.y) && IsFloatEqual(lhs.z, rhs.z) && IsFloatEqual(lhs.w, rhs.w);
	}

	static bool IsFloatEqual(float lhs, float rhs)
	{
		return abs(lhs - rhs) < FLOAT_EQUAL_EPSILON;
	}

	//标准椭球分布(minEmitterRange为0：整个椭球 minEmitterRange为1：椭球壳)
	static ork::vec3f UniformEllipsoidSample(const ork::vec3f & ellipsoid, float minEmitterRange)
	{
		//先计算标准正球分布
		float a = Math3D::RandFloatToFloat(-180, 180) * ANGLE_TO_RADIAN_RATIO;
		float b = acos(2 * Math3D::RandZeroToOne() - 1);
		float r = pow(Math3D::RandZeroToOne() * (1 - pow(minEmitterRange, 3) + pow(minEmitterRange, 3)), 1 / 3);

		ork::vec3f sampleP;
		sampleP.x = r * sin(b) * cos(a);
		sampleP.y = r * sin(b) * sin(a);
		sampleP.z = r * cos(b);

		return sampleP * ellipsoid;
	}
};

#endif //_MATH3D_H