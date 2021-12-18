#include "common.h"

struct nesting_count_and_value
{
	int nesting_count;
	int value;
};

using snailfish_number_t = vector<nesting_count_and_value>;

static snailfish_number_t parse_snailfish_number(std::string_view str)
{
	int nesting_count = 0;
	snailfish_number_t result;
	for (auto const c : str)
	{
		switch (c)
		{
		case '[':
			nesting_count += 1;
			break;
		case ']':
			nesting_count -= 1;
			break;
		case ',':
			// nothing
			break;
		default:
			result.push_back({ nesting_count, c - '0' });
			break;
		}
	}
	return result;
}

static snailfish_number_t::iterator find_nested_pair(snailfish_number_t &number, int min_nesting_count)
{
	auto it = number.begin();
	auto const end = number.end();
	for (; it != end && it + 1 != end; ++it)
	{
		if (it->nesting_count >= min_nesting_count && (it + 1)->nesting_count == it->nesting_count)
		{
			return it;
		}
	}
	return end;
}

static snailfish_number_t::iterator find_large_value(snailfish_number_t &number)
{
	auto it = number.begin();
	auto const end = number.end();
	for (; it != end; ++it)
	{
		if (it->value >= 10)
		{
			return it;
		}
	}
	return end;
}

static bool reduce_number(snailfish_number_t &number)
{
	if (auto const nested_it = find_nested_pair(number, 5); nested_it != number.end())
	{
		if (nested_it != number.begin())
		{
			(nested_it - 1)->value += nested_it->value;
		}
		if ((nested_it + 2) != number.end())
		{
			(nested_it + 2)->value += (nested_it + 1)->value;
		}
		auto const other_nested_it = number.erase(nested_it);
		other_nested_it->nesting_count -= 1;
		other_nested_it->value = 0;
		return true;
	}

	if (auto const large_value_it = find_large_value(number); large_value_it != number.end())
	{
		auto const new_lhs = nesting_count_and_value{ large_value_it->nesting_count + 1, large_value_it->value / 2 };
		auto const new_rhs = nesting_count_and_value{ new_lhs.nesting_count, large_value_it->value - new_lhs.value };
		*large_value_it = new_rhs;
		number.insert(large_value_it, new_lhs);
		return true;
	}

	return false;
}

static snailfish_number_t add(snailfish_number_t const &lhs, snailfish_number_t const &rhs)
{
	snailfish_number_t result;
	result.reserve(lhs.size() + rhs.size());
	for (auto const [nesting_count, value] : lhs)
	{
		result.push_back({ nesting_count + 1, value });
	}
	for (auto const [nesting_count, value] : rhs)
	{
		result.push_back({ nesting_count + 1, value });
	}

	do {
		// fmt::print("reducing: {}\n", as_string(result));
	} while (reduce_number(result));
	return result;
}

static int calculate_magnitude(snailfish_number_t number)
{
	auto nested_it = find_nested_pair(number, 0);
	while (nested_it != number.end())
	{
		auto const new_value = 3 * nested_it->value + 2 * (nested_it + 1)->value;
		auto const other_nested_it = number.erase(nested_it);
		other_nested_it->nesting_count -= 1;
		other_nested_it->value = new_value;
		nested_it = find_nested_pair(number, 0);
	}
	assert(number.size() == 1);
	assert(number[0].nesting_count == 0);
	return number[0].value;
}

static int solution_part_1(span<snailfish_number_t const> snailfish_numbers)
{
	snailfish_number_t sum = snailfish_numbers[0];
	for (auto const &rhs : snailfish_numbers.slice(1))
	{
		sum = add(sum, rhs);
	}
	return calculate_magnitude(sum);
}

static int solution_part_2(span<snailfish_number_t const> snailfish_numbers)
{
	return snailfish_numbers
		.transform([snailfish_numbers](auto const &snailfish_number) {
			return snailfish_numbers
				.transform([&snailfish_number](auto const &inner_snalfish_number) {
					return calculate_magnitude(add(snailfish_number, inner_snalfish_number));
				})
				.max();
		})
		.max();
}

int main(void)
{
	auto const snailfish_numbers = read_file("input.txt", &parse_snailfish_number);
	auto const part_1_result = solution_part_1(snailfish_numbers);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(snailfish_numbers);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
