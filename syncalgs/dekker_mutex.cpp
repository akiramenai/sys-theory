/*
 * dekker_mutex.cpp
 *
 *  Created on: Mar 8, 2013
 *      Author: dmitriy
 */

#include "dekker_mutex.h"

dekker_mutex::dekker_mutex(){
	_interested0($) = 0;
	_interested1($) = 0;
	_turn($) = 0;
}

void dekker_mutex::lock(unsigned char index){
	if (index == 0){
		_interested0.store(true, std::memory_order_seq_cst);
		while (_interested1.load(std::memory_order_seq_cst)){
			if (_turn.load(std::memory_order_seq_cst) == 1){
				_interested0.store(false, std::memory_order_seq_cst);
				while (_turn.load(std::memory_order_seq_cst) == 1)
					rl::yield(1, $); //spin
				_interested0.store(true, std::memory_order_seq_cst);
			}
		}
	}
	else{
		_interested1.store(true, std::memory_order_seq_cst);
		while (_interested0.load(std::memory_order_seq_cst)){
			if (_turn.load(std::memory_order_seq_cst) == 0){
				_interested1.store(false, std::memory_order_seq_cst);
				while (_turn.load(std::memory_order_seq_cst) == 0)
					rl::yield(1, $); //spin
				_interested1.store(true, std::memory_order_seq_cst);
			}
		}
	}
}

void dekker_mutex::unlock(unsigned char index){
	if (index == 0){
		_interested0.store(false, std::memory_order_seq_cst);
		_turn.exchange(1, std::memory_order_seq_cst);
	}
	else{
		_interested1.store(false, std::memory_order_seq_cst);
		_turn.exchange(0, std::memory_order_seq_cst);
	}
}


