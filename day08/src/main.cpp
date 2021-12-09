#include "common.h"
#include <bit>

struct digit_info_t
{
	vector<uint8_t> signal_patterns;
	vector<uint8_t> output_value;
};

static uint8_t string_to_digit(std::string_view s)
{
	uint8_t result = 0;
	for (auto const c : s)
	{
		if (c >= 'a' && c <= 'g')
		{
			auto const n = c - 'a';
			result |= 1 << n;
		}
	}
	return result;
}

static int solution_part_1(span<digit_info_t const> digit_infos)
{
	return digit_infos
		.transform([](auto const &digit_info) {
			return digit_info.output_value
				.transform([](auto const n) { return std::popcount(n); })
				.transform([](auto const count) { return count == 2 || count == 4 || count == 3 || count == 7 ? 1 : 0; })
				.sum();
		})
		.sum();
}

static int solution_part_2(vector<digit_info_t> digit_infos)
{
	for (auto &[signal_patterns, output_value] : digit_infos)
	{
		assert(signal_patterns.size() == 10);
		// determine 1, 4, 7, 8 by unique segment counts
		for (std::size_t i = 0; i < 10; ++i)
		{
			// 1
			if (std::popcount(signal_patterns[i]) == 2)
			{
				std::swap(signal_patterns[i], signal_patterns[1]);
				break;
			}
		}
		for (std::size_t i = 0; i < 10; ++i)
		{
			// 4
			if (std::popcount(signal_patterns[i]) == 4)
			{
				std::swap(signal_patterns[i], signal_patterns[4]);
				break;
			}
		}
		for (std::size_t i = 0; i < 10; ++i)
		{
			// 7
			if (std::popcount(signal_patterns[i]) == 3)
			{
				std::swap(signal_patterns[i], signal_patterns[7]);
				break;
			}
		}
		for (std::size_t i = 0; i < 10; ++i)
		{
			// 8
			if (std::popcount(signal_patterns[i]) == 7)
			{
				std::swap(signal_patterns[i], signal_patterns[8]);
				break;
			}
		}
		// 9 can be determined with the pattern 4 | 7, since only 9 and 8 satisfy the condition (x & (4 | 7)) == (4 | 7)
		uint8_t const unique_9 = signal_patterns[4] | signal_patterns[7];
		for (std::size_t const i : { 0, 2, 3, 5, 6, 9 })
		{
			if (i != 8 && (signal_patterns[i] & unique_9) == unique_9)
			{
				std::swap(signal_patterns[i], signal_patterns[9]);
				break;
			}
		}
		// 5 and 6 can be determined with 9 & ~1, since only these two and 8 and 9 satisfy the condition (x & (9 & ~1)) == (9 & ~1),
		// and they have different segment counts
		uint8_t const unique_56 = signal_patterns[9] & ~signal_patterns[1];
		for (std::size_t const i : { 0, 2, 3, 5, 6 })
		{
			// 5
			if ((signal_patterns[i] & unique_56) == unique_56 && std::popcount(signal_patterns[i]) == 5)
			{
				std::swap(signal_patterns[i], signal_patterns[5]);
				break;
			}
		}
		for (std::size_t const i : { 0, 2, 3, 6 })
		{
			// 6
			if ((signal_patterns[i] & unique_56) == unique_56)
			{
				std::swap(signal_patterns[i], signal_patterns[6]);
				break;
			}
		}
		// 0 can be determined by its unique segment count of 6
		for (std::size_t const i : { 0, 2, 3 })
		{
			if (std::popcount(signal_patterns[i]) == 6)
			{
				std::swap(signal_patterns[i], signal_patterns[0]);
				break;
			}
		}
		// differentiate 2 and 3 by using 1
		if ((signal_patterns[3] & signal_patterns[1]) == signal_patterns[1])
		{
			// nothing
		}
		else
		{
			std::swap(signal_patterns[2], signal_patterns[3]);
		}
	}

	return digit_infos
		.transform([](auto const &digit_info) {
			return digit_info.output_value
				.transform([&digit_info](auto const val) {
					return static_cast<int>(std::find(digit_info.signal_patterns.begin(), digit_info.signal_patterns.end(), val) - digit_info.signal_patterns.begin());
				})
				.reduce([](auto const lhs, auto const rhs) {
					return lhs * 10 + rhs;
				});
		})
		.sum();
}

int main(void)
{
	auto const digit_infos = read_file(
		"input.txt",
		[](auto const &line) {
			auto const segments = split_by(line, " | ");
			return digit_info_t{
				split_by(segments[0], " ").transform([](auto const s) { return string_to_digit(s); }).template collect<vector>(),
				split_by(segments[1], " ").transform([](auto const s) { return string_to_digit(s); }).template collect<vector>(),
			};
		}
	);
	auto const part_1_result = solution_part_1(digit_infos);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(digit_infos);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
