#include<glad/glad.h>

#include"IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &mHandle);
	mCount = 0;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &mHandle);
}

void IndexBuffer::Set(unsigned int* arr, unsigned int len)
{
	mCount = len;
	unsigned int size = sizeof(unsigned int);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, len * size, arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Set(std::vector<unsigned int>& input)
{
	Set(&input[0], (unsigned int)input.size());
}

unsigned int IndexBuffer::Count()
{
	return mCount;
}

unsigned int IndexBuffer::GetHandle()
{
	return mHandle;
}
