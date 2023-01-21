#pragma once

// Standard
#include <vector>


template<typename T>
class vector_ptr {
public:
	std::vector<T>* vec;
	size_t index;

public:
	vector_ptr() = default;

	vector_ptr(std::vector<T>& new_vector, size_t index = 0)
	{
		this->vec = &new_vector;
		this->index = 0;
	}

	T* operator->()
	{
		return &(*vec)[index];
	}
};
