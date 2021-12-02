#include "common.h"

enum class direction
{
	forward,
	up,
	down,
};

static direction direction_from_string(std::string_view dir)
{
	if (dir == "forward")
	{
		return direction::forward;
	}
	else if (dir == "up")
	{
		return direction::up;
	}
	else if (dir == "down")
	{
		return direction::down;
	}
	assert(false);
	return direction::forward;
}

static int solution_part_1(span<std::pair<direction, int> const> commands)
{
	int position = 0;
	int depth = 0;
	for (auto const &[dir, count] : commands)
	{
		switch (dir)
		{
		case direction::forward:
			position += count;
			break;
		case direction::up:
			depth -= count;
			break;
		case direction::down:
			depth += count;
			break;
		}
	}
	return position * depth;
}

static int solution_part_2(span<std::pair<direction, int> const> commands)
{
	int position = 0;
	int aim = 0;
	int depth = 0;
	for (auto const &[dir, count] : commands)
	{
		switch (dir)
		{
		case direction::forward:
			position += count;
			depth += aim * count;
			break;
		case direction::up:
			aim -= count;
			break;
		case direction::down:
			aim += count;
			break;
		}
	}
	return position * depth;
}

int main(void)
{
	auto const commands = read_file("input.txt", [](auto const &line) -> std::pair<direction, int> {
		auto const space_it = line.find(' ');
		return { direction_from_string(line.substr(0, space_it)), parse_int<int>(line.substr(space_it + 1, line.size())) };
	});
	auto const part_1_result = solution_part_1(commands);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(commands);
	fmt::print("part 2: {}\n", part_2_result);

	return 0;
}
