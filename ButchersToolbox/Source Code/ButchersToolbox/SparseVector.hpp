#pragma once

// Standard
#include <cstdint>
#include <vector>
#include <cassert>


template<typename T = uint32_t>
class SparseVector {
	std::vector<uint32_t> deleted;


	template<typename T>
	struct Node {
		bool is_deleted;
		T elem;
	};
	std::vector<Node<T>> nodes;

public:
	class Iterator {
	private:
		SparseVector* parent;
		uint32_t index;

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = int;
		using value_type = T;
		using pointer = void;
		using reference = T;


		Iterator(SparseVector* new_parent, uint32_t new_index) {
			this->parent = new_parent;
			this->index = new_index;
		}

		reference operator*() const {
			return (*parent)[index];
		}
		
		// pointer operator->() { return count; }

		void next()
		{
			index++;

			while (index < parent->nodes.size() &&
				parent->isDeleted(index) == true)
			{
				index++;
			}
		}

		Iterator& operator++() {
			next();
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp = *this;
			(*this).next();
			return tmp;
		}

		friend bool operator== (const Iterator& a, const Iterator& b) {
			return a.index == b.index;
		};

		friend bool operator!= (const Iterator& a, const Iterator& b) {
			return a.index != b.index;
		};
	};

public:

	/* Query */

	/**
	 * @brief Access the element directly.
	 * @return Stored element.
	*/
	T& operator[](uint32_t index);

	/** 
	 * @brief Check whether or not the specified element at index is deleted.
	*/
	bool isDeleted(uint32_t index);


	/* Iterators */

	Iterator begin();
	Iterator end();


	/* Add Functions */

	void resize(uint32_t new_size);

	T& emplace(uint32_t& r_index);


	/* Delete Functions */

	void erase(uint32_t index);
};


template<typename T>
T& SparseVector<T>::operator[](uint32_t index)
{
	// Accessed element must not be marked as deleted
	assert(isDeleted(index) == false);
	return nodes[index].elem;
}

template<typename T>
bool SparseVector<T>::isDeleted(uint32_t index)
{
	return nodes[index].is_deleted;
}

template<typename T>
SparseVector<T>::Iterator SparseVector<T>::begin()
{
	return Iterator(this, 0);
}

template<typename T>
SparseVector<T>::Iterator SparseVector<T>::end()
{
	return Iterator(this, (uint32_t)nodes.size());
}

template<typename T>
void SparseVector<T>::resize(uint32_t new_size)
{
	nodes.resize(new_size);
}

template<typename T>
T& SparseVector<T>::emplace(uint32_t& r_index)
{
	// Try reuse deleted
	for (uint32_t& deleted_idx : deleted) {

		if (deleted_idx != 0xFFFF'FFFF) {

			// mark node as available
			auto& recycled_node = nodes[deleted_idx];
			recycled_node.is_deleted = false;

			// remove from deleted list
			deleted_idx = 0xFFFF'FFFF;

			r_index = deleted_idx;
			return recycled_node.elem;
		}
	}

	// Create new node
	r_index = nodes.size();

	auto& new_node = nodes.emplace_back();
	return new_node.elem;
}

template<typename T>
void SparseVector<T>::erase(uint32_t index)
{
	Node<T>& node = nodes[index];

	if (node.is_deleted == false) {

		node.is_deleted = true;

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