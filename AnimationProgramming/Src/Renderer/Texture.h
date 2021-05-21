#pragma once


class Texture {
public:
	Texture();
	Texture(const char* path);
	~Texture();

	void Load(const char* path);

	void Set(unsigned int uniform, unsigned int texIndex);
	void UnSet(unsigned int texIndex);
	unsigned int GetHandle();

protected:
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mChannels;
	unsigned int mHandle;

private:
	Texture(const Texture&);
	Texture& operator=(const Texture&);
};