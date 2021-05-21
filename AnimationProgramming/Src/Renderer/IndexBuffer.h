#pragma once
#include<vector>

class IndexBuffer {
public:
	IndexBuffer();
	~IndexBuffer();

	void Set(unsigned int* arr, unsigned int len);
	void Set(std::vector<unsigned int>& input);

	unsigned int Count();
	unsigned int GetHandle();

private:
	IndexBuffer(const IndexBuffer&);
	IndexBuffer& operator=(const IndexBuffer&);

private:
	unsigned int mHandle;
	unsigned int mCount;
};