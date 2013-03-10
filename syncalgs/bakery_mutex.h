/*
 * bakery_mutex.h
 *
 *  Created on: Mar 8, 2013
 *      Author: dmitriy
 */

#ifndef BAKERY_MUTEX_H_
#define BAKERY_MUTEX_H_

#include <relacy/relacy_std.hpp>

struct bakery_mutex{
private:
	std::vector<std::atomic_bool> _choosing;
	std::vector<std::atomic_uint> _number;
public:
	bakery_mutex();
	void lock(unsigned char index);
	void unlock(unsigned char index);
};

#endif /* BAKERY_MUTEX_H_ */
