#include "DDLLCaller.h"
#ifdef _WIN32
#	ifdef _WIN64
#		define STDCALL
#	else
#		define STDCALL __stdcall
#	endif // _WIN64
#else
#	ifdef __x86_64
#		define STDCALL
#	else
#		define STDCALL ____attribute__((__stdcall__))
#	endif
#endif

int add(int a, int b);
int STDCALL sad(int a, int b);
double sub(double a, double b);
double STDCALL ssb(double a, double b);
LoadModel(TestDllAPI);
LoadUsualFunction(TestDllAPI, add);
LoadUsualFunction(TestDllAPI, sub);
LoadStdcallFunction(TestDllAPI, sad);
LoadStdcallFunction(TestDllAPI, ssb);

#include <chrono>
#include <iostream>
#include <string>

#define COST(callable, mem)\
	start = chrono::high_resolution_clock::now();\
	for (int i = 0; i < count; ++i) {\
		mem += GetFunction(TestDllAPI, callable)(i, 2);\
	}\
	finish = chrono::high_resolution_clock::now();\
	cost = chrono::duration_cast<chrono::microseconds>(finish - start).count();\
	cout << "Call " << count << " times " << #callable << " cost:"\
		 << cost << "us. average:" << cost / count << "us." << endl;

using namespace std;
int main(int argc, char* argv[])
{
	int count = 10000;
	if (argc > 1) {
		count = stoi(argv[1]);
	}
	int sum = 0;
	decltype(chrono::high_resolution_clock::now()) start, finish;
	double cost = 0;
	COST(add, sum);
	sum = 0;
	COST(add, sum);
	sum = 0;
	COST(add, sum);
	sum = 0;
	COST(sad, sum);
	sum = 0;
	COST(sad, sum);
	sum = 0;
	COST(sub, sum);
	sum = 0;
	COST(sub, sum);
	sum = 0;
	COST(ssb, sum);
	sum = 0;
	COST(ssb, sum);
	return 0;
}
