#pragma once

#ifndef _KERNEL_LIST_H
#define _KERNEL_LIST_H

#include <stddef.h>
#include <iterator>

class kernel_list_node;

class kernel_list;

template <class T>
class kernel_list_iterator;

template <class T>
class kernel_list_iterable;

class kernel_list_node {
	friend class kernel_list;

	template <class T>
	friend class kernel_list_iterator;
protected:
	kernel_list_node *prev_, *next_;
public:
	constexpr kernel_list_node();
	~kernel_list_node();

	void insert( kernel_list_node &other );
	void insert_before( kernel_list_node &other );
	void earse();
	void splice( kernel_list &list );

	template< class T >
	inline constexpr T* container( const kernel_list_node T::*member ) const;
};

class kernel_list {
	template <class T>
	friend class kernel_list_iterable;
protected:
	kernel_list_node head;
public:
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	kernel_list_node* front() const;
	kernel_list_node* back() const;

	kernel_list_node* prev( kernel_list_node &ref ) const;
	kernel_list_node* next( kernel_list_node &ref ) const;

	bool empty() const;
	size_type size() const;

	kernel_list_node* at( difference_type idx ) const;
	difference_type index_of( kernel_list_node &ref ) const;

	void push_front( kernel_list_node &node );
	void pop_front();
	void push_back( kernel_list_node &node );
	void pop_back();

	void shift_forward( difference_type num = 1 );
	void shift_backwards( size_type num = 1 );

	void clear();
};

template <class T>
class kernel_list_iterator {
private:
	kernel_list_node *node;
	const kernel_list_node T::*member;
public:
	using iterator_category = std::bidirectional_iterator_tag;
	using difference_type = ptrdiff_t;
	using value_type = T;
	using reference = T & ;
	using pointer = T * ;

	constexpr kernel_list_iterator( kernel_list_node &node, const kernel_list_node T::*member );

	bool operator!=( const kernel_list_iterator<T>& rhs ) const;
	reference operator*() const;
	kernel_list_iterator<T>& operator++();
	kernel_list_iterator<T>& operator--();
};

template <class T>
class kernel_list_iterable {
private:
	kernel_list &list;
	const kernel_list_node T::*member;
public:
	constexpr kernel_list_iterable( kernel_list &list, const kernel_list_node T::*member );

	kernel_list_iterator<T> begin() const;
	kernel_list_iterator<T> end() const;
};

#include "kernel-list.impl.hpp"

#endif	// _KERNEL_LIST_H
