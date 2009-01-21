#ifndef COMMON_HPP
#define COMMON_HPP
#include <stdlib.h>

template<typename T>
bool in_range_and_new(T new_val, T old_val, T max, T min = 0){
	if(new_val != old_val && new_val <= max && new_val >= min)
		return true;
	else
		return false;
}

#endif
