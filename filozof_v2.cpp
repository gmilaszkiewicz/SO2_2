#include <iostream>
#include <thread>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <mutex>

using namespace std;
mutex mtx;

const int numThreads = 4;
bool forkEnable[numThreads];
int status[numThreads];
int HP[numThreads];
void pickUp(int tID){
	//mtx.lock();
	forkEnable[tID]=false;
	forkEnable[((2*numThreads+tID-1)%numThreads)]=false;
	//mtx.unlock();
}
void putDown(int tID){
//	mtx.lock();
	forkEnable[tID]=true;
	forkEnable[((2*numThreads+tID-1)%numThreads)]=true;
//	mtx.unlock();
}
void showStatus(){
		mtx.lock();
	for (int i=0;i<numThreads;i++)
	{
		mvprintw(1,i*5,"%d",status[i]);
		mvprintw(2,i*5+3,"%d",forkEnable[i]);
		mvprintw(i+4,0,"HP filozofa %d = %d",i,HP[i]);
		
		if(HP[i]<0)
		{
			mvprintw(i+10,5,"filozof %d padl",i);
		}
		refresh();
	}
	mtx.unlock();
}
void startThread(int tID) {
	while(1)//HP[tID]>0)
	{
		mtx.lock();
		if(forkEnable[tID] && forkEnable[(((2*numThreads)+tID-1)%numThreads)])
		{
			pickUp(tID);
			status[tID]=1;
			// mvprintw(1,tID*5,"%d",status[tID]);
			// mvprintw(2,tID*5+3,"%d",forkEnable[tID]);
			// refresh();
			
		}
		mtx.unlock();
		showStatus();
		usleep(rand() %500000+500000);

		mtx.lock();
		if(status[tID]==1)
		{
			putDown(tID);
			status[tID]=0;
			// mvprintw(1,tID*5,"%d",status[tID]);
			// mvprintw(2,tID*5+3,"%d",forkEnable[tID]);
			// refresh();
			
		}
		mtx.unlock();
		showStatus();
		// mvprintw(1,tID*5,"%d",status[tID]);
		// mvprintw(2,tID*5+3,"%d",forkEnable[tID]);
		// refresh();
		usleep(rand() %500000+500000);
	}	
}


//wzór na prawy widelec ((2N + i -2)%N)+1
int main(){
	srand(time(NULL));
	for(int i=0;i<numThreads;i++)
	{
		forkEnable[i]=true;
		status[i]=0;
		HP[i]=999;
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