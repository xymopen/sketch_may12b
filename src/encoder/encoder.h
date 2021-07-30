#pragma once

#ifndef _ENCODER_H
#define _ENCODER_H

template<class counter_t = signed int>
class encoder {
private:
	volatile bool logicalA, logicalB;
	volatile counter_t count;

	void onChange(bool clockwise);
public:
	const unsigned char A, B;

	encoder( const unsigned char A, const unsigned char B );
	void onChangeA();
	void onChangeB();
	counter_t read() const;
	void reset();
};

#include "encoder.impl.hpp"

#endif	// _ENCODER_H
