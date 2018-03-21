#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;

const int numThreads = 5;
bool forkEnable[numThreads];

void startThread(int tID) {
while(1)
{
	if(forkEnable[tID] && forkEnable[((2*numThreads+tID-1)%numThreads)])
	{
		mtx.lock();
		forkEnable[tID]=false;
		forkEnable[((2*numThreads+tID-1)%numThreads)]=false;
		//cout<<"Filozof " << tID <<" je" <<endl;
		mvprintw(2,tID*2,"E");
		refresh();
		int eatTime = rand() %1000;
		eatTime*=1000;
		usleep(eatTime);
		forkEnable[tID]=true;
		forkEnable[((2*numThreads+tID-1)%numThreads)]=true;

		int philosopheTime = rand() %1000;
		//cout<<"Filozof " << tID <<" filozofuje" <<endl;
		mvprintw(2,tID*2,"P");
		refresh();
		mtx.unlock();
		philosopheTime*1000;
		usleep(philosopheTime);
	}
	
}
	
}

//wzór na prawy widelec ((2N + i -2)%N)+1
int main(){
	srand(time(NULL));
	for(int i=0;i<numThreads;i++)
	{
		forkEnable[i]=true;
	}
	int tab[numThreads];
	
	thread t[numThreads];
	initscr();
	for (int i = 0; i < numThreads; i++)
	{
		t[i] = thread(startThread, i);
		//mvprintw(0,numThreads*2,"%d",&numThreads);
	}
	for (int i = 0; i < numThreads; i++)
	{
		//int wynik=((2*numThreads+(i+1)-2) % numThreads)+1;
		//cout<<wynik<<endl;
		t[i].join();
	}
	endwin();

	return 0;
}