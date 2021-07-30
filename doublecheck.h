#pragma once

#ifndef _DOUBLECHECK_H
#define _DOUBLECHECK_H

#include <Arduino.h>

bool doublecheck(unsigned char pin, decltype(millis()) duration = 1);

#endif	// _DOUBLECHECK_H
