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
#		define STDCALL __attribute__((__stdcall__))
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

DLL_EXP int STDCALL seven_int(int a, int b, int c, int d, int e, int f, int g)
{
	a += b;
	c += d;
	e += f;
	a += (c + e);
	a += g;
	return a;
}

DLL_EXP double STDCALL seven_double(double a, double b, double c, double d, double e, double f, double g)
{
	a += b;
	c += d;
	e += f;
	a += (c + e);
	a += g;
	return a;
}

DLL_EXP double STDCALL mix_eight(int a, double b, int c, double d, int e, double f, int g, double h)
{
	h += (b + d + f);
	a += (c + e + g);
	return h / a;
}
