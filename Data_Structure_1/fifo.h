#ifndef FIFO_H
#define FIFO_H

#include "simulator.h"
#include <queue>
#include <string>

class fifo:public simulator{
public:
	fifo(int seconds_per_page):simulator(seconds_per_page){};
	virtual void simulate(string file);
	virtual ~fifo(){};
};


#endif