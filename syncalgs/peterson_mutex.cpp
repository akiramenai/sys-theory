#include "peterson_mutex.h"

peterson_mutex::peterson_mutex(){
	_interested[0]($) = 0;
	_interested[1]($) = 0;
	_victim($) = 0;
}

void peterson_mutex::lock(unsigned char index){
	_interested[index].store(true, std::memory_order_relaxed);
	_victim.exchange(index, std::memory_order_acq_rel);
	while (_interested[1 - index].load(std::memory_order_acquire)
		&& _victim.load(std::memory_order_seq_cst) == index)
		rl::yield(1, $); //spin
}

void peterson_mutex::unlock(unsigned char index){
	_interested[index].store(false, std::memory_order_release);
}
