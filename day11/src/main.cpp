#include "common.h"

struct energy_data_t
{
	int level;
	bool has_flashed;
};

static int do_step(std::size_t i, std::size_t j, vector<vector<energy_data_t>> &energy_levels)
{
	if (energy_levels[i][j].has_flashed)
	{
		return 0;
	}

	energy_levels[i][j].level += 1;
	if (energy_levels[i][j].level > 9)
	{
		auto const height = energy_levels.size();
		auto const width = energy_levels[0].size();
		energy_levels[i][j].has_flashed = true;
		auto const i_min = i == 0 ? i : i - 1;
		auto const i_max = i == height - 1 ? i : i + 1;
		auto const j_min = j == 0 ? j : j - 1;
		auto const j_max = j == width - 1 ? j : j + 1;

		int result = 1;
		for (auto const i_prime : utils::iota(i_min, i_max + 1))
		{
			for (auto const j_prime : utils::iota(j_min, j_max + 1))
			{
				result += do_step(i_prime, j_prime, energy_levels);
			}
		}
		return result;
	}
	else
	{
		return 0;
	}
}

static int do_step(vector<vector<energy_data_t>> &energy_levels)
{
	auto const height = energy_levels.size();
	auto const width = energy_levels[0].size();
	int result = 0;
	for (auto const i : utils::iota(0, height))
	{
		for (auto const j : utils::iota(0, width))
		{
			result += do_step(i, j, energy_levels);
		}
	}

	for (auto &row : energy_levels)
	{
		for (auto &[level, has_flashed] : row)
		{
			if (has_flashed)
			{
				has_flashed = false;
				level = 0;
			}
		}
	}

	return result;
}

static int solution_part_1(vector<vector<int>> const &energy_levels_)
{
	auto energy_levels = energy_levels_
		.transform([](auto const &levels) {
			return levels
				.transform([](auto const level) { return energy_data_t{ level, false }; })
				.template collect<vector>();
		})
		.collect<vector>();
	return utils::iota(0, 100)
		.transform([&](auto const) { return do_step(energy_levels); })
		.sum();
}

static int solution_part_2(vector<vector<int>> const &energy_levels_)
{
	auto energy_levels = energy_levels_
		.transform([](auto const &levels) {
			return levels
				.transform([](auto const level) { return energy_data_t{ level, false }; })
				.template collect<vector>();
		})
		.collect<vector>();
	auto const height = energy_levels.size();
	auto const width = energy_levels[0].size();
	auto const size = static_cast<int>(height * width);
	for (int i = 1;; ++i)
	{
		if (do_step(energy_levels) == size)
		{
			return i;
		}
	}
}

int main(void)
{
	auto const energy_levels = read_file(
		"input.txt",
		[](auto const &line) {
			return line.transform([](auto const c) { return c - '0'; }).template collect<vector<int>>();
		}
	);
	auto const part_1_result = solution_part_1(energy_levels);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(energy_levels);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
