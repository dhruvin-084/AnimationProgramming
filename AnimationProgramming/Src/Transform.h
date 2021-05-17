#pragma once
#include"Math/vec3.h"
#include"Math/quat.h"

struct Transform {
	vec3 position;
	quat rotation;
	vec3 scale;

	Transform(const vec3& p, const quat& r, const vec3& s) :
		position(p), rotation(r), scale(s) {}

	Transform() :
		position(vec3(0, 0, 0)),
		rotation(quat(0, 0, 0, 1)),
		scale(vec3(1, 1, 1)) {}

};


Transform combine(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);


// Interpolations
Transform mix(const Transform& a, const Transform& b, float t);


mat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const mat4& m);


vec3 tranformPoint(const Transform& a, const vec3& b);
vec3 tranformVector(const Transform& a, const vec3& b);