#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
	T *data;
	size_t current_size;
	size_t current_capacity;

	void resize_capacity(size_t new_capacity) {
		if (new_capacity == 0) new_capacity = 1;
		T *new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

		for (size_t i = 0; i < current_size; ++i) {
			new (new_data + i) T(data[i]);
		}

		for (size_t i = 0; i < current_size; ++i) {
			data[i].~T();
		}
		operator delete[](data);

		data = new_data;
		current_capacity = new_capacity;
	}

public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		T *ptr;
		friend class vector<T>;
		friend class const_iterator;
		iterator(T *p) : ptr(p) {}
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			return iterator(ptr + n);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++ptr;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--ptr;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		const T *ptr;
		friend class vector<T>;
		const_iterator(const T *p) : ptr(p) {}

	public:
		const_iterator(const iterator &it) : ptr(it.ptr) {}

		const_iterator operator+(const int &n) const {
			return const_iterator(ptr + n);
		}
		const_iterator operator-(const int &n) const {
			return const_iterator(ptr - n);
		}
		int operator-(const const_iterator &rhs) const {
			return ptr - rhs.ptr;
		}
		const_iterator& operator+=(const int &n) {
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) {
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		const_iterator& operator++() {
			++ptr;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		const_iterator& operator--() {
			--ptr;
			return *this;
		}
		const T& operator*() const {
			return *ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}

	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data(nullptr), current_size(0), current_capacity(0) {}
	vector(const vector &other) : data(nullptr), current_size(0), current_capacity(0) {
		if (other.current_size > 0) {
			data = static_cast<T*>(operator new[](other.current_capacity * sizeof(T)));
			current_capacity = other.current_capacity;
			for (size_t i = 0; i < other.current_size; ++i) {
				new (data + i) T(other.data[i]);
			}
			current_size = other.current_size;
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (size_t i = 0; i < current_size; ++i) {
			data[i].~T();
		}
		operator delete[](data);
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this != &other) {
			for (size_t i = 0; i < current_size; ++i) {
				data[i].~T();
			}
			operator delete[](data);

			current_size = 0;
			current_capacity = 0;
			data = nullptr;

			if (other.current_size > 0) {
				data = static_cast<T*>(operator new[](other.current_capacity * sizeof(T)));
				current_capacity = other.current_capacity;
				for (size_t i = 0; i < other.current_size; ++i) {
					new (data + i) T(other.data[i]);
				}
				current_size = other.current_size;
			}
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= current_size) {
			throw index_out_of_bound();
		}
		return data[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (current_size == 0) {
			throw container_is_empty();
		}
		return data[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (current_size == 0) {
			throw container_is_empty();
		}
		return data[current_size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data);
	}
	const_iterator begin() const {
		return const_iterator(data);
	}
	const_iterator cbegin() const {
		return const_iterator(data);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data + current_size);
	}
	const_iterator end() const {
		return const_iterator(data + current_size);
	}
	const_iterator cend() const {
		return const_iterator(data + current_size);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return current_size == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return current_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < current_size; ++i) {
			data[i].~T();
		}
		current_size = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		size_t index = pos.ptr - data;
		if (current_size == current_capacity) {
			resize_capacity(current_capacity == 0 ? 1 : current_capacity * 2);
		}

		for (size_t i = current_size; i > index; --i) {
			new (data + i) T(data[i - 1]);
			data[i - 1].~T();
		}
		new (data + index) T(value);
		++current_size;
		return iterator(data + index);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > current_size) {
			throw index_out_of_bound();
		}
		if (current_size == current_capacity) {
			resize_capacity(current_capacity == 0 ? 1 : current_capacity * 2);
		}

		for (size_t i = current_size; i > ind; --i) {
			new (data + i) T(data[i - 1]);
			data[i - 1].~T();
		}
		new (data + ind) T(value);
		++current_size;
		return iterator(data + ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		size_t index = pos.ptr - data;
		if (index >= current_size) {
			throw invalid_iterator();
		}

		data[index].~T();
		for (size_t i = index; i < current_size - 1; ++i) {
			new (data + i) T(data[i + 1]);
			data[i + 1].~T();
		}
		--current_size;
		return iterator(data + index);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= current_size) {
			throw index_out_of_bound();
		}

		data[ind].~T();
		for (size_t i = ind; i < current_size - 1; ++i) {
			new (data + i) T(data[i + 1]);
			data[i + 1].~T();
		}
		--current_size;
		return iterator(data + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (current_size == current_capacity) {
			resize_capacity(current_capacity == 0 ? 1 : current_capacity * 2);
		}
		new (data + current_size) T(value);
		++current_size;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (current_size == 0) {
			throw container_is_empty();
		}
		--current_size;
		data[current_size].~T();
	}
};


}

#endif
