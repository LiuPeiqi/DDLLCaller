#ifndef LPQ_LIB_D_DLL_CALLER_h
#define LPQ_LIB_D_DLL_CALLER_h
/*
* Connot load const variable symbol, because maybe value equal 0 and raise a exception.
*/
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

#include "FunctionInfo.h"

#define LoadFunction(Model, Function)\
inline const decltype(&Function)& Model ## _ ## Function ## _singleton(\
								const char* filename,\
								const unsigned int line_no){\
	struct Function##_singleton_struct{\
		decltype(&Function) func;\
		Function##_singleton_struct(const char* filename, const unsigned int line_no){\
			auto func_info = FUNCTION_INFO(Function);\
			const auto& name = func_info.decoration_name;\
			func = reinterpret_cast<decltype(&Function)>(\
				DDLLCaller_GetProcAddress(GetModel(Model), name.c_str()));\
			if(0 == func){\
				std::stringstream sst;\
				sst << "Can not find the function address of '" << STR(Function)\
					<< "' in '" << ModelName(Model) << "'!("\
					<< filename <<"[" << line_no << "] call: " << name << ")"\
					<<std::endl;\
				throw std::domain_error(sst.str());\
			}\
		}\
	};\
	static Function##_singleton_struct f(filename, line_no);/*filename and line_no ware used on only first call*/\
	return f.func;\
}

#define GetFunction(Model, Function) Model ## _ ## Function ## _singleton(__FILE__, __LINE__)
#define GetFunction_FL_(Model, Function, filename, lineno) Model ## _ ## Function ## _singleton(filename, lineno)

#endif 
