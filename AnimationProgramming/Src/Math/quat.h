#pragma once
#include"vec3.h"
#include"mat4.h"

#define QUAT_EPSILON 0.000001f

struct quat {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			vec3 vector;
			float scalar;
		};
		float v[4];
	};

	inline quat() : x(0), y(0), z(0), w(1) {}
	inline quat(float _x, float _y, float _z, float _w) :
		x(_x), y(_y), z(_z), w(_w) {}
};

// Create quaternions
quat axisAngle(float angle, const vec3& axis);
quat fromTo(const vec3& from, const vec3& to);

// Get data of quaternions
vec3 getAxis(const quat& q);
float getAngle(const quat& q);

// Operations on quaternions
quat operator+(const quat& a, const quat& b);
quat operator-(const quat& a, const quat& b);
quat operator-(const quat& a);
quat operator*(const quat& a, float f);
quat operator*(const quat& Q1, const quat& Q2);
vec3 operator*(const quat& q, const vec3& v);

// Comarison operators
bool operator==(const quat& a, const quat& b);
bool operator!=(const quat& a, const quat& b);

// Component wise
float dot(const quat& a, const quat& b);
float lenSq(const quat& a);
float len(const quat& a);

void normalize(quat& a);
quat normalized(const quat& a);

quat conjugate(const quat& a);
quat inverse(const quat& a);
