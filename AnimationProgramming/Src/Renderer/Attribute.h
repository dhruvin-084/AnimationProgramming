#pragma once
#include<vector>

template<typename T>
class Attribute {

public:
	Attribute();
	~Attribute();

	void Set(T* inputArray, unsigned int arraySize);
	void Set(std::vector<T>& input);

	void BindTo(unsigned int slot);
	void UnBindFrom(unsigned int slot);

	unsigned int GetCount();
	unsigned int GetHandle();

private:
	void SetAttribPointer(unsigned int slot);

protected:
	unsigned int mHandle;
	unsigned int mCount;

private:
	Attribute(const Attribute&);
	Attribute& operator=(const Attribute&);
};