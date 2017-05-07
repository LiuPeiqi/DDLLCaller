#ifndef LPQ_LIB_FUNCTION_ARGEMENTS_SIZE
#define LPQ_LIB_FUNCTION_ARGEMENTS_SIZE
namespace lpq {
	template<typename T1, typename ...Rest>
	struct ArgsSize
	{
		enum { size = ArgsSize<T1>::size + ArgsSize<Rest...>::size };
	};

	template<typename T1>
	struct ArgsSize<T1>
	{
		enum { size = sizeof(T1) };;
	};

	template<>
	struct ArgsSize<void>
	{
		enum { size = 0 };
	};

	template<class> class FunctionArgsSize;

#define GENERATE_CLASS_IMP(CALL_CONVENTION) \
	template<class R, class... ArgTypes>\
	class FunctionArgsSize<R CALL_CONVENTION (ArgTypes...)> {\
	public:\
		enum { size = ArgsSize<ArgTypes...>::size };\
	};\
	template<class R>\
	class FunctionArgsSize<R CALL_CONVENTION ()> {\
	public:\
		enum { size = 0 };\
	};

	GENERATE_CLASS_IMP(__cdecl);
	GENERATE_CLASS_IMP(__stdcall);
	//GENERATE_CLASS_IMP(__fastcall);//may useful
	//GENERATE_CLASS_IMP(__thiscall);//may useful

#undef GENERATE_CLASS_IMP
}
#define GET_FUNC_ARGS_SIZE(func) lpq::FunctionArgsSize<decltype(func)>::size
#endif