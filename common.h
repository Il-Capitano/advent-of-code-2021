#ifndef AOC_COMMON_H
#define AOC_COMMON_H

#include <fmt/format.h>
#include "ranges.h"
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <span>

namespace fs = std::filesystem;

template<typename T>
struct span : std::span<T>, utils::collection_base<span<T>>
{
	using std::span<T>::span;

	span<T> slice(std::size_t begin, std::size_t end)
	{ return span(this->data() + begin, this->data() + end); }

	span<T const> slice(std::size_t begin, std::size_t end) const
	{ return span(this->data() + begin, this->data() + end); }
};

template<typename T>
struct vector : std::vector<T>, utils::collection_base<vector<T>>
{
	using std::vector<T>::vector;

	span<T> slice(std::size_t begin, std::size_t end)
	{ return span(this->data() + begin, this->data() + end); }

	span<T const> slice(std::size_t begin, std::size_t end) const
	{ return span(this->data() + begin, this->data() + end); }
};

struct string : std::string, utils::collection_base<string>
{
	string(std::string const &str) : std::string(str) {}
	string(std::string &&str) : std::string(std::move(str)) {}
	using std::string::string;
};

template<typename Key, typename Value>
struct unordered_map : std::unordered_map<Key, Value>, utils::collection_base<unordered_map<Key, Value>>
{
	using std::unordered_map<Key, Value>::unordered_map;
	using std::unordered_map<Key, Value>::contains;
};

template<typename Int>
Int parse_int(std::string_view str) noexcept
{
	Int result(0);
	for (auto const c : str)
	{
		if (c < '0' || c > '9')
		{
			break;
		}
		result *= Int(10);
		result += Int(c - '0');
	}
	return result;
}

template<typename Int, Int Base>
Int parse_int_base(std::string_view str) noexcept
{
	static_assert(Base <= 10 && Base >= 2);
	Int result(0);
	for (auto const c : str)
	{
		if (c < '0' || c > '9')
		{
			break;
		}
		result *= Int(Base);
		result += Int(c - '0');
	}
	return result;
}

template<typename ParseFunc>
auto read_file(fs::path const &file, ParseFunc parse_func)
{
	auto data_file = std::ifstream(file);

	vector<decltype(parse_func(std::declval<string&>()))> result;

	string line;
	while (std::getline(data_file, line))
	{
		result.push_back(parse_func(line));
	}

	return result;
}

inline vector<std::string_view> split_by(std::string_view str, std::string_view split_str)
{
	vector<std::string_view> result;
	auto it = str.find(split_str);
	while (it != std::string_view::npos)
	{
		result.push_back(str.substr(0, it));
		str = str.substr(it + split_str.size());
		it = str.find(split_str);
	}
	result.push_back(str);
	return result;
}

#endif // AOC_COMMON_H
