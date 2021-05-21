#include<glad/glad.h>

#include"../Math/vec2.h"
#include"../Math/vec3.h"
#include"../Math/vec4.h"
#include"../Math/quat.h"
#include"../Math/mat4.h"

#include"Uniform.h"

template Uniform<int>;
template Uniform<ivec4>;
template Uniform<ivec2>;
template Uniform<float>;
template Uniform<vec2>;
template Uniform<vec3>;
template Uniform<vec4>;
template Uniform<quat>;
template Uniform<mat4>;

template <typename T>
void Uniform<T>::Set(unsigned int slot, const T& value) {
	Set(slot, (T*)&value, 1);
}
template <typename T>
void Uniform<T>::Set(unsigned int s, std::vector<T>& v) {
	Set(s, &v[0], (unsigned int)v.size());
}


#define UNIFORM_IMPL(gl_func, tType, dType) \
template<> void Uniform<tType>::Set(unsigned int slot, tType* data, unsigned int len) \
	{ gl_func(slot, (GLsizei)len, (dType*)&data[0]); } \

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, ivec4, int)
UNIFORM_IMPL(glUniform2iv, ivec2, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, vec2, float)
UNIFORM_IMPL(glUniform3fv, vec3, float)
UNIFORM_IMPL(glUniform4fv, vec4, float)
UNIFORM_IMPL(glUniform4fv, quat, float)


template<>
void Uniform<mat4>::Set(unsigned int slot, mat4* inputArr, unsigned int len) {
	glUniformMatrix4fv(slot, (GLsizei)len, GL_FALSE, (float*)&inputArr[0]);
}
