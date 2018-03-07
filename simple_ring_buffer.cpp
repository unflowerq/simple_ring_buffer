#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

#define Interval 50
#define putaverage 5
#define putsigma2 3
#define getaverage 5
#define getsigma2 1
#define PI 3.141592

using namespace std;

int EndPoint = 0;
int EndTime = Interval * 100;
bool End = false;
int x = Interval * 100;

double putgaus(double input)
{
	double gisu;
	gisu = -1 * (pow((input-putaverage),2)) / (2 * putsigma2);

	return exp(gisu) / (sqrt(putsigma2)*sqrt(2 * PI));
}

double getgaus(double input)
{
	double gisu;
	gisu = -1 * (pow((input - getaverage), 2)) / (2 * getsigma2);

	return exp(gisu) / (sqrt(getsigma2)*sqrt(2 * PI));
}

void putData(int bufsize, int *ring)
{
	int inputdata = 0;
	int lookahead = 0;
	double i = 0;
	srand((unsigned int)time(NULL));
	double probability = 0;
	int randNum = 0;

	chrono::milliseconds mt(Interval);

	while (true)
	{
		if (End == true)
			break;

		probability += putgaus(i);
		i += (Interval / (double)1000);

		randNum = rand() % 100 + 1;

		if (probability*100 > randNum && ring[lookahead%bufsize] != -1)
		{
			cout << "Overflow" << endl;
			inputdata++;

			i = 0;
			probability = 0;
			EndPoint += Interval;
		}

		else if(probability * 100 > randNum && ring[lookahead%bufsize] == -1)
		{
			ring[lookahead%bufsize] = inputdata;
			lookahead++;
			inputdata++;

			for (int i = 0; i < bufsize; i++)
			{
				cout << ring[i] << "  ";
			}

			cout << endl;

			i = 0;
			probability = 0;
			EndPoint += Interval;
		}

		this_thread::sleep_for(mt);
	}
}

void getData(int bufsize, int *ring)
{
	int lookahead = 0;
	double i = 0;
	srand((unsigned int)time(NULL));
	double probability = 0;
	int randNum = 0;

	chrono::milliseconds dt(Interval);

	while (true)
	{
		if (EndPoint > EndTime)
		{
			End = true;
			break;
		}

		probability += getgaus(i);
		i += (Interval / (double)1000);

		randNum = rand() % 100 + 1;

		if (probability * 100 > randNum && ring[lookahead%bufsize] == -1)
		{
			cout << "Underflow" << endl;
			
			i = 0;
			probability = 0;
			EndPoint += Interval;
		}

		else if(probability * 100 > randNum && ring[lookahead%bufsize] != -1)
		{
			ring[lookahead%bufsize] = -1;
			lookahead++;

			i = 0;
			probability = 0;
			EndPoint += Interval;
		}

		this_thread::sleep_for(dt);
	}
}

int main()
{
	int bufsize = 4;
	int *ring;
	ring = new int[bufsize];

	for (int i = 0; i < bufsize; i++)
	{
		ring[i] = -1;
	}

	thread makedata(putData, bufsize, ring);

	thread deletedata(getData, bufsize, ring);

	makedata.join();
	deletedata.join();

	return 0;
}