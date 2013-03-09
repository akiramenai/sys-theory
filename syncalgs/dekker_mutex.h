/*
 * dekker_mutex.h
 *
 *  Created on: Mar 8, 2013
 *      Author: dmitriy
 */

#ifndef DEKKER_MUTEX_H_
#define DEKKER_MUTEX_H_

#include <relacy/relacy_std.hpp>

struct dekker_mutex{
private:
	std::atomic_bool _interested0;
	std::atomic_bool _interested1;
	std::atomic_int _turn;
public:
	dekker_mutex();
	void lock(unsigned char index);
	void unlock(unsigned char index);
};


#endif /* DEKKER_MUTEX_H_ */
