#ifndef LPQ_LIB_FUNCTION_ARGEMENTS_SIZE
#define LPQ_LIB_FUNCTION_ARGEMENTS_SIZE
namespace lpq {
	template<typename ... Args>	struct ArgsSize;

	template<typename T, typename ...Rest>
	struct ArgsSize<T, Rest...>
	{
		enum { size = (sizeof(T) + ArgsSize<Rest...>::size) };
	};

	template<typename ... Args>	struct ArgsSize;

	template<>
	struct ArgsSize<>
	{
		enum { size = 0 };
	};
}
#endif