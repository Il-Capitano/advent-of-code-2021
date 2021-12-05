#include "common.h"

struct vent_position_t
{
	int x1;
	int y1;
	int x2;
	int y2;
};

static std::pair<int, int> parse_int_pair(std::string_view s)
{
	auto const comma_it = s.find(',');
	auto const x = parse_int<int>(s.substr(0, comma_it));
	auto const y = parse_int<int>(s.substr(comma_it + 1));
	return { x, y };
}

static int solution_part_1(span<vent_position_t const> vent_positions)
{
	std::size_t const x_size = vent_positions
		.transform([](auto const pos) { return std::max(pos.x1, pos.x2); })
		.max() + 1;
	std::size_t const y_size = vent_positions
		.transform([](auto const pos) { return std::max(pos.y1, pos.y2); })
		.max() + 1;
	vector<int> table;
	table.resize(x_size * y_size, 0);
	for (auto const [x1, y1, x2, y2] : vent_positions)
	{
		if (x1 == x2)
		{
			auto const y_min = std::min(y1, y2);
			auto const y_max = std::max(y1, y2);
			for (int y = y_min; y <= y_max; ++y)
			{
				table[y * x_size + x1] += 1;
			}
		}
		else if (y1 == y2)
		{
			auto const x_min = std::min(x1, x2);
			auto const x_max = std::max(x1, x2);
			for (int x = x_min; x <= x_max; ++x)
			{
				table[y1 * x_size + x] += 1;
			}
		}
	}

	return table.transform([](auto const n) { return n >= 2 ? 1 : 0; }).sum();
}

static int solution_part_2(span<vent_position_t const> vent_positions)
{
	std::size_t const x_size = vent_positions
		.transform([](auto const pos) { return std::max(pos.x1, pos.x2); })
		.max() + 1;
	std::size_t const y_size = vent_positions
		.transform([](auto const pos) { return std::max(pos.y1, pos.y2); })
		.max() + 1;
	vector<int> table;
	table.resize(x_size * y_size, 0);
	for (auto const [x1, y1, x2, y2] : vent_positions)
	{
		if (x1 == x2)
		{
			auto const y_min = std::min(y1, y2);
			auto const y_max = std::max(y1, y2);
			for (int y = y_min; y <= y_max; ++y)
			{
				table[y * x_size + x1] += 1;
			}
		}
		else if (y1 == y2)
		{
			auto const x_min = std::min(x1, x2);
			auto const x_max = std::max(x1, x2);
			for (int x = x_min; x <= x_max; ++x)
			{
				table[y1 * x_size + x] += 1;
			}
		}
		else
		{
			auto const x_min = std::min(x1, x2);
			auto const x_max = std::max(x1, x2);
			auto const y_min = x_min == x1 ? y1 : y2;
			auto const y_max = x_max == x1 ? y1 : y2;
			auto const y_step = y_max > y_min ? 1 : -1;
			assert(x_max - x_min == y_step * (y_max - y_min));
			for (int x = x_min, y = y_min; x <= x_max; ++x, y += y_step)
			{
				table[y * x_size + x] += 1;
			}
		}
	}

	return table.transform([](auto const n) { return n >= 2 ? 1 : 0; }).sum();
}

int main(void)
{
	auto const vent_positions = read_file(
		"input.txt",
		[](auto const &line_) {
			std::string_view const line = line_;
			auto const it = line.find(" -> ");
			assert(it != std::string_view::npos);
			auto const [x1, y1] = parse_int_pair(line.substr(0, it));
			auto const [x2, y2] = parse_int_pair(line.substr(it + 4));
			return vent_position_t{ x1, y1, x2, y2 };
		}
	);
	auto const part_1_result = solution_part_1(vent_positions);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(vent_positions);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
