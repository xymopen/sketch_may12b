#pragma once

#ifndef _ENCODER_IMPL_H
#define _ENCODER_IMPL_H

template <class counter_t>
encoder<counter_t>::encoder(const unsigned char A, const unsigned char B)
	: logicalA(false), logicalB(false), count(0), A(A), B(B) {
	pinMode(this->A, INPUT);
	pinMode(this->B, INPUT);
}

template <class counter_t> void encoder<counter_t>::onChangeA() {
	this->logicalA = digitalRead(A);
	this->onChange(this->logicalA != this->logicalB);
}

template <class counter_t> void encoder<counter_t>::onChangeB() {
	this->logicalB = digitalRead(B);
	this->onChange(this->logicalA == this->logicalB);
}

template <class counter_t> void encoder<counter_t>::onChange(bool clockwise) {
	if (clockwise) {
		this->count += 1;
	} else {
		this->count -= 1;
	}
}

template <class counter_t> counter_t encoder<counter_t>::read() const {
	return this->count;
}

template <class counter_t> void encoder<counter_t>::reset() { this->count = 0; }

#endif	// _ENCODER_IMPL_H
