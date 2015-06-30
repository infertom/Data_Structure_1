#include <iostream>
#include <fstream>

using namespace std;
//#define DEBUG

#include "fifo.h"

void fifo::simulate(string file){
	loadworkload(file);
	int total = workload.size();
	int latency = 0;

	cout<<"FIFO Simulation"<<endl<<endl;

	queue<event> waitEvent;
	int systemTime = 1, remainTime = 0;
	event currentEvent, currentWaitEvent;
	bool flag1 = false, flag2 = false;

	while ( !(!flag1 && !flag2 && workload.empty() && waitEvent.empty()) )
	{
#ifdef DEBUG
		cout<<"workload:"<<workload.size()<<"    waitevent:"<<waitEvent.size()<<"	flag1:"<<flag1<<"	flag2:"<<flag2<<endl;
#endif

		//确保当前选定的到达任务存在
		if ( !flag1 && !workload.empty() ) {
			currentEvent = workload.front();
			flag1 = true;
			workload.pop();
		}

		//将所有与系统时间相同的任务加入等待执行队列
		while ( flag1 && currentEvent.arrival_time() == systemTime )
		{
			cout<<"      Arriving: "<<currentEvent.getjob()<<" at "<<systemTime<<" second"<<((systemTime > 1)? "s":"")<<endl;
			waitEvent.push(currentEvent);
			if ( !workload.empty() ) {
				currentEvent = workload.front();
				workload.pop();
			}
			else flag1 = false;
		}

		//确保当前选定的执行任务存在
		if ( !flag2 && !waitEvent.empty() ){
			currentWaitEvent = waitEvent.front();
			flag2 = true;
			waitEvent.pop();
		}

		//当上一个任务执行时间为0时执行当前任务，并重置剩余时间
#ifdef DEBUG
		cout<<"xxx:"<<(currentWaitEvent.arrival_time() + seconds_per_page * currentWaitEvent.getjob().getnumpages())<<endl;
#endif
		if ( flag2 && !remainTime )
		{
			cout<<"      Servicing: "<<currentWaitEvent.getjob()<<" at "<<systemTime<<" second"<<((systemTime > 1)? "s":"")<<endl;
			remainTime = seconds_per_page * currentWaitEvent.getjob().getnumpages();
			latency += systemTime - currentWaitEvent.arrival_time();
			if ( !waitEvent.empty() ){
				currentWaitEvent = waitEvent.front();
				waitEvent.pop();
			}
			else flag2 = false;
		}

		systemTime++;
		if ( remainTime ) remainTime--;
	}

	cout<<endl
		<<"      Total jobs: "<<total<<endl
		<<"      Aggregate latency: "<<latency<<" second"<<((latency > 1)? "s":"")<<endl
		<<"      Mean latency: "<<(1.0 * latency / total)<<" seconds "<<endl;
};