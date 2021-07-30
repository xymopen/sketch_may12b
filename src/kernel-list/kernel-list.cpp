#include "kernel-list.h"

kernel_list_node::~kernel_list_node() { earse(); }

void kernel_list_node::insert( kernel_list_node &next ) {
	kernel_list_node* nextp = this->next_;

	this->next_ = &next;
	next.prev_ = this;
	next.next_ = nextp;
	nextp->prev_ = &next;
}

void kernel_list_node::earse() {
	kernel_list_node *prevp = this->prev_, *nextp = this->next_;

	prevp->next_ = nextp;
	nextp->prev_ = prevp;

	this->prev_ = this;
	this->next_ = this;
}

void kernel_list_node::splice( kernel_list &list ) {
	kernel_list_node *prev = this->prev_,
		*next = this->next_;

	earse();

	if ( !list.empty() ) {
		prev->insert( *list.front() );
		list.push_back( *next );
	} else {
		prev->insert( *next );
	}
}

kernel_list::size_type kernel_list::size() const {
	kernel_list::size_type s = 0;

	for ( kernel_list_node *p = this->head.next_; p != &this->head; p = p->next_ ) {
		s += 1;
	}

	return s;
}

kernel_list_node* kernel_list::at( kernel_list::difference_type idx ) const {
	if ( empty() ) {
		return nullptr;
	} else {
		kernel_list_node *p = const_cast< kernel_list_node* >( &this->head );

		if ( idx >= 0 ) {
			while ( idx >= 0 ) {
				p = p->next_;

				if ( p != &this->head ) {
					idx -= 1;
				}
			}
		} else {
			while ( idx < 0 ) {
				p = p->prev_;

				if ( p != &this->head ) {
					idx += 1;
				}
			}
		}

		return p;
	}
}

kernel_list::difference_type kernel_list::index_of( kernel_list_node &ref ) const {
	kernel_list::size_type idx = 0;

	for ( kernel_list_node *p = this->head.next_; p != &this->head; p = p->next_ ) {
		if ( p == &ref ) {
			return idx;
		} else if ( p ) {
			idx += 1;
		}
	}

	return -1;
}

void kernel_list::shift_forward( kernel_list::difference_type num ) {
	kernel_list_node *dst = at( num );

	this->head.earse();
	dst->insert_before( this->head );
}
