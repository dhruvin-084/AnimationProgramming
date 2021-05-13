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
inline vec3 operator+(const vec3& l, const vec3& r);
inline vec3 operator-(const vec3& l, const vec3& r);
inline vec3 operator*(const vec3& l, const vec3& r);
inline vec3 operator*(const vec3& l, float f);
inline float dot(const vec3& l, const vec3& r);


// Non Component Wise operations
inline float lenSq(const vec3& v);
inline float len(const vec3& v);
inline void normailze(vec3& v);
inline vec3 normalized(const vec3& v);
inline float angle(const vec3& l, const vec3& r);
inline vec3 project(const vec3& a, const vec3& b);
inline vec3 reject(const vec3& a, const vec3& b);
inline vec3 reflect(const vec3& a, const vec3& b);
inline vec3 cross(const vec3& l, const vec3& r);

// Interpolation of vactors
inline vec3 lerp(const vec3& s, const vec3& e, float t);
inline vec3 slerp(const vec3& s, const vec3& e, float t);
inline vec3 nlerp(const vec3& s, const vec3& e, float t);

// Comparison operators
bool operator==(const vec3& l, const vec3& r);
bool operator!=(const vec3& l, const vec3& r);