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
int STDCALL seven_int(int a, int b, int c, int d, int e, int f, int g);
double STDCALL seven_double(double a, double b, double c, double d, double e, double f, double g);
double STDCALL mix_eight(int a, double b, int c, double d, int e, double f, int g, double h);
LoadModel(TestDllAPI);
LoadFunction(TestDllAPI, add);
LoadFunction(TestDllAPI, sub);
LoadFunction(TestDllAPI, sad);
LoadFunction(TestDllAPI, ssb);
LoadFunction(TestDllAPI, seven_int);
LoadFunction(TestDllAPI, seven_double);
LoadFunction(TestDllAPI, mix_eight);


#include <chrono>
#include <iostream>
#include <string>

#define COST(callable, mem, ...)\
	start = chrono::high_resolution_clock::now();\
	for (int i = 0; i < count; ++i) {\
		mem += GetFunction(TestDllAPI, callable)(__VA_ARGS__);\
	}\
	finish = chrono::high_resolution_clock::now();\
	cost = chrono::duration_cast<chrono::microseconds>(finish - start).count();\
	cout << "Call " << count << " times " << #callable << " cost:"\
		 << cost << "us. average:" << cost / count << "us." << endl;

void STDCALL empty_func()
{
	return;
}

void empty_func1()
{
	return;
}

using namespace std;
int main(int argc, char* argv[])
{
	auto name = FUNCTION_INFO(empty_func).decoration_name;
	auto name1 = FUNCTION_INFO(empty_func1).decoration_name;
	int count = 10000;
	if (argc > 1) {
		count = stoi(argv[1]);
	}
	int sum = 0;
	decltype(chrono::high_resolution_clock::now()) start, finish;
	double cost = 0;
	COST(add, sum, i, 2);
	sum = 0;
	COST(add, sum, i, 2);
	sum = 0;
	COST(add, sum, i, 2);
	sum = 0;
	COST(sad, sum, i, 2);
	sum = 0;
	COST(sad, sum, i, 2);
	sum = 0;
	COST(sub, sum, i, 2);
	sum = 0;
	COST(sub, sum, i, 2);
	sum = 0;
	COST(ssb, sum, i, 2);
	sum = 0;
	COST(ssb, sum, i, 2);
	sum = 0;
	COST(seven_int, sum, i, i, i, i, i, i, i);
	sum = 0;
	COST(seven_int, sum, i, i, i, i, i, i, i);
	double a = 1.0;
	double b = 2.0;
	double c = 3.0;
	double d = 4.0;
	double e = 5.0;
	double f = 6.0;
	sum = 0;
	COST(seven_double, sum, a, b, c, d, e, f, 5.0);
	sum = 0;
	COST(seven_double, sum, a, b, c, d, e, f, 5.0);
	sum = 0;
	COST(mix_eight, sum, i, a, i, b, i, c, i, d);
	sum = 0;
	COST(mix_eight, sum, i, a, i, b, i, c, i, d);
	return 0;
}
