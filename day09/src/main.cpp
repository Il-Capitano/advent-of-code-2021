#include "common.h"

static int solution_part_1(vector<vector<int>> const &heightmap)
{
	int risk_sum = 0;
	auto const height = heightmap.size();
	auto const width  = heightmap[0].size();
	for (auto const i : utils::iota(0, height))
	{
		for (auto const j : utils::iota(0, width))
		{
			auto const is_low_point =
				(i == 0 || heightmap[i - 1][j] > heightmap[i][j])
				&& (i == height - 1 || heightmap[i + 1][j] > heightmap[i][j])
				&& (j == 0 || heightmap[i][j - 1] > heightmap[i][j])
				&& (j == width - 1 || heightmap[i][j + 1] > heightmap[i][j]);
			if (is_low_point)
			{
				risk_sum += 1 + heightmap[i][j];
			}
		}
	}
	return risk_sum;
}

static int floodfill(std::size_t i, std::size_t j, vector<vector<int>> const &heightmap, vector<vector<bool>> &occupied_map)
{
	auto const height = heightmap.size();
	auto const width  = heightmap[0].size();
	assert(occupied_map.size() == height);
	assert(occupied_map[0].size() == width);
	if (occupied_map[i][j] || heightmap[i][j] == 9)
	{
		return 0;
	}

	occupied_map[i][j] = true;

	return 1 + (i == 0 ? 0 : floodfill(i - 1, j, heightmap, occupied_map))
		+ (i == height - 1 ? 0 : floodfill(i + 1, j, heightmap, occupied_map))
		+ (j == 0 ? 0 : floodfill(i, j - 1, heightmap, occupied_map))
		+ (j == width - 1 ? 0 : floodfill(i, j + 1, heightmap, occupied_map));
}

static int solution_part_2(vector<vector<int>> const &heightmap)
{
	auto const height = heightmap.size();
	auto const width  = heightmap[0].size();
	auto occupied_map = vector<vector<bool>>(height, vector<bool>(width, false));

	std::array<int, 3> biggest_sizes{};

	auto const add_size = [&biggest_sizes](int const basin_size) {
		if (basin_size > biggest_sizes[2])
		{
			biggest_sizes[0] = biggest_sizes[1];
			biggest_sizes[1] = biggest_sizes[2];
			biggest_sizes[2] = basin_size;
		}
		else if (basin_size > biggest_sizes[1])
		{
			biggest_sizes[0] = biggest_sizes[1];
			biggest_sizes[1] = basin_size;
		}
		else if (basin_size > biggest_sizes[0])
		{
			biggest_sizes[0] = basin_size;
		}
	};

	for (auto const i : utils::iota(0, height))
	{
		for (auto const j : utils::iota(0, width))
		{
			auto const basin_size = floodfill(i, j, heightmap, occupied_map);
			add_size(basin_size);
		}
	}

	return biggest_sizes[0] * biggest_sizes[1] * biggest_sizes[2];
}

int main(void)
{
	auto const heightmap = read_file(
		"input.txt",
		[](auto const &line) {
			return line
				.transform([](auto const c) { return c - '0'; })
				.template collect<vector<int>>();
		}
	);
	auto const part_1_result = solution_part_1(heightmap);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(heightmap);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
