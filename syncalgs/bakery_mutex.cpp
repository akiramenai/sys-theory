/*
 * bakery_mutex.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: dmitriy
 */

#include "bakery_mutex.h"
#include <algorithm>
#include <tuple>

bakery_mutex::bakery_mutex(){
	unsigned N = _choosing.size();
	for (unsigned i = 0; i < N; ++i){
		_choosing[i]($) = false;
		_number[i]($) = 0;
	}
}

void bakery_mutex::lock(unsigned char index){
	_choosing[index]($) = true;
	unsigned N = _choosing.size();
	for (unsigned i = 0; i < N; ++i){
		if (_number[index]($) < _number[i]($))
			_number[index].store(_number[i]($), std::memory_order_seq_cst);
	}
	_choosing[index]($) = false;
    for(unsigned i = 0; i < N; ++i){
    	while(_choosing[i]($)) rl::yield(1, $);
    	while(_number[i]($) != 0 && std::tuple<unsigned, unsigned>(_number[i]($),i) <
    		  std::tuple<unsigned, unsigned>(_number[index]($),index))
    		rl::yield(1, $);
    }
}

void bakery_mutex::unlock(unsigned char index){
	_number[index]($) = 0;
}
