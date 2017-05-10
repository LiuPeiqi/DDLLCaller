#ifdef _WIN32
#	define DLL_EXP __declspec(dllexport)
#	ifdef _WIN64
#		define STDCALL
#	else
#		define STDCALL __stdcall
#	endif // _WIN64
#else
#	define DLL_EXP
#	ifdef __x86_64
#		define STDCALL
#	else
#		define STDCALL ____attribute__((__stdcall__))
#	endif
#endif

DLL_EXP int add(int a, int b)
{
	return a + b;
}

DLL_EXP int STDCALL sad(int a, int b)
{
	return a + b;
}

DLL_EXP double sub(double a, double b)
{
	return a - b;
}

DLL_EXP double STDCALL ssb(double a, double b)
{
	return a - b;
}
