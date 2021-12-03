#include "common.h"

static int solution_part_1(span<string const> report)
{
	auto const gamma_rate = report
		.transform([](auto const &line) {
			return line.transform([](auto const c) { return static_cast<std::size_t>(c == '1'); }).template collect<vector>();
		})
		.reduce([](auto lhs, auto const &rhs) {
			for (std::size_t i = 0; i < lhs.size(); ++i)
			{
				lhs[i] += rhs[i];
			}
			return lhs;
		})
		.transform([&report](auto const n) { return static_cast<int>(n > (report.size() / 2));})
		.reduce([](auto const lhs, auto const rhs) { return (lhs << 1) | rhs; });
	auto const epsilon_rate = ((1 << report[0].size()) - 1) & ~gamma_rate;
	return gamma_rate * epsilon_rate;
}

static int solution_part_2(span<string const> report)
{
	auto oxygen_generator_rating = utils::iota(0, report.size()).collect<vector>();
	auto scrubber_rating = utils::iota(0, report.size()).collect<vector>();

	for (std::size_t i : utils::iota(0, report[0].size()))
	{
		if (oxygen_generator_rating.size() == 1 && scrubber_rating.size() == 1)
		{
			break;
		}
		if (oxygen_generator_rating.size() > 1)
		{
			auto const one_count = oxygen_generator_rating
				.transform([&report, i](auto const index) { return static_cast<std::size_t>(report[index][i] == '1'); })
				.sum();
			auto const most_common = one_count > (oxygen_generator_rating.size() - 1) / 2 ? '1' : '0';
			oxygen_generator_rating = oxygen_generator_rating
				.filter([&report, i, most_common](auto const index) { return report[index][i] == most_common; })
				.collect<vector>();
		}
		if (scrubber_rating.size() > 1)
		{
			auto const one_count = scrubber_rating
				.transform([&report, i](auto const index) { return static_cast<std::size_t>(report[index][i] == '1'); })
				.sum();
			auto const least_common = one_count > (scrubber_rating.size() - 1) / 2 ? '0' : '1';
			scrubber_rating = scrubber_rating
				.filter([&report, i, least_common](auto const index) { return report[index][i] == least_common; })
				.collect<vector>();
		}
	}

	return parse_int_base<int, 2>(report[oxygen_generator_rating[0]]) * parse_int_base<int, 2>(report[scrubber_rating[0]]);
}

int main(void)
{
	auto const report = read_file("input.txt", [](auto const &line) { return line; });
	auto const part_1_result = solution_part_1(report);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(report);
	fmt::print("part 2: {}\n", part_2_result);

	return 0;
}
