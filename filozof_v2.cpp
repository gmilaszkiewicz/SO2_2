#include <iostream>
#include <thread>
#include <ncurses.h>

using namespace std;

const int numThreads = 5;

void startThread(int tID) {
	//cout << "Filozof " << tID <<" zasiadl do stolu." <<endl;
}


int main(){
	srand(time(NULL));
	initscr();
	thread t[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		t[i] = thread(startThread, i);
	}
	for (int i = 0; i < numThreads; i++)
	{
		t[i].join();
	}
	printw("Hello World");
	getch();
	refresh();
	endwin();
	return 0;
}