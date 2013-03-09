/*
 * peterson_mutex.h
 *
 *  Created on: Mar 7, 2013
 *      Author: dmitriy
 */

#ifndef PETERSON_MUTEX_H_
#define PETERSON_MUTEX_H_

#include <relacy/relacy_std.hpp>

struct peterson_mutex{
private:
	std::atomic_bool _interested[2];
	std::atomic_int _victim;
public:
	peterson_mutex();
	void lock(unsigned char index);
	void unlock(unsigned char index);
};



#endif /* PETERSON_MUTEX_H_ */
