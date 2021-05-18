#pragma once
#include<string>
#include<map>

class Shader {
public:
	Shader();
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Load(const std::string& vertexPath, const std::string& fragmentPath);

	void Bind();
	void Unbind();

	unsigned int GetAttribute(const std::string& name);
	unsigned int GetUniform(const std::string& name);
	unsigned int GetHandle();

private:
	Shader(const Shader&);
	Shader& operator=(const Shader&);
	
	std::string ReadFile(const std::string& path);
	unsigned int ComplieVertexShader(const std::string vertexSource);
	unsigned int ComplieFragmentShader(const std::string fragmentSource);
	bool LinkShaders(unsigned int vertex, unsigned int fragment);

	void PopulateAttributes();
	void PopulateUniforms();

private:
	unsigned int mHandle;

	std::map<std::string, unsigned int> mAttributes;
	std::map<std::string, unsigned int> mUniforms;
};