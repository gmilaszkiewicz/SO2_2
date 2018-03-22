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
int status[numThreads];
int cycleCounter[numThreads];
int eatStatus[numThreads];
int thinkStatus[numThreads];
bool endProgram=false;

void pickUp(int tID){
	forkEnable[tID]=false;
	forkEnable[((2*numThreads+tID-1)%numThreads)]=false;
}

void putDown(int tID){
	forkEnable[tID]=true;
	forkEnable[((2*numThreads+tID-1)%numThreads)]=true;
}

void showStatus(){
		mtx.lock();
	for (int i=0;i<numThreads;i++)
	{
		mvprintw(1,i*5+3,"%d",status[i]);
		mvprintw(2,i*5+6,"%d",forkEnable[i]);
		mvprintw(2,0,"%d",forkEnable[numThreads-1]);
		mvprintw(i+4,0,"Ile razy jadl filozof %d --> %d",i,cycleCounter[i]);
		mvprintw(i+4,45,"Procent jedzenia --> %d",eatStatus[i]);
		mvprintw(i+4,75,"Procent myslenia --> %d",thinkStatus[i]);
		mvprintw(1,numThreads*6,"<-- filozofowie");
		mvprintw(2,numThreads*6,"<-- widelce");
		refresh();
	}
	mtx.unlock();
}

void startThread(int tID) {
	while(!endProgram)
	{
		if(getch()==27)
		{
			endProgram=true;
			break;
		}
		mtx.lock();
		if((forkEnable[tID] && forkEnable[(((2*numThreads)+tID-1)%numThreads)]) && 
		(cycleCounter[tID]<=cycleCounter[(2*numThreads+tID-1)%numThreads]) && (cycleCounter[tID]<=cycleCounter[(2*numThreads+tID+1)%numThreads]))
		{
			pickUp(tID);
			status[tID]=1;	
			cycleCounter[tID]++;	
		}
		mtx.unlock();
		eatStatus[tID]=0;
		for(int i=1;i<=10;i++)
		{
			usleep(rand() %300000+200000);
			eatStatus[tID]+=10;
			showStatus();
		}
		eatStatus[tID]=0;
		clear();
		showStatus();

		mtx.lock();
		if(status[tID]==1)
		{
			putDown(tID);
			status[tID]=0;		
		}
		mtx.unlock();

		thinkStatus[tID]=0;
		for(int i=1;i<=10;i++)
		{
			usleep(rand() %10000+100000);
			thinkStatus[tID]+=10;
			showStatus();
		}
		thinkStatus[tID]=0;
		clear();
		showStatus();	
	}	
}
int main(){
	srand(time(NULL));
	for(int i=0;i<numThreads;i++)
	{
		forkEnable[i]=true;
		status[i]=0;
	}
	int tab[numThreads];
	thread t[numThreads];
	initscr();
	nodelay(stdscr,TRUE);
	refresh();
	for (int i = 0; i < numThreads; i++)
	{
		t[i] = thread(startThread, i);
	}
	for (int i = 0; i < numThreads; i++)
	{
		t[i].join();
	}
	endwin();
	return 0;
}