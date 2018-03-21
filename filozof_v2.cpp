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

void pickUp(int tID){
	mtx.lock();
	forkEnable[tID]=false;
	forkEnable[((2*numThreads+tID-1)%numThreads)]=false;
	mtx.unlock();
	
}
void putDown(int tID){
	mtx.lock();
	forkEnable[tID]=true;
	forkEnable[((2*numThreads+tID-1)%numThreads)]=true;
	mtx.unlock();
}
void showStatus(){
		mtx.lock();
		int wynik = 0;
	for (int i=0;i<numThreads;i++)
	{
		cout<<status[i]<< " ";
		//wynik+=status[i];
	}
	//cout<<wynik<<endl;
	cout<<endl;
	mtx.unlock();
}
void startThread(int tID) {
	while(1)
	{
		if(forkEnable[tID] && forkEnable[((2*numThreads+tID-1)%numThreads)])
		{

		
			pickUp(tID);
			//pickUp(((2*numThreads+tID-1)%numThreads));
			//mvprintw(2,tID*2,"E");
			//refresh();
			status[tID]=1;
			int eatTime = rand() %1000;
			eatTime*=1000;
			usleep(eatTime);

			//mtx.lock();
			putDown(tID);
			//putDown(((2*numThreads+tID-1)%numThreads));
		//	mvprintw(2,tID*2,"P");
			//refresh();
			status[tID]=0;
			int philosopheTime = rand() %1000;
			philosopheTime*1000;
			usleep(philosopheTime);
			//mtx.unlock();

			
			
			
					
			showStatus();
		}
		
	}	
}


//wzór na prawy widelec ((2N + i -2)%N)+1
int main(){
	srand(time(NULL));
	for(int i=0;i<numThreads;i++)
	{
		forkEnable[i]=true;
		status[i]=0;
	}
	int tab[numThreads];
	
	thread t[numThreads];
	//initscr();
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
//	endwin();

	return 0;
}