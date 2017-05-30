#include <iostream>
#include <assert.h>
#include <exception>
#include <vector>
#include <tuple>
#include <type_traits>
#include "typelists.h"

enum class SpanType {SP_NONE, SP_DIM, SP_STED, SP_STEDSTEP};
class Span
{
public:
	long long index_[3];
	SpanType c_;
public:
	Span() : c_(SpanType::SP_NONE) {}
	Span(long long ind) 
		: c_(SpanType::SP_DIM)
	{
		index_[0] = ind;
	}
	Span &operator | (long long ind)
	{
		if (c_ == SpanType::SP_NONE)
		{
			index_[0] = ind;
			c_ = SpanType::SP_DIM;
		}
		else if (c_ == SpanType::SP_DIM)
		{
			index_[1] = ind;
			c_ = SpanType::SP_STED;
		}
		else if (c_ == SpanType::SP_STED)
		{
			index_[2] = ind;
			c_ = SpanType::SP_STEDSTEP;
		}
		else if (c_ == SpanType::SP_STEDSTEP)
			throw(std::exception("Span should contain at most 3 elems."));
		return *this;
	}
};

class SpanHelper
{
public:
	SpanHelper() {}
	Span operator | (long long ind)
	{
		return Span(ind);
	}
};

class SpanList
{
private:
	std::vector<Span> splist_;
	long long originD_;
	long long newD_;
public:
	SpanList() : originD_(0), newD_(0) {}
	SpanList(std::initializer_list<Span> list)
	{
		for (auto &i : list)
		{
			originD_++;
			if (i.c_ == SpanType::SP_STED || i.c_ == SpanType::SP_STEDSTEP)
				newD_++;
			splist_.push_back(i);
		}
	}
};

SpanHelper $;

struct A
{
	template <typename T1, typename T2>
	struct cond
	{
		static constexpr bool result = T2::result > 0;
	};

	template <typename T1, typename T2>
	struct body
	{
		using result = COUNTER<T2::result-1>;
	};
	static constexpr int result = UNTIL<cond, body, int, COUNTER<10> >::result::result;
};

int main()
{
	SpanList xplist({ $ | -1 | 1 | 2, $ | 1 | 3, $ | 5 });

	TypeList<int, float, double>::rest::first x = 1;
	
	Get<1, TypeList<int, float, double> >::result y = 1;

	int m = A::result;
	system("pause");
	return 0;
}