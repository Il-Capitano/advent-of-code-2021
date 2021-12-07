#include "common.h"

static int calculate_fuel_cost_1(span<int const> horizontal_positions, int position)
{
	return horizontal_positions.transform([position](auto const pos) { return std::abs(pos - position); }).sum();
}

static int solution_part_1(span<int const> horizontal_positions)
{
	auto const min = horizontal_positions.min();
	auto const max = horizontal_positions.max();
	return utils::iota(min, max)
		.transform([horizontal_positions](auto const pos) { return calculate_fuel_cost_1(horizontal_positions, pos); })
		.min();
}

static int calculate_fuel_cost_2(int a, int b)
{
	auto const diff = std::abs(a - b);
	return (diff * (diff + 1)) / 2;
}

static int calculate_fuel_cost_2(span<int const> horizontal_positions, int position)
{
	return horizontal_positions.transform([position](auto const pos) { return calculate_fuel_cost_2(pos, position); }).sum();
}

static int solution_part_2(span<int const> horizontal_positions)
{
	auto const min = horizontal_positions.min();
	auto const max = horizontal_positions.max();
	return utils::iota(min, max)
		.transform([horizontal_positions](auto const pos) { return calculate_fuel_cost_2(horizontal_positions, pos); })
		.min();
}

int main(void)
{
	auto const horizontal_positions = read_file(
		"input.txt",
		[](auto const &line) {
			return split_by(line, ",")
			.transform([](auto const s) { return parse_int<int>(s); })
			.template collect<vector>();
		}
	).front();
	auto const part_1_result = solution_part_1(horizontal_positions);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(horizontal_positions);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
