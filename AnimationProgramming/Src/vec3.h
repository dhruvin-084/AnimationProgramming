#pragma once

#include<cmath>

#define VEC3_EPSILON 0.000001f

struct vec3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	};

	inline vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	inline vec3(float* fv) : x(fv[0]), y(fv[1]), z(fv[2]) {}
};


// Component Wise operations
inline vec3 operator+(const vec3& l, const vec3& r) {
	return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}
inline vec3 operator-(const vec3& l, const vec3& r) {
	return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}
inline vec3 operator*(const vec3& l, const vec3& r) {
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}
inline vec3 operator*(const vec3& l, float f) {
	return vec3(l.x * f, l.y * f, l.z * f);
}
inline float dot(const vec3& l, const vec3& r) {
	return (l.x * r.x + l.y * r.y + l.z * r.z);
}


// Non Component Wise operations
inline float lenSq(const vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
inline float len(const vec3& v) {
	float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lenSq < VEC3_EPSILON)
		return 0.0f;

	return sqrtf(lenSq);
}
inline void normailze(vec3& v) {
	float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lenSq < VEC3_EPSILON)
		return;
	float inv = 1.0f / sqrtf(lenSq);
	v.x *= inv;
	v.y *= inv;
	v.z *= inv;
}
inline vec3 normalized(const vec3& v) {
	float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (lenSq < VEC3_EPSILON)
		return v;
	float inv = 1.0f / sqrtf(lenSq);
	return vec3(
		v.x * inv,
		v.y * inv,
		v.z * inv
	);
}
inline float angle(const vec3& l, const vec3& r) {
	float lenSql = l.x * l.x + l.y * l.y + l.z * l.z;
	float lenSqr = r.x * r.x + r.y * r.y + r.z * r.z;

	if (lenSql < VEC3_EPSILON || lenSqr < VEC3_EPSILON)
		return 0.0f;
	float dot = l.x * r.x + l.y * r.y + l.z * r.z;
	float len = sqrtf(lenSql * lenSqr);
	return acosf(dot / len);
}
inline vec3 project(const vec3& a, const vec3& b) {
	float magB = len(b);
	if (magB < VEC3_EPSILON)
		return vec3();
	float scale = dot(a, b) / magB;
	return b * scale;
}
inline vec3 reject(const vec3& a, const vec3& b) {
	vec3 projection = project(a, b);
	return a - projection;
}
inline vec3 reflect(const vec3& a, const vec3& b) {
	float magB = len(b);
	if (magB < VEC3_EPSILON)
		return vec3();
	float scale = dot(a, b) / magB;
	vec3 proj2 = b * (scale * 2);
	return a - proj2;
}
inline vec3 cross(const vec3& l, const vec3& r) {
	return vec3(
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	);
}

// Interpolation of vactors
inline vec3 lerp(const vec3& s, const vec3& e, float t) {
	return vec3(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
}
inline vec3 slerp(const vec3& s, const vec3& e, float t) {
	if (t < 0.1f)
		return lerp(s, e, t);
	vec3 from = normalized(s);
	vec3 to = normalized(e);

	float theta = angle(from, to);
	float sin_theta = sinf(theta);

	float a = sinf((1 - t) * theta) / sin_theta;
	float b = sinf(t * theta) / sin_theta;

	return from * a + to * b;
}
inline vec3 nlerp(const vec3& s, const vec3& e, float t) {
	vec3 linear(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
	return normalized(linear);
}

// Comparison operators
bool operator==(const vec3& l, const vec3& r) {
	vec3 diff(l - r);
	return lenSq(diff) < VEC3_EPSILON;
}
bool operator!=(const vec3& l, const vec3& r) {
	return !(l == r);
}