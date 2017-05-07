#ifndef LPQ_LIB_D_DLL_CALLER_h
#define LPQ_LIB_D_DLL_CALLER_h

#ifdef _WIN32
#	define DLL "dll"
#include <Windows.h>
#	define DDLLCaller_MODULE HMODULE
#	define DDLLCaller_LoadLibrary(model) LoadLibrary(model)
#	define DDLLCaller_GetProcAddress GetProcAddress
#else // Linux
#	define DLL "so"
#include <dlfcn.h>
#	define DDLLCaller_MODULE void*
#	define DDLLCaller_LoadLibrary(model) dlopen(model, RTLD_LAZY)
#	define DDLLCaller_GetProcAddress dlsym
#endif // _WIN32

#define _STR(x) #x
#define STR(x) _STR(x)
#define ModelName(Model) STR(Model) "." DLL

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>


#define LoadModel(Model)\
inline const DDLLCaller_MODULE Model##_singleton(){\
	struct Model##_singleton_struct{\
		DDLLCaller_MODULE model;\
		Model##_singleton_struct(){\
			model = DDLLCaller_LoadLibrary(ModelName(Model));\
			if(model == 0){\
				std::stringstream sst;\
				sst << "Can not load library: '" << ModelName(Model) << "'!"\
					<<std::endl;\
				throw std::domain_error(sst.str());\
			}\
		}\
	};\
	static  Model##_singleton_struct m;\
	return m.model;\
}

#define GetModel(Model) Model##_singleton()

#include "FuncArgsSize.h"
namespace lpq {
	inline const char* _StdCallConvention(const char *func, unsigned int size)
	{
		static std::vector<std::string> str_pool;//never release
		std::stringstream sst;
		sst << "_" << func << "@" << std::to_string(size);
		str_pool.push_back(sst.str());
		return str_pool.back().c_str();
	}
}
#define ONLY_ORIGINAL_NAME(func) STR(func)
#define CDECL_CONVENTION(func) "_" STR(func)
#define STDCALL_CONVENTION(func) lpq::_StdCallConvention(STR(func), GET_FUNC_ARGS_SIZE(func))

#define LoadFunction(Model, Function, CALL_CONVENTION)\
inline const decltype(&Function)& Model##Function##_singleton(\
								const char* filename,\
								const unsigned int line_no){\
	struct Function##_singleton_struct{\
		decltype(&Function) func ;\
		Function##_singleton_struct(const char* filename, const unsigned int line_no){\
			const char* func_name = CALL_CONVENTION(Function);\
			func = reinterpret_cast<decltype(&Function)>(\
				DDLLCaller_GetProcAddress(GetModel(Model), func_name));\
			if(func == 0){\
				std::stringstream sst;\
				sst << "Can not find the function address of '" << STR(Function)\
					<< "' in '" << ModelName(Model) << "'!("\
					<< filename <<"[" << line_no << "] call: " << func_name << ")"\
					<<std::endl;\
				throw std::domain_error(sst.str());\
			}\
		}\
	};\
	static Function##_singleton_struct f(filename, line_no);/*filename and line_no ware used on only first call*/\
	return f.func;\
}

#define LoadUsualFunction(Model, Function) LoadFunction(Model, Function, ONLY_ORIGINAL_NAME)
#define LoadCdeclFunction(Model, Function) LoadFunction(Model, Function, CDECL_CONVENTION)
#define LoadStdcallFunction(Model, Function) LoadFunction(Model, Function, STDCALL_CONVENTION)

#define GetFunction(Model,Function) Model##Function##_singleton(__FILE__, __LINE__)
#define GetFunction_FL_(Model,Function, filename, lineno) Model##Function##_singleton(filename, lineno)

#endif 
