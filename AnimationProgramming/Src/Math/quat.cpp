#include"quat.h"
#include<cmath>

quat axisAngle(float angle, const vec3& axis) {
	vec3 norm = normalized(axis);
	float s = sinf(angle * 0.5f);
	return quat(
		norm.x * s,
		norm.y * s,
		norm.z * s,
		cosf(angle * 0.5)
	);
}
quat fromTo(const vec3& from, const vec3& to) {
	vec3 f = normalized(from);
	vec3 t = normalized(to);
	if (f == t)
		return quat();
	else if (f == t * -1.0f) {
		vec3 ortho = vec3(1, 0, 0);
		if (fabsf(f.y) < fabsf(f.x))
			ortho = vec3(0, 1, 0);
		if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) 
			ortho = vec3(0, 0, 1);
		vec3 axis = normalized(cross(f, ortho));
		return quat(axis.x, axis.y, axis.z, 0);
	}

	vec3 half = normalized(f + t);
	vec3 axis = cross(f, half);
	return quat(axis.x, axis.y, axis.z, dot(f, half));
}

// Get data of quaternions
vec3 getAxis(const quat& q) {
	return normalized(vec3(q.x, q.y, q.z));
}
float getAngle(const quat& q) {
	return 2.0f * acosf(q.w);
}

// Operations on quaternions
quat operator+(const quat& a, const quat& b) {
	return quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
quat operator-(const quat& a, const quat& b) {
	return quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
quat operator-(const quat& a) {
	return quat(-a.x, -a.y, -a.z, -a.w);
}
quat operator*(const quat& a, float f) {
	return quat(a.x * f, a.y * f, a.z * f, a.w * f);
}
quat operator*(const quat& Q1, const quat& Q2) {
	return quat(
		 Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y + Q2.w * Q1.x,
		-Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x + Q2.w * Q1.y,
		 Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w + Q2.w * Q1.z,
		-Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z + Q2.w * Q1.w
	); 
	/* Alternative Implemantation
	quat result;
	result.scalar = Q2.scalar * Q1.scalar - dot(Q2.vector, Q1.vector);
	result.vector = (Q1.vector * Q2.scalar) + (Q2.vector * Q1.scalar) + cross(Q2.vector, Q1.vector);
	return result;
	*/
}
vec3 operator*(const quat& q, const vec3& v) {
	return q.vector * (2.0f * dot(q.vector, v)) +
		v * (q.scalar * q.scalar - dot(q.vector, q.vector)) +
		cross(q.vector, v) * (2.0f * q.scalar);
}
quat operator^(const quat& q, float f) {
	float angle = 2.0f * acosf(q.scalar);
	vec3 axis = normalized(q.vector);

	float halfsin = sinf(f * angle * 0.5f);
	float halfcos = cosf(f * angle * 0.5f);

	return quat(
		axis.x * halfsin,
		axis.y * halfsin,
		axis.z * halfsin,
		halfcos
	);
}

// Comarison operators
bool operator==(const quat& a, const quat& b) {
	return (
		fabsf(a.x - b.x) <= QUAT_EPSILON &&
		fabsf(a.y - b.y) <= QUAT_EPSILON &&
		fabsf(a.z - b.z) <= QUAT_EPSILON &&
		fabsf(a.w - b.w) <= QUAT_EPSILON
		);
}
bool operator!=(const quat& a, const quat& b) {
	return !(a == b);
}
bool sameOrientation(const quat& a, const quat& b) {
	return(
		(fabsf(a.x - b.x) <= QUAT_EPSILON &&
		 fabsf(a.y - b.y) <= QUAT_EPSILON &&
		 fabsf(a.z - b.z) <= QUAT_EPSILON &&
		 fabsf(a.w - b.w) <= QUAT_EPSILON) ||
		(fabsf(a.x + b.x) <= QUAT_EPSILON &&
		 fabsf(a.y + b.y) <= QUAT_EPSILON &&
		 fabsf(a.z + b.z) <= QUAT_EPSILON &&
		 fabsf(a.w + b.w) <= QUAT_EPSILON)
		);
}


// Component wise
float dot(const quat& a, const quat& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
float lenSq(const quat& a) {
	return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
}
float len(const quat& a){
	float lenSq = a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	if (lenSq < QUAT_EPSILON)
		return 0.0f;
	return sqrtf(lenSq);
}

void normalize(quat& a) {
	float lenSq = a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	if (lenSq < QUAT_EPSILON)
		return;
	float inv = 1.0f / sqrtf(lenSq);
	a.x *= inv;
	a.y *= inv;
	a.z *= inv;
	a.w *= inv;
}
quat normalized(const quat& a) {
	float lenSq = a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	if (lenSq < QUAT_EPSILON)
		return quat();
	float inv = 1.0f / sqrtf(lenSq);
	return quat(a.x * inv, a.y * inv, a.z * inv, a.w * inv);
}

quat conjugate(const quat& a) {
	return quat(-a.x, -a.y, -a.z, a.w);
}
quat inverse(const quat& a) {
	float lenSq = a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w;
	if (lenSq < QUAT_EPSILON)
		return quat();
	float inv = 1.0f / lenSq;
	return quat(-a.x * inv, -a.y * inv, -a.z * inv, a.w * inv);
}


// Interpolation functions
quat mix(const quat& from, const quat& to, float t) {
	return from * (1.0f - t) + to * t;
}
quat nlerp(const quat& from, const quat& to, float t) {
	return normalized(from * (1.0f - t) + to * t);
}
quat slerp(const quat& from, const quat& to, float t) {
	if (fabsf(dot(from, to) > (1.0f - QUAT_EPSILON)))
		return nlerp(from, to, t);

	quat delta = inverse(from) * to;
	return normalized((delta ^ t) * to);
}

quat lookRotation(const vec3& direction, const vec3& up) {
	vec3 f = normalized(direction); // object forward vector
	vec3 u = normalized(up); // desired up vector
	vec3 r = cross(u, f); // objcet right vector
	u = cross(f, r); // object up vector

	// from world forward to objcet forward
	quat wroldToObject = fromTo(vec3(0, 0, 1), f);

	// new object up
	vec3 objUp = wroldToObject * vec3(0, 1, 0);
	// from object up to disred up
	quat u2u = fromTo(objUp, u);

	quat result = wroldToObject * u2u;

	return normalized(result);
}


mat4 quatToMat4(const quat& q) {
	vec3 r = q * vec3(1, 0, 0);
	vec3 u = q * vec3(0, 1, 0);
	vec3 f = q * vec3(0, 0, 1);

	return mat4(r.x, r.y, r.z, 0,
				u.x, u.y, u.z, 0,
				f.x, f.y, f.x, 0,
				0,   0,   0,   1);
}
quat mat4ToQuat(const mat4& m) {
	vec3 up = normalized(vec3(m.up.x, m.up.y, m.up.z));
	vec3 forward = normalized(vec3(m.forward.x, m.forward.y, m.forward.z));
	vec3 right = cross(up, forward);
	up = cross(forward, right);

	return lookRotation(forward, up);
}