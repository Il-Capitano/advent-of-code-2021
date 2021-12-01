#include <iostream>
#include "common.h"

static int solution_part_1(span<int const> depth_data)
{
	return depth_data
		.adjacent()
		.transform([](auto const pair) { return pair.second > pair.first ? 1 : 0; })
		.sum();
}

static int solution_part_2(span<int const> depth_data)
{
	return depth_data
		.running_sum<3>()
		.adjacent()
		.transform([](auto const pair) { return pair.second > pair.first ? 1 : 0; })
		.sum();
}

int main(void)
{
	auto const depth_data = read_file("input.txt", [](auto const &line) { return parse_int<int>(line); });
	auto const part_1_result = solution_part_1(depth_data);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(depth_data);
	fmt::print("part 2: {}\n", part_2_result);

	return 0;
}
