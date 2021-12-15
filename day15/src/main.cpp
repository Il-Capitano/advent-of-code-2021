#include "common.h"

static int find_lowest_risk_path(vector<vector<int>> const &risk_levels)
{
	auto const height = risk_levels.size();
	auto const width = risk_levels[0].size();
	auto const max_risk_level = std::numeric_limits<int>::max();
	auto total_risk_levels = vector<vector<int>>(height, vector<int>(width, max_risk_level));
	total_risk_levels[0][0] = 0; // starting position

	bool is_changed = false;
	do {
		is_changed = false;
		for (auto const i : utils::iota(0, height))
		{
			for (auto const j : utils::iota(0, width))
			{
				auto const original_surrounding_risk_level = total_risk_levels[i][j] - risk_levels[i][j];
				auto surrounding_risk_level = original_surrounding_risk_level;
				if (i != 0)
				{
					surrounding_risk_level = std::min(surrounding_risk_level, total_risk_levels[i - 1][j]);
				}
				if (i != height - 1)
				{
					surrounding_risk_level = std::min(surrounding_risk_level, total_risk_levels[i + 1][j]);
				}
				if (j != 0)
				{
					surrounding_risk_level = std::min(surrounding_risk_level, total_risk_levels[i][j - 1]);
				}
				if (j != width - 1)
				{
					surrounding_risk_level = std::min(surrounding_risk_level, total_risk_levels[i][j + 1]);
				}
				if (surrounding_risk_level != max_risk_level)
				{
				}
				is_changed |= surrounding_risk_level != original_surrounding_risk_level;
				total_risk_levels[i][j] = surrounding_risk_level + risk_levels[i][j];
			}
		}
	} while (is_changed);

	return total_risk_levels.back().back();
}

static int solution_part_1(vector<vector<int>> const &risk_levels)
{
	return find_lowest_risk_path(risk_levels);
}

static int solution_part_2(vector<vector<int>> risk_levels)
{
	auto const height = risk_levels.size();
	auto const width = risk_levels[0].size();

	// horizontal expansion
	for (auto const tile_index : utils::iota(size_t(1), size_t(5)))
	{
		auto const j_start = tile_index * width;
		for (auto const i : utils::iota(0, height))
		{
			risk_levels[i].resize(5 * width);
			for (auto const j : utils::iota(j_start, j_start + width))
			{
				risk_levels[i][j] = (risk_levels[i][j - width] % 9) + 1;
			}
		}
	}

	// vertical expansion
	risk_levels.resize(5 * height);
	for (auto const tile_index : utils::iota(size_t(1), size_t(5)))
	{
		auto const i_start = tile_index * height;
		for (auto const i : utils::iota(i_start, i_start + height))
		{
			risk_levels[i].resize(5 * width);
			for (auto const j : utils::iota(0, 5 * width))
			{
				risk_levels[i][j] = (risk_levels[i - height][j] % 9) + 1;
			}
		}
	}

	return find_lowest_risk_path(risk_levels);
}

int main(void)
{
	auto const risk_levels = read_file(
		"input.txt",
		[](auto const &line) {
			return line.transform([](auto const c) -> int { return c - '0'; }).template collect<vector>();
		}
	);
	auto const part_1_result = solution_part_1(risk_levels);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(risk_levels);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
