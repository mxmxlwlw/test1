template <typename ...T>
struct TypeList;

template <int N, typename Tfirst, typename ...Trest>
struct Get
{
	static_assert(N >= 0, "N_MUST_BIG_THAN_0");
	static_assert(N <= (int)sizeof...(Trest), "INDEX_IS_OUT_OF_RANGE");
	using result = typename Get<N - 1, Trest...>::result;
};

template <typename Tfirst, typename ...Trest>
struct Get<0, Tfirst, Trest...>
{
	using result = Tfirst;
};

template <int N, typename ...Targ>
struct Get<N, TypeList<Targ...> >
{
	using result = typename Get<N, Targ...>::result;
};

template <typename Tfirst, typename ...Trest>
struct Split
{
	using first = Tfirst;
	using rest = TypeList<Trest...>;
};

template <typename ...T>
struct TypeList
{
	using first=typename Split<T...>::first;
	using rest=typename Split<T...>::rest;
};

template <bool, typename T1, typename T2>
struct IF
{
	using result = T2;
};

template <typename T1, typename T2>
struct IF<true, T1, T2>
{
	using result = T1;
};

template <template <typename, typename> typename cond, template <typename, typename> typename body, typename T1, typename T2>
struct WHILE
{
	template <typename T2> struct STOP { using result = T2; };
	using result = typename IF<cond<T1, T2>::result, 
		typename WHILE<
			cond, 
			body, 
			T1, 
			typename body<T1, T2>::result
		>, 
		STOP<T2> >::result::result;
};

template <template <typename, typename> typename cond, template <typename, typename> typename body, typename T1, typename T2>
struct UNTIL
{
	template <typename T2> struct STOP { using result = T2; };
	using result = typename IF<!cond<T1, T2>::result, 
		typename WHILE<
			cond, 
			body, 
			T1, 
			typename body<T1, T2>::result
		>, 
		STOP<T2> >::result::result;
};

template <template <typename, typename> typename cond, template <typename, typename> typename body, typename T1, typename T2>
struct DO_WHILE
{
	template <typename T2> struct STOP { using result = T2; };
	using doThisIter = typename body<T1, T2>::result;
	using result = typename IF<cond<T1, T2>::result, 
		typename DO_WHILE < 
			cond, 
			body, 
			T1,
			doThisIter
		>, 
		STOP<doThisIter> >::result::result;
};

template <template <typename, typename> typename cond, template <typename, typename> typename body, typename T1, typename T2>
struct DO_UNTIL
{
	template <typename T2> struct STOP { using result = T2; };
	using doThisIter = typename body<T1, T2>::result;
	using result = typename IF<!cond<T1, T2>::result, 
		typename DO_WHILE < 
			cond, 
			body, 
			T1,
			doThisIter
		>, 
		STOP<doThisIter> >::result::result;
};

template <typename T1, typename T2>
struct IS_SAME
{
	static constexpr bool result = false;
};

template <typename T>
struct IS_SAME<T, T>
{
	static constexpr bool result = true;
};

template <int N>
struct COUNTER 
{
	static constexpr int result = N;
};

