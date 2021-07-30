#pragma once

#ifndef _KERNEL_LIST_IMPL_H
#define _KERNEL_LIST_IMPL_H

constexpr kernel_list_node::kernel_list_node() : prev_( this ), next_( this ) {
	/* empty */
};

inline void kernel_list_node::insert_before( kernel_list_node &prev ) {
	this->prev_->insert( prev );
}

template< class T >
inline constexpr T* kernel_list_node::container( const kernel_list_node T::*member ) const {
	// C++ equivalences of
	// [offsetof](http://man7.org/linux/man-pages/man3/offsetof.3.html "offsetof(3) - Linux manual page")
	// and
	// [container_of](https://stackoverflow.com/questions/15832301/understanding-container-of-macro-in-the-linux-kernel
	// "C - Understanding container_of macro in the Linux kernel - Stack Overflow")
	// macro
	return reinterpret_cast< T* >( reinterpret_cast< intptr_t >( this ) -
		reinterpret_cast< ptrdiff_t >( &( reinterpret_cast< T* >( 0 )->*member ) ) );
}

template< class T >
inline constexpr kernel_list_node member( const T &container, const kernel_list_node T::*member ) {
	// C++ equivalences of offsetof and container_of macro
	return reinterpret_cast< T* >( reinterpret_cast< intptr_t >( &container ) +
		reinterpret_cast< ptrdiff_t >( &( reinterpret_cast< T* >( 0 )->*member ) ) );
}

inline kernel_list_node* kernel_list::front() const {
	return empty() ? nullptr : this->head.next_;
}

inline kernel_list_node* kernel_list::back() const {
	return empty() ? nullptr : this->head.prev_;
}

inline kernel_list_node* kernel_list::prev( kernel_list_node &ref ) const {
	kernel_list_node* prev = ref.prev_;

	return prev == &this->head ? nullptr : prev;
}

inline kernel_list_node* kernel_list::next( kernel_list_node &ref ) const {
	kernel_list_node* next = ref.next_;

	return next == &this->head ? nullptr : next;
}

inline bool kernel_list::empty() const {
	return this->head.next_ == &this->head;
}

inline void kernel_list::push_front( kernel_list_node &node ) {
	this->head.insert( node );
}

inline void kernel_list::pop_front() {
	if ( !empty() ) {
		this->head.next_->earse();
	}
}

inline void kernel_list::push_back( kernel_list_node &node ) {
	this->head.insert_before( node );
}

inline void kernel_list::pop_back() {
	if ( !empty() ) {
		this->head.prev_->earse();
	}
}

inline void kernel_list::shift_backwards( kernel_list::size_type num ) {
	shift_forward( -static_cast< kernel_list::difference_type >( num ) );
}

inline void kernel_list::clear() {
	while ( !empty() ) {
		this->head.next_->earse();
	}
}

template<class T>
constexpr kernel_list_iterator<T>::kernel_list_iterator( kernel_list_node &node, const kernel_list_node T::*member ) : node( &node ), member( member ) {
	/* empty */
}

template<class T>
inline bool kernel_list_iterator<T>::operator!=( const kernel_list_iterator<T> &rhs ) const {
	return this->node != rhs.node;
}

template<class T>
inline typename kernel_list_iterator<T>::reference kernel_list_iterator<T>::operator*() const {
	return *( this->node->container( this->member ) );
}

template<class T>
inline kernel_list_iterator<T>& kernel_list_iterator<T>::operator++() {
	this->node = this->node->next_;

	return *this;
}

template<class T>
inline kernel_list_iterator<T>& kernel_list_iterator<T>::operator--() {
	this->node = this->node->prev_;

	return *this;
}

template <class T>
constexpr kernel_list_iterable<T>::kernel_list_iterable( kernel_list &list, const kernel_list_node T::*member ) : list( list ), member( member ) {
	/* empty */
};

template <class T>
kernel_list_iterator<T> kernel_list_iterable<T>::begin() const {
	kernel_list_node *first = list.front();

	if ( nullptr == first ) {
		return this->end();
	} else {
		kernel_list_iterator<T> temp( *first, this->member );

		return temp;
	}
};

template <class T>
kernel_list_iterator<T> kernel_list_iterable<T>::end() const {
	kernel_list_iterator<T> temp( list.head, this->member );

	return temp;
};

#endif	// _KERNEL_LIST_IMPL_H
