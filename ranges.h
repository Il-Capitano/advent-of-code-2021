#ifndef _bz_ranges_h__
#define _bz_ranges_h__

#include <algorithm>
#include <utility>
#include <type_traits>
#include <array>
#include <cassert>
#include <vector>

namespace utils
{

template<typename Range>
struct universal_end_sentinel {};


namespace internal
{

template<typename Range>
struct range_base_filter
{
	template<typename Func>
	auto filter(Func &&func) const noexcept;
};

template<typename Collection>
struct collection_base_filter
{
	template<typename Func>
	auto filter(Func &&func) const noexcept;
};

template<typename Range>
struct range_base_transform
{
	template<typename Func>
	auto transform(Func &&func) const noexcept;
};

template<typename Collection>
struct collection_base_transform
{
	template<typename Func>
	auto transform(Func &&func) const noexcept;
};

template<typename Range>
struct range_base_member
{
	template<auto MemberPtr>
	auto member(void) const noexcept;
};

template<typename Collection>
struct collection_base_member
{
	template<auto MemberPtr>
	auto member(void) const noexcept;
};

template<typename Range>
struct range_base_collect
{
	template<template<typename ...Ts> typename Vec>
	auto collect(void) const;

	template<typename Vec>
	Vec collect(void) const;

	auto collect(void) const
	{ return this->template collect<std::vector>(); }
};

template<typename Range>
struct range_base_count
{
	std::size_t count(void) const noexcept;
};

template<typename Range>
struct range_base_is_any
{
	template<typename Func>
	bool is_any(Func &&func) const noexcept;
};

template<typename Collection>
struct collection_base_is_any
{
	template<typename Func>
	bool is_any(Func &&func) const noexcept;
};

template<typename Range>
struct range_base_is_all
{
	template<typename Func>
	bool is_all(Func &&func) const noexcept;
};

template<typename Collection>
struct collection_base_is_all
{
	template<typename Func>
	bool is_all(Func &&func) const noexcept;
};

template<typename Range>
struct range_base_contains
{
	template<typename T>
	bool contains(T &&val) const noexcept;
};

template<typename Collection>
struct collection_base_contains
{
	template<typename T>
	bool contains(T &&val) const noexcept;
};

template<typename Range>
struct range_base_for_each
{
	template<typename Func>
	void for_each(Func &&func) const noexcept;
};

template<typename Collection>
struct collection_base_for_each
{
	template<typename Func>
	void for_each(Func &&func) const noexcept;
};

template<typename Range>
struct range_base_sum
{
	auto sum(void) const noexcept;
};

template<typename Collection>
struct collection_base_sum
{
	auto sum(void) const noexcept;
};

template<typename Range>
struct range_base_partial_sum
{
	auto partial_sum(void) const noexcept;
};

template<typename Collection>
struct collection_base_partial_sum
{
	auto partial_sum(void) const noexcept;
};

template<typename Range>
struct range_base_running_sum
{
	template<std::size_t N>
	auto running_sum(void) const noexcept;
};

template<typename Collection>
struct collection_base_running_sum
{
	template<std::size_t N>
	auto running_sum(void) const noexcept;
};

template<typename Range>
struct range_base_adjacent
{
	auto adjacent(void) const noexcept;
};

template<typename Collection>
struct collection_base_adjacent
{
	auto adjacent(void) const noexcept;
};

template<typename Range>
struct range_base_reduce
{
	template<typename BinOp>
	auto reduce(BinOp &&bin_op) const noexcept;
};

template<typename Collection>
struct collection_base_reduce
{
	template<typename BinOp>
	auto reduce(BinOp &&bin_op) const noexcept;
};

template<typename Range>
struct range_base_max
{
	auto max(void) const noexcept;
	template<typename Cmp>
	auto max(Cmp &&cmp) const noexcept;
};

template<typename Collection>
struct collection_base_max
{
	auto max(void) const noexcept;
	template<typename Cmp>
	auto max(Cmp &&cmp) const noexcept;
};

template<typename Range>
struct range_base_min
{
	auto min(void) const noexcept;
	template<typename Cmp>
	auto min(Cmp &&cmp) const noexcept;
};

template<typename Collection>
struct collection_base_min
{
	auto min(void) const noexcept;
	template<typename Cmp>
	auto min(Cmp &&cmp) const noexcept;
};

template<typename Collection>
struct collection_base_sort
{
	void sort(void) noexcept;
	template<typename Cmp>
	void sort(Cmp &&cmp) noexcept;
};

template<typename Collection>
struct collection_base_reversed
{
	auto reversed(void) noexcept;
};

template<typename Range>
struct range_base_enumerate
{
	auto enumerate(void) const noexcept;
};

template<typename Collection>
struct collection_base_enumerate
{
	auto enumerate(void) const noexcept;
};

} // namespace internal


template<typename Range>
struct range_base :
	internal::range_base_filter     <Range>,
	internal::range_base_transform  <Range>,
	internal::range_base_member     <Range>,
	internal::range_base_collect    <Range>,
	internal::range_base_count      <Range>,
	internal::range_base_is_any     <Range>,
	internal::range_base_is_all     <Range>,
	internal::range_base_contains   <Range>,
	internal::range_base_for_each   <Range>,
	internal::range_base_sum        <Range>,
	internal::range_base_partial_sum<Range>,
	internal::range_base_running_sum<Range>,
	internal::range_base_adjacent   <Range>,
	internal::range_base_reduce     <Range>,
	internal::range_base_max        <Range>,
	internal::range_base_min        <Range>,
	internal::range_base_enumerate  <Range>
{};

template<typename Collection>
struct collection_base :
	internal::collection_base_filter     <Collection>,
	internal::collection_base_transform  <Collection>,
	internal::collection_base_member     <Collection>,
	internal::collection_base_is_any     <Collection>,
	internal::collection_base_is_all     <Collection>,
	internal::collection_base_contains   <Collection>,
	internal::collection_base_for_each   <Collection>,
	internal::collection_base_sum        <Collection>,
	internal::collection_base_partial_sum<Collection>,
	internal::collection_base_running_sum<Collection>,
	internal::collection_base_adjacent   <Collection>,
	internal::collection_base_reduce     <Collection>,
	internal::collection_base_max        <Collection>,
	internal::collection_base_min        <Collection>,
	internal::collection_base_sort       <Collection>,
	internal::collection_base_reversed   <Collection>,
	internal::collection_base_enumerate  <Collection>
{
	auto as_range(void) const noexcept;
};


template<typename ItType, typename EndType>
struct basic_range : range_base<basic_range<ItType, EndType>>
{
private:
	using self_t = basic_range<ItType, EndType>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;

public:
	basic_range(ItType it, EndType end)
		: _it(std::move(it)), _end(std::move(end))
	{}

	template<typename T, std::size_t N>
	basic_range(T (&arr)[N]) noexcept
		: _it(std::begin(arr)), _end(std::end(arr))
	{}

	template<typename T, std::size_t N>
	basic_range(T const (&arr)[N]) noexcept
		: _it(std::begin(arr)), _end(std::end(arr))
	{}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		++this->_it;
		return *this;
	}

	decltype(auto) operator * (void) const noexcept
	{ return *this->_it; }

	auto const &operator -> (void) const noexcept
	{ return this->_it; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename T>
struct iota_range : range_base<iota_range<T>>
{
private:
	using self_t = iota_range<T>;
private:
	T _it;
	T _end;

public:
	iota_range(T it, T end)
		: _it(std::move(it)), _end(std::move(end))
	{}

	template<typename T1, typename T2>
	iota_range(T1 it, T2 end)
		: _it(std::move(it)), _end(std::move(end))
	{}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		++this->_it;
		return *this;
	}

	decltype(auto) operator * (void) const noexcept
	{ return this->_it; }

	auto const &operator -> (void) const noexcept
	{ return &this->_it; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename ItType, typename EndType, typename FilterFuncType>
struct filter_range : range_base<filter_range<ItType, EndType, FilterFuncType>>
{
private:
	using self_t = filter_range<ItType, EndType, FilterFuncType>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;
	[[no_unique_address]] mutable FilterFuncType _filter_function;

public:
	filter_range(ItType it, EndType end, FilterFuncType func)
		: _it(std::move(it)), _end(std::move(end)), _filter_function(std::move(func))
	{
		while (this->_it != this->_end && !this->_filter_function(*this->_it))
		{
			++this->_it;
		}
	}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		do
		{
			++this->_it;
		} while (this->_it != this->_end && !this->_filter_function(*this->_it));
		return *this;
	}

	decltype(auto) operator * (void) const noexcept
	{ return *this->_it; }

	auto const &operator -> (void) const noexcept
	{ return this->_it; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename ItType, typename EndType, typename TransformFuncType>
struct transform_range : range_base<transform_range<ItType, EndType, TransformFuncType>>
{
private:
	using self_t = transform_range<ItType, EndType, TransformFuncType>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;
	[[no_unique_address]] mutable TransformFuncType _transform_func;

public:
	transform_range(ItType it, EndType end, TransformFuncType func)
		: _it(std::move(it)), _end(std::move(end)), _transform_func(std::move(func))
	{}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		++this->_it;
		return *this;
	}

	decltype(auto) operator * (void) const noexcept
	{ return this->_transform_func(*this->_it); }

	auto const &operator -> (void) const noexcept
	{ return this->_it; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename ItType, typename EndType>
struct enumerate_range : range_base<enumerate_range<ItType, EndType>>
{
private:
	using self_t = enumerate_range<ItType, EndType>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;
	std::size_t _index;

public:
	enumerate_range(ItType it, EndType end)
		: _it(std::move(it)), _end(std::move(end)), _index(0)
	{}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		++this->_it;
		++this->_index;
		return *this;
	}

	decltype(auto) operator * (void) const noexcept
	{ return std::pair<std::size_t, decltype(*this->_it)>{ this->_index, *this->_it }; }

	auto const &operator -> (void) const noexcept
	{ return this->_it; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename ItType, typename EndType>
struct partial_sum_range : range_base<partial_sum_range<ItType, EndType>>
{
private:
	using self_t = partial_sum_range<ItType, EndType>;
	using sum_t  = std::decay_t<decltype(*std::declval<ItType const &>())>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;
	sum_t _sum;

public:
	partial_sum_range(ItType it, EndType end)
		: _it(std::move(it)), _end(std::move(end)), _sum(this->_it != this->_end ? *this->_it : sum_t{})
	{}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		++this->_it;
		if (!this->at_end())
		{
			this->_sum += *this->_it;
		}
		return *this;
	}

	sum_t const &operator * (void) const noexcept
	{ return this->_sum; }

	sum_t const *operator -> (void) const noexcept
	{ return &this->_sum; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename ItType, typename EndType, std::size_t N>
struct running_sum_range : range_base<running_sum_range<ItType, EndType, N>>
{
	static_assert(N != 0, "invalid size 0 for running_sum_range");
private:
	using self_t = running_sum_range<ItType, EndType, N>;
	using sum_t  = std::decay_t<decltype(*std::declval<ItType const &>())>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;
	std::array<sum_t, N> _sum;

public:
	running_sum_range(ItType it, EndType end)
		: _it(std::move(it)), _end(std::move(end)), _sum()
	{
		for (std::size_t i = 0; i < N; ++i)
		{
			if (i != 0)
			{
				++this->_it;
			}
			if (this->at_end())
			{
				break;
			}
			auto &&new_val = *this->_it;
			for (std::size_t j = 0; j < i + 1; ++j)
			{
				this->_sum[j] += new_val;
			}
		}
	}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		++this->_it;
		if (!this->at_end())
		{
			auto &&new_val = *this->_it;
			for (std::size_t i = 0; i < this->_sum.size() - 1; ++i)
			{
				this->_sum[i] = std::move(this->_sum[i + 1]) + new_val;
			}
			this->_sum[this->_sum.size() - 1] = new_val;
		}
		return *this;
	}

	sum_t const &operator * (void) const noexcept
	{ return this->_sum[0]; }

	sum_t const *operator -> (void) const noexcept
	{ return &this->_sum[0]; }

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};

template<typename ItType, typename EndType>
struct adjacent_range : range_base<adjacent_range<ItType, EndType>>
{
private:
	using self_t  = adjacent_range<ItType, EndType>;
	using deref_t = decltype(*std::declval<ItType const &>());
	using prev_t  = std::decay_t<deref_t>;
private:
	ItType _it;
	[[no_unique_address]] EndType _end;
	prev_t _prev;

public:
	adjacent_range(ItType it, EndType end)
		: _it(std::move(it)), _end(std::move(end)), _prev([this]() -> prev_t {
			if (this->_it != this->_end)
			{
				auto result = *this->_it;
				++this->_it;
				return result;
			}
			else
			{
				return prev_t{};
			}
		}())
	{}

	bool at_end(void) const noexcept
	{ return this->_it == this->_end; }

	self_t &operator ++ (void)
	{
		this->_prev = *this->_it;
		++this->_it;
		return *this;
	}

	std::pair<prev_t const &, deref_t> operator * (void) const noexcept
	{
		return { this->_prev, *this->_it };
	}

	friend bool operator == (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return lhs.at_end(); }

	friend bool operator == ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return rhs.at_end(); }

	friend bool operator != (self_t const &lhs, [[maybe_unused]] universal_end_sentinel<self_t> rhs) noexcept
	{ return !lhs.at_end(); }

	friend bool operator != ([[maybe_unused]] universal_end_sentinel<self_t> lhs, self_t const &rhs) noexcept
	{ return !rhs.at_end(); }


	self_t begin(void) const noexcept
	{ return *this; }

	universal_end_sentinel<self_t> end(void) const noexcept
	{ return universal_end_sentinel<self_t>{}; }

	decltype(auto) front(void) const noexcept
	{ return **this; }
};


template<typename ItType, typename EndType>
basic_range(ItType it, EndType end) -> basic_range<ItType, EndType>;

template<typename T, std::size_t N>
basic_range(T (&arr)[N]) -> basic_range<decltype(std::begin(arr)), decltype(std::end(arr))>;

template<typename T, std::size_t N>
basic_range(T const (&arr)[N]) -> basic_range<decltype(std::begin(arr)), decltype(std::end(arr))>;

template<typename T>
iota_range(T it, T end) -> iota_range<T>;

template<typename T1, typename T2>
iota_range(T1 it, T2 end) -> iota_range<std::common_type_t<T1, T2>>;

template<typename ItType, typename EndType, typename FilterFuncType>
filter_range(ItType it, EndType end, FilterFuncType func) -> filter_range<ItType, EndType, FilterFuncType>;

template<typename ItType, typename EndType, typename TransformFuncType>
transform_range(ItType it, EndType end, TransformFuncType func) -> transform_range<ItType, EndType, TransformFuncType>;

template<typename ItType, typename EndType>
enumerate_range(ItType it, EndType end) -> enumerate_range<ItType, EndType>;

template<typename ItType, typename EndType>
partial_sum_range(ItType it, EndType end) -> partial_sum_range<ItType, EndType>;

template<typename ItType, typename EndType>
adjacent_range(ItType it, EndType end) -> adjacent_range<ItType, EndType>;


template<typename Collection>
auto collection_base<Collection>::as_range(void) const noexcept
{
	auto const self = static_cast<Collection const *>(this);
	return basic_range{ self->begin(), self->end() };
}

template<
	typename Range,
	std::enable_if_t<std::is_reference_v<Range> && !std::is_rvalue_reference_v<Range>, int> = 0
>
auto to_range(Range &&range) noexcept
{
	return basic_range{
		std::begin(std::forward<Range>(range)),
		std::end  (std::forward<Range>(range))
	};
}

template<typename T1, typename T2>
auto iota(T1 begin, T2 end) noexcept
{
	return iota_range{
		std::move(begin),
		std::move(end)
	};
}

template<
	typename Range, typename Func,
	std::enable_if_t<std::is_reference_v<Range> && !std::is_rvalue_reference_v<Range>, int> = 0
>
auto filter(Range &&range, Func &&func) noexcept
{
	return filter_range{
		std::forward<Range>(range).begin(),
		std::forward<Range>(range).end(),
		std::forward<Func>(func)
	};
}

template<
	typename Range, typename Func,
	std::enable_if_t<std::is_reference_v<Range> && !std::is_rvalue_reference_v<Range>, int> = 0
>
auto transform(Range &&range, Func &&func) noexcept
{
	return transform_range{
		std::forward<Range>(range).begin(),
		std::forward<Range>(range).end(),
		std::forward<Func>(func)
	};
}


namespace internal
{

template<typename Range>
template<typename Func>
auto range_base_filter<Range>::filter(Func &&func) const noexcept
{ return ::utils::filter(*static_cast<Range const *>(this), std::forward<Func>(func)); }

template<typename Collection>
template<typename Func>
auto collection_base_filter<Collection>::filter(Func &&func) const noexcept
{ return static_cast<Collection const *>(this)->as_range().filter(std::forward<Func>(func)); }

template<typename Range>
template<typename Func>
auto range_base_transform<Range>::transform(Func &&func) const noexcept
{ return ::utils::transform(*static_cast<Range const *>(this), std::forward<Func>(func)); }

template<typename Collection>
template<typename Func>
auto collection_base_transform<Collection>::transform(Func &&func) const noexcept
{ return static_cast<Collection const *>(this)->as_range().transform(std::forward<Func>(func)); }

template<typename Range>
template<auto MemberPtr>
auto range_base_member<Range>::member(void) const noexcept
{
	return ::utils::transform(
		*static_cast<Range const *>(this),
		[](auto const &value) -> auto const & { return value.*MemberPtr; }
	);
}

template<typename Collection>
template<auto MemberPtr>
auto collection_base_member<Collection>::member(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().template member<MemberPtr>(); }

namespace internal
{

template<typename T, typename ...Ts>
struct has_emplace_back
{
	using yes = int;
	using no = char;

	template<typename U, typename ...Us>
	static auto test(int) -> decltype(std::declval<U>().emplace_back(std::declval<Us>()...), yes{});
	template<typename U, typename ...Us>
	static no test(...);

	static constexpr bool value = std::is_same_v<decltype(test<T, Ts...>(42)), yes>;
};

} // namespace internal

template<typename Range>
template<template<typename ...Ts> typename Vec>
auto range_base_collect<Range>::collect(void) const
{
	auto const self = static_cast<Range const *>(this);
	Vec<std::decay_t<decltype(self->operator*())>> result;
	for (auto &&it : *self)
	{
		if constexpr (internal::has_emplace_back<decltype(result), decltype(it)>::value)
		{
			result.emplace_back(std::forward<decltype(it)>(it));
		}
		else
		{
			result.push_back(std::forward<decltype(it)>(it));
		}
	}
	return result;
}

template<typename Range>
template<typename Vec>
Vec range_base_collect<Range>::collect(void) const
{
	auto const self = static_cast<Range const *>(this);
	Vec result;
	for (auto &&it : *self)
	{
		if constexpr (internal::has_emplace_back<decltype(result), decltype(it)>::value)
		{
			result.emplace_back(std::forward<decltype(it)>(it));
		}
		else
		{
			result.push_back(std::forward<decltype(it)>(it));
		}
	}
	return result;
}

template<typename Range>
std::size_t range_base_count<Range>::count(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	auto &&it = self->begin();
	auto &&end = self->end();
	std::size_t result = 0;
	while (it != end)
	{
		++it;
		++result;
	}
	return result;
}

template<typename Range>
template<typename Func>
bool range_base_is_any<Range>::is_any(Func &&func) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	for (auto &&it : *self)
	{
		if (func(std::forward<decltype(it)>(it)))
		{
			return true;
		}
	}
	return false;
}

template<typename Collection>
template<typename Func>
bool collection_base_is_any<Collection>::is_any(Func &&func) const noexcept
{ return static_cast<Collection const *>(this)->as_range().is_any(std::forward<Func>(func)); }

template<typename Range>
template<typename Func>
bool range_base_is_all<Range>::is_all(Func &&func) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	for (auto &&it : *self)
	{
		if (!func(std::forward<decltype(it)>(it)))
		{
			return false;
		}
	}
	return true;
}

template<typename Collection>
template<typename Func>
bool collection_base_is_all<Collection>::is_all(Func &&func) const noexcept
{ return static_cast<Collection const *>(this)->as_range().is_all(std::forward<Func>(func)); }

template<typename Range>
template<typename T>
bool range_base_contains<Range>::contains(T &&val) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	for (auto &&it : *self)
	{
		if (std::forward<decltype(it)>(it) == val)
		{
			return true;
		}
	}
	return false;
}

template<typename Collection>
template<typename T>
bool collection_base_contains<Collection>::contains(T &&val) const noexcept
{ return static_cast<Collection const *>(this)->as_range().contains(std::forward<T>(val)); }

template<typename Range>
template<typename Func>
void range_base_for_each<Range>::for_each(Func &&func) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	for (auto &&it : *self)
	{
		func(std::forward<decltype(it)>(it));
	}
}

template<typename Collection>
template<typename Func>
void collection_base_for_each<Collection>::for_each(Func &&func) const noexcept
{ return static_cast<Collection const *>(this)->as_range().for_each(std::forward<Func>(func)); }

template<typename Range>
auto range_base_sum<Range>::sum(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	std::decay_t<decltype(self->operator*())> result{};
	for (auto &&it : *self)
	{
		result += std::forward<decltype(it)>(it);
	}
	return result;
}

template<typename Collection>
auto collection_base_sum<Collection>::sum(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().sum(); }

template<typename Range>
auto range_base_partial_sum<Range>::partial_sum(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	return partial_sum_range{ self->begin(), self->end() };
}

template<typename Collection>
auto collection_base_partial_sum<Collection>::partial_sum(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().partial_sum(); }

template<typename Range>
template<std::size_t N>
auto range_base_running_sum<Range>::running_sum(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	return running_sum_range<decltype(self->begin()), decltype(self->end()), N>{ self->begin(), self->end() };
}

template<typename Collection>
template<std::size_t N>
auto collection_base_running_sum<Collection>::running_sum(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().template running_sum<N>(); }

template<typename Range>
auto range_base_adjacent<Range>::adjacent(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	return adjacent_range{ self->begin(), self->end() };
}

template<typename Collection>
auto collection_base_adjacent<Collection>::adjacent(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().adjacent(); }

template<typename Range>
template<typename BinOp>
auto range_base_reduce<Range>::reduce(BinOp &&bin_op) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	auto &&it = self->begin();
	auto &&end = self->end();
	assert(it != end);
	std::decay_t<decltype(self->operator*())> result(*it);
	++it;
	for (; it != end; ++it)
	{
		result = bin_op(std::move(result), *it);
	}
	return result;
}

template<typename Collection>
template<typename BinOp>
auto collection_base_reduce<Collection>::reduce(BinOp &&bin_op) const noexcept
{ return static_cast<Collection const *>(this)->as_range().reduce(std::forward<BinOp>(bin_op)); }

template<typename Range>
auto range_base_max<Range>::max(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	auto &&it = self->begin();
	auto &&end = self->end();
	assert(it != end);
	std::decay_t<decltype(self->operator*())> result(*it);
	++it;
	for (; it != end; ++it)
	{
		auto &&it_val = *it;
		if (result < it_val)
		{
			result = std::forward<decltype(it_val)>(it_val);
		}
	}
	return result;
}

template<typename Collection>
auto collection_base_max<Collection>::max(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().max(); }

template<typename Range>
template<typename Cmp>
auto range_base_max<Range>::max(Cmp &&cmp) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	auto &&it = self->begin();
	auto &&end = self->end();
	assert(it != end);
	std::decay_t<decltype(self->operator*())> result(*it);
	++it;
	for (; it != end; ++it)
	{
		auto &&it_val = *it;
		if (cmp(result, it_val))
		{
			result = std::forward<decltype(it_val)>(it_val);
		}
	}
	return result;
}

template<typename Collection>
template<typename Cmp>
auto collection_base_max<Collection>::max(Cmp &&cmp) const noexcept
{ return static_cast<Collection const *>(this)->as_range().max(std::forward<Cmp>(cmp)); }

template<typename Range>
auto range_base_min<Range>::min(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	auto &&it = self->begin();
	auto &&end = self->end();
	assert(it != end);
	std::decay_t<decltype(self->operator*())> result(*it);
	++it;
	for (; it != end; ++it)
	{
		auto &&it_val = *it;
		if (it_val < result)
		{
			result = std::forward<decltype(it_val)>(it_val);
		}
	}
	return result;
}

template<typename Collection>
auto collection_base_min<Collection>::min(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().min(); }

template<typename Range>
template<typename Cmp>
auto range_base_min<Range>::min(Cmp &&cmp) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	auto &&it = self->begin();
	auto &&end = self->end();
	assert(it != end);
	std::decay_t<decltype(self->operator*())> result(*it);
	++it;
	for (; it != end; ++it)
	{
		auto &&it_val = *it;
		if (cmp(it_val, result))
		{
			result = std::forward<decltype(it_val)>(it_val);
		}
	}
	return result;
}

template<typename Collection>
template<typename Cmp>
auto collection_base_min<Collection>::min(Cmp &&cmp) const noexcept
{ return static_cast<Collection const *>(this)->as_range().min(std::forward<Cmp>(cmp)); }

template<typename Collection>
void collection_base_sort<Collection>::sort(void) noexcept
{
	auto const self = static_cast<Collection *>(this);
	std::sort(self->begin(), self->end());
}

template<typename Collection>
template<typename Cmp>
void collection_base_sort<Collection>::sort(Cmp &&cmp) noexcept
{
	auto const self = static_cast<Collection *>(this);
	std::sort(self->begin(), self->end(), std::forward<Cmp>(cmp));
}

template<typename Collection>
auto collection_base_reversed<Collection>::reversed(void) noexcept
{
	auto const self = static_cast<Collection *>(this);
	return basic_range{ self->rbegin(), self->rend() };
}

template<typename Range>
auto range_base_enumerate<Range>::enumerate(void) const noexcept
{
	auto const self = static_cast<Range const *>(this);
	return enumerate_range{ self->begin(), self->end() };
}

template<typename Collection>
auto collection_base_enumerate<Collection>::enumerate(void) const noexcept
{ return static_cast<Collection const *>(this)->as_range().enumerate(); }

} // namespace internal

} // namespace utils

#endif // _bz_ranges_h__
