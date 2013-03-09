/*
 * dekker_mutex.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: dmitriy
 */

#include "dekker_mutex.h"

dekker_mutex::dekker_mutex(){
	_interested[0]($) = 0;
	_interested[1]($) = 0;
	_turn($) = 0;
}

void dekker_mutex::lock(unsigned char index){
	_interested[index].store(true, std::memory_order_seq_cst);
	while (_interested[1 - index].load(std::memory_order_seq_cst)){
		if (_turn.load(std::memory_order_seq_cst) == 1 - index){
			_interested[index].store(false, std::memory_order_seq_cst);
			while (_turn.load(std::memory_order_seq_cst) == 1 - index)
				rl::yield(1, $); //spin
			_interested[index].store(true, std::memory_order_seq_cst);
		}
		rl::yield(1, $);
	}
}

void dekker_mutex::unlock(unsigned char index){
	_interested[index].store(false, std::memory_order_seq_cst);
	_turn.exchange(1 - index, std::memory_order_seq_cst);
}


