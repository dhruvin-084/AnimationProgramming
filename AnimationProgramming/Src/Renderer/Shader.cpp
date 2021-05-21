#include<iostream>
#include<fstream>
#include<sstream>
#include<glad/glad.h>
#include"Shader.h"

Shader::Shader()
{
	mHandle = glCreateProgram();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	mHandle = glCreateProgram();
	Load(vertexPath, fragmentPath);
}

Shader::~Shader()
{
	glDeleteProgram(mHandle);
}

void Shader::Load(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::ifstream f(vertexPath.c_str());
	bool vertFile = f.good();
	f.close();

	f = std::ifstream(fragmentPath.c_str());
	bool fragFile = f.good();
	f.close();

	std::string v_source;
	if (vertFile)
		v_source = ReadFile(vertexPath);
	else
		std::cout << vertexPath << " File not avaliable\n";

	std::string f_sourse;
	if (fragFile)
		f_sourse = ReadFile(fragmentPath);
	else
		std::cout << fragmentPath << " File not avaliable\n";

	unsigned int vert = ComplieVertexShader(v_source);
	unsigned int frag = ComplieFragmentShader(f_sourse);
	if (LinkShaders(vert, frag)) {
		PopulateAttributes();
		PopulateUniforms();
	}
}

void Shader::Bind()
{
	glUseProgram(mHandle);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

unsigned int Shader::GetAttribute(const std::string& name)
{
	std::map<std::string, unsigned int>::iterator it = mAttributes.find(name);
	if (it == mAttributes.end()) {
		std::cout << "Bad attrib index: " << name << "\n";
		return 0;
	}

	return it->second;
}

unsigned int Shader::GetUniform(const std::string& name)
{
	std::map<std::string, unsigned int>::iterator it = mUniforms.find(name);
	if (it == mAttributes.end()) {
		std::cout << "Bad Uniform index: " << name << "\n";
		return 0;
	}

	return it->second;
}

unsigned int Shader::GetHandle()
{
	return mHandle;
}

std::string Shader::ReadFile(const std::string& path)
{
	std::ifstream file;
	file.open(path);

	std::stringstream contents;
	contents << file.rdbuf();

	file.close();

	return contents.str();
}

unsigned int Shader::ComplieVertexShader(const std::string vertexSource)
{
	unsigned int vHandle = glCreateShader(GL_VERTEX_SHADER);
	const char* v_source = vertexSource.c_str();
	glShaderSource(vHandle, 1, &v_source, nullptr);
	glCompileShader(vHandle);
	int success = 0;
	glGetShaderiv(vHandle, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infolog[512];
		glGetShaderInfoLog(vHandle, sizeof(infolog), nullptr, infolog);
		std::cout << "Vertex shader compilation failed\n";
		std::cout << "\t" << infolog << "\n";
		glDeleteShader(vHandle);
		return 0;
	}

	return vHandle;
}

unsigned int Shader::ComplieFragmentShader(const std::string fragmentSource)
{
	unsigned int fHandle = glCreateShader(GL_FRAGMENT_SHADER);
	const char* f_source = fragmentSource.c_str();
	glShaderSource(fHandle, 1, &f_source, nullptr);
	glCompileShader(fHandle);
	int success = 0;
	glGetShaderiv(fHandle, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infolog[512];
		glGetShaderInfoLog(fHandle, sizeof(infolog), nullptr, infolog);
		std::cout << "Fragment shader compilation failed\n";
		std::cout << "\t" << infolog << "\n";
		glDeleteShader(fHandle);
		return 0;
	}

	return fHandle;
}

bool Shader::LinkShaders(unsigned int vertex, unsigned int fragment)
{
	glAttachShader(mHandle, vertex);
	glAttachShader(mHandle, fragment);
	glLinkProgram(mHandle);
	int success = 0;
	glGetProgramiv(mHandle, GL_LINK_STATUS, &success);
	if (!success) {
		char infolog[512];
		glGetProgramInfoLog(mHandle, sizeof(infolog), nullptr, infolog);
		std::cout << "Error: Shader linking failed\n";
		std::cout << "\t" << infolog << "\n";
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return false;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void Shader::PopulateAttributes()
{
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;

	glUseProgram(mHandle);
	glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &count);

	for (int i = 0; i < count; i++) {
		memset(name, 0, sizeof(name));
		glGetActiveAttrib(mHandle, (GLuint)i, sizeof(name), &length, &size, &type, name);
		
		int attribLocation = glGetAttribLocation(mHandle, name);
		if (attribLocation >= 0)
			mAttributes[name] = attribLocation;
	}
	glUseProgram(0);
}

void Shader::PopulateUniforms()
{
	int count = -1;
	int length;
	int size;
	char name[128];
	GLenum type;
	char testName[256];

	glUseProgram(mHandle);
	glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; i++) {
		memset(name, 0, sizeof(name));
		glGetActiveUniform(mHandle, (GLuint)i, sizeof(name), &length, &size, &type, name);

		int uniform = glGetUniformLocation(mHandle, name);
		if (uniform >= 0) {
			std::string uniformName = name;
			// if name contains [ it is an array
			std::size_t found = uniformName.find('[');
			if (found != std::string::npos) {
				uniformName.erase(uniformName.begin() + found, uniformName.end());
				unsigned int uniformIndex = 0;
				while (true) {
					memset(testName, 0, sizeof(testName));
					sprintf_s(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);

					int uniformLocation = glGetUniformLocation(mHandle, testName);
					if (uniformLocation < 0)
						break;

					mUniforms[testName] = uniformLocation;
				}
			}
			mUniforms[uniformName] = uniform;
		}
	}

	glUseProgram(0);
}
