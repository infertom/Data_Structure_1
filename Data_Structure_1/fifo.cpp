
#include<iostream>   
using namespace std;   
#include "fifo.h"   

fifo::fifo(int seconds_per_page):simulator(seconds_per_page){}   

void fifo::simulate(string file){   
	loadworkload(file);   

	queue<event> waiting;//等待队列   
	queue<event> servicing;//进行队列   

	int time = 0;//进行时间     
	int service_time = workload.front().arrival_time();   
	float aggregate_latency = 0;   
	event e;
	int totaljob = workload.size();

	cout<<"FIFO Simulation"<<endl<<endl;

	while(!(workload.empty() && waiting.empty() && servicing.empty()) ){   
		while( !workload.empty() && workload.front().arrival_time() == time ){    
			e = workload.front();
			workload.pop();         
			cout << "\tArriving:  " << e.getjob() << " at " << time <<" seconds "<<  endl;         
			if( servicing.empty() ){   
				servicing.push(e);          
				cout << "\tServicing:  " << servicing.front().getjob() << " at " << time <<" seconds "<<  endl;   
			}
			else waiting.push(e);
		}

		if( !waiting.empty() && servicing.empty() ){   
			e = waiting.front();
			waiting.pop();    
			servicing.push(e);
			aggregate_latency += service_time - e.arrival_time();
			cout << "\tServicing:  " << servicing.front().getjob() << " at " << time <<" seconds "<<  endl; 
		}   

		if( !servicing.empty() && servicing.front().getjob().getnumpages() * seconds_per_page + service_time - 1 == time ){   
			service_time = time + 1;
			servicing.pop();   
		}

		time++;
	}   

	cout<< endl << "\tTotal jobs: " << totaljob << endl
		<< "\tAggregate latency: " << aggregate_latency <<" seconds " << endl
		<< "\tMean latency: " << aggregate_latency/totaljob <<" seconds" << endl;     
}  
