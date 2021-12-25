#include "common.h"
#include <cstdint>

enum class space_kind : std::uint8_t
{
	empty,
	east,
	south,
};

static std::pair<vector<vector<space_kind>>, bool> do_cucumber_movement_step(vector<vector<space_kind>> const &seafloor)
{
	auto const height = seafloor.size();
	auto const width  = seafloor[0].size();

	std::pair<vector<vector<space_kind>>, bool> result{};
	auto &result_seafloor = result.first;
	auto &has_moved = result.second;
	result_seafloor = vector<vector<space_kind>>(height, vector<space_kind>(width, space_kind::empty));

	// east facing movement
	for (auto const i : utils::iota(0, height))
	{
		for (auto const j : utils::iota(0, width))
		{
			if (seafloor[i][j] == space_kind::east)
			{
				auto const next_j = j == width - 1 ? 0 : j + 1;
				if (seafloor[i][next_j] == space_kind::empty)
				{
					result_seafloor[i][next_j] = space_kind::east;
					has_moved |= true;
				}
				else
				{
					result_seafloor[i][j] = space_kind::east;
				}
			}
		}
	}

	// south facing movement
	for (auto const i : utils::iota(0, height))
	{
		for (auto const j : utils::iota(0, width))
		{
			if (seafloor[i][j] == space_kind::south)
			{
				auto const next_i = i == height - 1 ? 0 : i + 1;
				if (
					result_seafloor[next_i][j] == space_kind::empty
					&& (seafloor[next_i][j] == space_kind::empty || seafloor[next_i][j] == space_kind::east)
				)
				{
					result_seafloor[next_i][j] = space_kind::south;
					has_moved |= true;
				}
				else
				{
					result_seafloor[i][j] = space_kind::south;
				}
			}
		}
	}

	return result;
}

static int solution_part_1(vector<vector<space_kind>> seafloor)
{
	int step_count = 0;
	while (true)
	{
		step_count += 1;
		auto [next_seafloor, has_moved] = do_cucumber_movement_step(seafloor);
		if (!has_moved)
		{
			break;
		}
		seafloor = std::move(next_seafloor);
	}

	return step_count;
}

int main(void)
{
	auto const seafloor = read_file("input.txt", [](auto const &line) {
		return line.transform([](auto const c) {
			switch (c)
			{
			case '.':
				return space_kind::empty;
			case '>':
				return space_kind::east;
			case 'v':
				return space_kind::south;
			default:
				assert(false);
				return space_kind::empty;
			}
		}).template collect<vector>();
	});
	auto const part_1_result = solution_part_1(seafloor);
	fmt::print("part 1: {}\n", part_1_result);
	// auto const part_2_result = solution_part_2(navigation_file);
	// fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
