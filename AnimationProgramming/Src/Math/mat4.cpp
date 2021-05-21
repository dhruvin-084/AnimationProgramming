#include"mat4.h"
#include<iostream>

// Comarison operators
bool operator==(const mat4& a, const mat4& b) {
	for (int i = 0; i < 16; i++) {
		if (fabsf(a.v[i] - b.v[i]) > MAT4_EPSILON)
			return false;
	}
	return true;
}
bool operator!=(const mat4& a, const mat4& b) {
	return !(a == b);
}

// Addition
mat4 operator+(const mat4& a, const mat4& b) {
	return mat4(
		a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
		a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
		a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
		a.tx + b.tx, a.ty + b.ty, a.tz + b.tz, a.tw + b.tw
	);
}
// Multiplicaton with scaler
mat4 operator*(const mat4& a, float f) {
	return mat4(
		a.xx * f, a.xy * f, a.xz * f, a.xw * f,
		a.yx * f, a.yy * f, a.yz * f, a.yw * f,
		a.zx * f, a.zy * f, a.zz * f, a.zw * f,
		a.tx * f, a.ty * f, a.tz * f, a.tw * f
	);
}

// Multiplication with matrix
#define M4D(aRow, bCol) \
	a.v[0 * 4 + aRow] * b.v[bCol * 4 + 0] + \
	a.v[1 * 4 + aRow] * b.v[bCol * 4 + 1] + \
	a.v[2 * 4 + aRow] * b.v[bCol * 4 + 2] + \
	a.v[3 * 4 + aRow] * b.v[bCol * 4 + 3]
mat4 operator*(const mat4& a, const mat4& b) {
	return mat4(
		M4D(0, 0), M4D(1, 0), M4D(2, 0), M4D(3, 0),
		M4D(0, 1), M4D(1, 1), M4D(2, 1), M4D(3, 1),
		M4D(0, 2), M4D(1, 2), M4D(2, 2), M4D(3, 2),
		M4D(0, 3), M4D(1, 3), M4D(2, 3), M4D(3, 3)
	);
}

// Multiplication with vector
#define M4V4D(mRow, x, y, z, w) \
	x * m.v[0 * 4 + mRow] + \
	y * m.v[1 * 4 + mRow] + \
	z * m.v[2 * 4 + mRow] + \
	w * m.v[3 * 4 + mRow]
vec4 operator*(const mat4& m, const vec4& v) {
	return vec4(
		M4V4D(0, v.x, v.y, v.z, v.w),
		M4V4D(1, v.x, v.y, v.z, v.w),
		M4V4D(2, v.x, v.y, v.z, v.w),
		M4V4D(3, v.x, v.y, v.z, v.w)
	);
}

// Transform the vec3 using mat4
vec3 transformVector(const mat4& m, const vec3& v) {
	return vec3(
		M4V4D(0, v.x, v.y, v.z, 0.0f),
		M4V4D(1, v.x, v.y, v.z, 0.0f),
		M4V4D(2, v.x, v.y, v.z, 0.0f)
	);
}
vec3 transformPoint(const mat4& m, const vec3& v) {
	return vec3(
		M4V4D(0, v.x, v.y, v.z, 1.0f),
		M4V4D(1, v.x, v.y, v.z, 1.0f),
		M4V4D(2, v.x, v.y, v.z, 1.0f)
	);
}
vec3 transformPoint(const mat4& m, const vec3& v, float& w) {
	float _w = w;
	w = M4V4D(3, v.x, v.y, v.z, _w);
	return vec3(
		M4V4D(0, v.x, v.y, v.z, _w),
		M4V4D(1, v.x, v.y, v.z, _w),
		M4V4D(2, v.x, v.y, v.z, _w)
	);
}

// Transpose of mat4
#define M4SWAP(x, y) \
	{float t = x; x = y; y = t;}
void transpose(mat4& m) {
	M4SWAP(m.xy, m.yx);
	M4SWAP(m.xz, m.zx);
	M4SWAP(m.xw, m.tx);
	M4SWAP(m.yz, m.zy);
	M4SWAP(m.yw, m.ty);
	M4SWAP(m.zw, m.tz);
}
mat4 transposed(const mat4& m) {
	return mat4(
		m.xx, m.yx, m.zx, m.tx,
		m.xy, m.yy, m.zy, m.ty,
		m.xz, m.yz, m.zz, m.tz,
		m.xw, m.yw, m.zw, m.tw
	);
}

// Find inverse and determinant of mat4
#define M4_3X3MINOR(x, c0, c1, c2, r0, r1, r2) \
	(x[c0*4+r0]*(x[c1*4+r1]*x[c2*4+r2]-x[c1*4+r2]* x[c2*4+r1]) \
	-x[c1*4+r0]*(x[c0*4+r1]*x[c2*4+r2]- x[c0*4+r2]*x[c2*4+r1]) \
	+x[c2*4+r0]*(x[c0*4+r1]*x[c1*4+r2]-x[c0*4+r2]*x[c1*4+r1]))

float diterminant(const mat4& m) {
	return m.v[0] * M4_3X3MINOR(m.v, 1, 2, 3, 1, 2, 3)
		  -m.v[4] * M4_3X3MINOR(m.v, 0, 2, 3, 1, 2, 3)
		  +m.v[8] * M4_3X3MINOR(m.v, 0, 1, 3, 1, 2, 3)
		  -m.v[12]* M4_3X3MINOR(m.v, 0, 1, 2, 1, 2, 3);
}
mat4 adjugate(const mat4& m) {
	mat4 cf; // cofactor
	cf.v[0]  =  M4_3X3MINOR(m.v, 1, 2, 3, 1, 2, 3);cf.v[1]  = -M4_3X3MINOR(m.v, 1, 2, 3, 0, 2, 3);cf.v[2]  =  M4_3X3MINOR(m.v, 1, 2, 3, 0, 1, 3);cf.v[3]  = -M4_3X3MINOR(m.v, 1, 2, 3, 0, 1, 2);
	cf.v[4]  = -M4_3X3MINOR(m.v, 0, 2, 3, 1, 2, 3);cf.v[5]  =  M4_3X3MINOR(m.v, 0, 2, 3, 0, 2, 3);cf.v[6]  = -M4_3X3MINOR(m.v, 0, 2, 3, 0, 1, 3);cf.v[7]  =  M4_3X3MINOR(m.v, 0, 2, 3, 0, 1, 2);
	cf.v[8]  =  M4_3X3MINOR(m.v, 0, 1, 3, 1, 2, 3);cf.v[9]  = -M4_3X3MINOR(m.v, 0, 1, 3, 0, 2, 3);cf.v[10] =  M4_3X3MINOR(m.v, 0, 1, 3, 0, 1, 3);cf.v[11] = -M4_3X3MINOR(m.v, 0, 1, 3, 0, 1, 2);
	cf.v[12] = -M4_3X3MINOR(m.v, 0, 1, 2, 1, 2, 3);cf.v[13] =  M4_3X3MINOR(m.v, 0, 1, 2, 0, 2, 3);cf.v[14] = -M4_3X3MINOR(m.v, 0, 1, 2, 0, 1, 3);cf.v[15] =  M4_3X3MINOR(m.v, 0, 1, 2, 0, 1, 2);
	return transposed(cf);
}
mat4 inverse(const mat4& m) {
	float det = diterminant(m);
	if (det < MAT4_EPSILON) {
		std::cout << "Matrix determinant is 0 \n";
		return mat4();
	}
	mat4 adj = adjugate(m);
	return adj * (1.0f / det);
}
void invert(mat4& m) {
	float det = diterminant(m);
	if (det < MAT4_EPSILON) {
		std::cout << "Matrix determinant is 0 \n";
		return;
	}
	
	m = adjugate(m) * (1.0f / det);
}


// Camera stuff
mat4 frustum(float l, float r, float b, float t, float n, float f) {
	if (fabsf(l - r) < MAT4_EPSILON || fabsf(b - t) < MAT4_EPSILON || fabsf(n - f) < MAT4_EPSILON) {
		std::cout << "Invalid furstum \n";
		return mat4();
	}
	return mat4(
		(2.0f * n) / (r - 1),  0,                     0,                        0,
		0,                     (2.0f * n) / (t - b),  0,                        0,
		(r + l) / (r - l),     (t + b) / (t - b),     (n + f) / (n - f),       -1,
		0,                     0,                     (2.0f * n * f) / (n - f), 0
	);
}

mat4 perspective(float fov, float aspect, float n, float f) {
	float ymax = n * tanf(fov * 3.1415926536f / 360.0f);
	float xmax = ymax * aspect;
	return frustum(-xmax, xmax, -ymax, ymax, n, f);
}

mat4 ortho(float l, float r, float b, float t, float n, float f) {
	if (fabsf(l - r) < MAT4_EPSILON || fabsf(b - t) < MAT4_EPSILON || fabsf(n - f) < MAT4_EPSILON) {
		std::cout << "Invalid furstum \n";
		return mat4();
	}
	return mat4(
		2.0f / (r - l),  0.0f,            0.0f,          0.0f,
		0.0f,            2.0f / (t - b),  0.0f,          0.0f,
		0.0f,            0.0f,            2.0f / (n-f),  0.0f,
		(l+r) / (l-r),   (b+t) / (b-t),   (n+f)/(n-f),   1.0f
		);
}

mat4 lookAt(const vec3& position, const vec3& target, const vec3& up) {
	vec3 f = normalized(target - position) * (-1.0f);
	vec3 r = cross(up, f);
	if (r == vec3(0.0f, 0.0f, 0.0f))
		return mat4();
	normalize(r);

	vec3 u = normalized(cross(f, r));
	vec3 t = vec3(
		-dot(r, position),
		-dot(u, position),
		-dot(f, position)
	);

	return mat4(
		// Transpose upper 3x3 matrix to invert it
		r.x, u.x, f.x, 0,
		r.y, u.y, f.y, 0,
		r.z, u.z, f.z, 0,
		t.x, t.y, t.z, 1
	);
}