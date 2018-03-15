#include <iostream>
#include <thread>

using namespace std;

const int numThreads = 5;

void startThread(int tID) {
	cout << "Filozof " << tID <<" zasiadl do stolu." <<endl;
}


int main(){
	srand(time(NULL));
	thread t[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		t[i] = thread(startThread, i);
	}
	for (int i = 0; i < numThreads; i++)
	{
		t[i].join();
	}

	return 0;
}