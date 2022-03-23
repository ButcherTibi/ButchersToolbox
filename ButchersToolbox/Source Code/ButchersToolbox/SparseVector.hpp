#pragma once

// Standard
#include <cstdint>
#include <vector>


// Forward declarations
template<typename T>
class SparseVector;


inline void assert_cond(bool condition) {
#ifndef NDEBUG  // or _DEBUG
	if (condition != true) {
		throw std::exception();
	}
#endif
}

inline void assert_cond(bool condition, const char* fail_msg) {
#ifndef NDEBUG  // or _DEBUG
	if (condition != true) {
		throw std::exception(fail_msg);
	}
#endif
}


template<typename T>
struct DeferredVectorNode {
	bool is_deleted;
	T elem;
};


template<typename T>
class SparseVectorIterator {
public:
	SparseVector<T>* _parent;
	uint32_t _index;

public:

	/// <summary>
	/// Get current element pointed by vector
	/// </summary>
	T& get()
	{
		return (*_parent)[_index];
	}

	uint32_t index()
	{
		return _index;
	}

	void next()
	{
		_index++;

		while (_index < _parent->nodes.size() && _parent->nodes[_index].is_deleted == true) {
			_index++;
		}
	}

	void operator++(int)
	{
		next();
	}

	bool operator!=(SparseVectorIterator right)
	{
		return _parent != right._parent || _index != right._index;
	}
};


template<typename T = uint32_t>
class SparseVector {
public:
	uint32_t used_size;  // current used size without deleted elements

	std::vector<DeferredVectorNode<T>> nodes;
	std::vector<uint32_t> deleted;

public:

	/* Query */

	T& operator[](uint32_t index)
	{
		assert_cond(nodes[index].is_deleted == false, "accessed element marked as deleted");
		return nodes[index].elem;
	}

	/// <summary>
	/// Check whether or not the specified element at index is deleted
	/// </summary>
	bool isDeleted(uint32_t index)
	{
		return nodes[index].is_deleted;
	}


	/* Iterators */

	SparseVectorIterator<T> begin()
	{
		SparseVectorIterator<T> iter;
		iter._parent = this;
		iter._index = 0;

		return iter;
	}

	SparseVectorIterator<T> after()
	{
		SparseVectorIterator<T> iter;
		iter._parent = this;
		iter._index = (int32_t)nodes.size();

		return iter;
	}

	
	/* Capacity */

	uint32_t size()
	{
		return used_size;
	}

	uint32_t capacity()
	{
		return nodes.capacity();
	}


	/* Add Functions */

	void resize(uint32_t new_size)
	{
		nodes.resize(new_size);

		used_size = new_size;
	}

	void reserve(uint32_t new_capacity)
	{
		nodes.reserve(new_capacity);
	}

	T& emplace(uint32_t& r_index)
	{
		// try reuse deleted
		for (uint32_t& deleted_idx : deleted) {

			if (deleted_idx != 0xFFFF'FFFF) {

				// mark node as available
				DeferredVectorNode<T>& recycled_node = nodes[deleted_idx];
				recycled_node.is_deleted = false;

				// remove from deleted list
				deleted_idx = 0xFFFF'FFFF;

				used_size++;

				r_index = deleted_idx;
				return recycled_node.elem;
			}
		}

		// Create new node
		used_size++;

		r_index = nodes.size();

		DeferredVectorNode<T>& new_node = nodes.emplace_back();
		return new_node.elem;
	}


	/* Delete Functions */

	void erase(uint32_t index)
	{
		DeferredVectorNode<T>& node = nodes[index];

		if (node.is_deleted == false) {

			node.is_deleted = true;
			used_size--;

			// add to delete list
			for (uint32_t& deleted_idx : deleted) {
				if (deleted_idx == 0xFFFF'FFFF) {
					deleted_idx = index;
					return;
				}
			}

			uint32_t& new_deleted_index = deleted.emplace_back();
			new_deleted_index = index;
		}
	}

	void clear()
	{
		used_size = 0;
		nodes.clear();
		deleted.clear();
	}
};
