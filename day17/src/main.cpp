#include "common.h"

struct vec2
{
	int x;
	int y;
};

struct target_area_t
{
	vec2 pos;
	vec2 size;
};

static int solution_part_1(target_area_t target_area)
{
	auto const max_y_speed = -target_area.pos.y - 1;
	return max_y_speed * (max_y_speed + 1) / 2;
}

static bool do_simulation(target_area_t target_area, vec2 speed)
{
	vec2 pos = { 0, 0 };
	while (pos.x <= target_area.pos.x + target_area.size.x && pos.y >= target_area.pos.y)
	{
		if (
			pos.x >= target_area.pos.x && pos.x <= target_area.pos.x + target_area.size.x
			&& pos.y >= target_area.pos.y && pos.y <= target_area.pos.y + target_area.size.y
		)
		{
			return true;
		}

		pos.x += speed.x;
		pos.y += speed.y;
		if (speed.x > 0)
		{
			speed.x -= 1;
		}
		else if (speed.x < 0)
		{
			speed.x += 1;
		}
		speed.y -= 1;
	}
	return false;
}

static int solution_part_2(target_area_t target_area)
{
	auto const min_y_speed = target_area.pos.y;
	auto const max_y_speed = -target_area.pos.y - 1;
	auto const min_x_speed = 0;
	auto const max_x_speed = target_area.pos.x + target_area.size.x;

	int result = 0;
	for (auto const x_speed : utils::iota(min_x_speed, max_x_speed + 1))
	{
		for (auto const y_speed : utils::iota(min_y_speed, max_y_speed + 1))
		{
			if (do_simulation(target_area, { x_speed, y_speed }))
			{
				result += 1;
			}
		}
	}
	return result;
}

int main(void)
{
	auto const target_area = read_file(
		"input.txt",
		[](auto const &line) {
			auto line_sv = std::string_view(line);
			assert(line_sv.starts_with("target area: x="));
			auto const x_range_begin_it = line_sv.find('=') + 1;
			auto const x_range_end_it = line_sv.find(',');
			auto const x_range_str = line_sv.substr(x_range_begin_it, x_range_end_it - x_range_begin_it);
			auto const x_range_dot_it = x_range_str.find("..");
			auto const x_begin = parse_int<int>(x_range_str.substr(0, x_range_dot_it - 0));
			auto const x_end = parse_int<int>(x_range_str.substr(x_range_dot_it + 2));
			line_sv = line_sv.substr(x_range_end_it + 1);
			auto const y_range_begin_it = line_sv.find('=') + 1;
			auto const y_range_end_it = line_sv.size();
			auto const y_range_str = line_sv.substr(y_range_begin_it, y_range_end_it - y_range_begin_it);
			auto const y_range_dot_it = y_range_str.find("..");
			auto const y_begin = parse_int<int>(y_range_str.substr(0, y_range_dot_it - 0));
			auto const y_end = parse_int<int>(y_range_str.substr(y_range_dot_it + 2));
			return target_area_t{ vec2{ x_begin, y_begin }, vec2{ x_end - x_begin, y_end - y_begin } };
		}
	)[0];
	auto const part_1_result = solution_part_1(target_area);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(target_area);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
