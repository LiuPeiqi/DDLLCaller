#ifndef LPQ_LIB_FUNCTION_INFO_h
#define LPQ_LIB_FUNCTION_INFO_h

#ifdef _WIN32
#	ifdef _WIN64
/*actually x64 have only fastcall */
#		define LPQ_STDCALL 
#		define LPQ_CDECL 
#	else
#		define LPQ_STDCALL __stdcall
#		define LPQ_CDECL __cdecl
#	endif // _WIN64
#else
#	ifdef __x86_64
	inline std::string _StdCallConvention(const char* func, unsigned size) { return func; }
#		define LPQ_STDCALL
#		define LPQ_CDECL
#	else
#		define LPQ_STDCALL __attribute__((__stdcall__))
#		define LPQ_CDECL __attribute__((__cdecl__))
#	endif
#endif
#include <vector>
#include <sstream>
#include <string>

#include "FuncArgsSize.h"
namespace lpq {
	template<class> class FunctionInfo;

	template<class R, class... ArgTypes>
	class FunctionInfo<R LPQ_CDECL(ArgTypes...)> {
	public:
		const unsigned args_count = sizeof...(ArgTypes);
		const unsigned args_size = ArgsSize<ArgTypes...>::size;
		const std::string decoration_name;
		FunctionInfo(const char* f) :decoration_name(f) {}
	};

#ifdef _WIN64
	/*actually x64 have only fastcall */
#	define LPQ_HAVE_STDCALL 0
#elif defined __x86_64
#	define LPQ_HAVE_STDCALL 0
#else
#	define LPQ_HAVE_STDCALL 1
#endif

#if LPQ_HAVE_STDCALL
#	if defined _WIN32
	inline std::string _StdCallConvention(const char *func, unsigned size)
	{
		std::stringstream sst;
		sst << "_" << func << "@" << std::to_string(size);
		return std::move(sst.str());
	}
#	else
	inline std::string _StdCallConvention(const char* func, unsigned size) { return func; }
#	endif // _WIN32
	template<class R, class... ArgTypes>
	class FunctionInfo<R LPQ_STDCALL(ArgTypes...)> {
	public:
		const unsigned args_count = sizeof...(ArgTypes);
		const unsigned args_size = ArgsSize<ArgTypes...>::size;
		const std::string decoration_name;
		FunctionInfo(const char* f) :decoration_name(_StdCallConvention(f, args_size)) {}
	};
#endif // LPQ_HAVE_STDCALL
}
#define _STR(x) #x
#define STR(x) _STR(x)
#define FUNCTION_INFO(FUNCTION) lpq::FunctionInfo<decltype(FUNCTION)>(STR(FUNCTION))
#undef LPQ_CDECL
#undef LPQ_STDCALL
#endif
