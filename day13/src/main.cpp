#include "common.h"

struct vec2
{
	int x = 0;
	int y = 0;
};

static bool operator == (vec2 lhs, vec2 rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

enum class direction
{
	x, y
};

struct fold_t
{
	direction dir;
	int coord;
};

struct input_data_t
{
	vector<vec2> dots;
	vector<fold_t> folds;
};

static input_data_t read_input(fs::path const &filename)
{
	input_data_t result;

	std::ifstream file(filename);
	string line;
	while (std::getline(file, line) && line != "")
	{
		auto const line_sv = std::string_view(line);
		auto const comma_it = line_sv.find(',');
		assert(comma_it != std::string_view::npos);
		auto const x = parse_int<int>(line_sv.substr(0, comma_it));
		auto const y = parse_int<int>(line_sv.substr(comma_it + 1));
		result.dots.push_back({ x, y });
	}

	while (std::getline(file, line))
	{
		auto const line_sv = std::string_view(line);
		if (line.starts_with("fold along x="))
		{
			auto const coord = parse_int<int>(line_sv.substr(std::string_view("fold along x=").length()));
			result.folds.push_back({ direction::x, coord });
		}
		else if (line.starts_with("fold along y="))
		{
			auto const coord = parse_int<int>(line_sv.substr(std::string_view("fold along y=").length()));
			result.folds.push_back({ direction::y, coord });
		}
		else
		{
			assert(false);
		}
	}

	return result;
}

static vector<vec2> do_fold(vector<vec2> const &dots, fold_t fold)
{
	vector<vec2> dots_after_fold;

	switch (fold.dir)
	{
	case direction::x:
	{
		for (
			auto const new_dot
			: dots.transform([coord = fold.coord](vec2 dot) {
				if (dot.x > coord)
				{
					return vec2{  coord - (dot.x - coord), dot.y };
				}
				else
				{
					return dot;
				}
			})
		)
		{
			if (!dots_after_fold.contains(new_dot))
			{
				dots_after_fold.push_back(new_dot);
			}
		}
		break;
	}
	case direction::y:
	{
		for (
			auto const new_dot
			: dots.transform([coord = fold.coord](vec2 dot) {
				if (dot.y > coord)
				{
					return vec2{ dot.x, coord - (dot.y - coord) };
				}
				else
				{
					return dot;
				}
			})
		)
		{
			if (!dots_after_fold.contains(new_dot))
			{
				dots_after_fold.push_back(new_dot);
			}
		}
		break;
	}
	}

	return dots_after_fold;
}

static std::size_t solution_part_1(input_data_t const &input_data)
{
	return do_fold(input_data.dots, input_data.folds[0]).size();
}

static void solution_part_2(input_data_t const &input_data)
{
	vector<vec2> dots = input_data.dots;
	for (auto const fold : input_data.folds)
	{
		dots = do_fold(dots, fold);
	}

	auto const max_x = dots.member<&vec2::x>().max();
	auto const max_y = dots.member<&vec2::y>().max();

	for (int y = 0; y <= max_y; ++y)
	{
		for (int x = 0; x <= max_x; ++x)
		{
			if (dots.contains(vec2{ x, y }))
			{
				fmt::print("#");
			}
			else
			{
				fmt::print(" ");
			}
		}
		fmt::print("\n");
	}
}

int main(void)
{
	auto const input_data = read_input("input.txt");
	auto const part_1_result = solution_part_1(input_data);
	fmt::print("part 1: {}\n", part_1_result);
	fmt::print("part 2:\n");
	solution_part_2(input_data);
	return 0;
}
