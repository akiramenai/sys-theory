#include "peterson_mutex.h"

peterson_mutex::peterson_mutex(){
	_interested0.store(0, std::memory_order_seq_cst);
	_interested1.store(0, std::memory_order_seq_cst);
	_victim.store(0, std::memory_order_seq_cst);
}

void peterson_mutex::lock(unsigned char index){
	if (index == 0){
		_interested0.store(true, std::memory_order_relaxed);
		//Students should be able to explain why it's doesn't work:
		//_enter[index].store(true, std::memory_order_release);
		_victim.exchange(0, std::memory_order_acq_rel);
		while (_interested1.load(std::memory_order_acquire)
			&& _victim.load(std::memory_order_seq_cst) == 0)
			rl::yield(1, $); //spin
	}
	else{
		_interested1.store(true, std::memory_order_relaxed);
			//Students should be able to explain why it's doesn't work:
			//_enter[index].store(true, std::memory_order_release);
		_victim.exchange(1, std::memory_order_acq_rel);
		while (_interested0.load(std::memory_order_acquire)
			&& _victim.load(std::memory_order_relaxed) == 1)
			rl::yield(1, $); //spin
	}

}

void peterson_mutex::unlock(unsigned char index){
	if (index == 0)
		_interested0.exchange(false, std::memory_order_release);
	else
		_interested1.exchange(false, std::memory_order_release);
}
