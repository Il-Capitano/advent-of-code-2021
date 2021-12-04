#include "common.h"
#include <array>

struct int_bool_pair_t
{
	int number;
	bool is_drawn;
};

using bingo_board_t = std::array<int_bool_pair_t, 5 * 5>;

struct bingo_data_t
{
	vector<int> drawn_numbers;
	vector<bingo_board_t> boards;
};

static bingo_data_t read_bingo_file(fs::path const &filename)
{
	std::ifstream file(filename);

	string line;
	std::getline(file, line);
	assert(file);

	bingo_data_t result;

	result.drawn_numbers = split_by(line, ",")
		.transform([](auto const num_str) { return parse_int<int>(num_str); })
		.collect<vector>();

	while (std::getline(file, line))
	{
		assert(line == "");
		result.boards.emplace_back();
		for (auto const i : utils::iota(0, size_t(5)))
		{
			std::getline(file, line);
			assert(file);
			std::stringstream ss(line);
			for (auto const j : utils::iota(0, size_t(5)))
			{
				ss >> result.boards.back()[i * 5 + j].number;
			}
		}
	}

	return result;
}

static bool is_winning_board(bingo_board_t const &board)
{
	// rows
	for (auto const i : utils::iota(0, size_t(5)))
	{
		bool is_winning = true;
		for (auto const j : utils::iota(0, size_t(5)))
		{
			is_winning &= board[i * 5 + j].is_drawn;
		}
		if (is_winning)
		{
			return true;
		}
	}

	// columns
	for (auto const j : utils::iota(0, size_t(5)))
	{
		bool is_winning = true;
		for (auto const i : utils::iota(0, size_t(5)))
		{
			is_winning &= board[i * 5 + j].is_drawn;
		}
		if (is_winning)
		{
			return true;
		}
	}

	return false;
}

static int solution_part_1(bingo_data_t bingo_data)
{
	for (auto const drawn_num : bingo_data.drawn_numbers)
	{
		for (auto &board : bingo_data.boards)
		{
			for (auto &[num, is_drawn] : board)
			{
				is_drawn |= num == drawn_num;
			}
			if (is_winning_board(board))
			{
				auto const non_drawn_sum = utils::to_range(board)
					.filter([](auto const square) { return !square.is_drawn; })
					.member<&int_bool_pair_t::number>()
					.sum();
				return non_drawn_sum * drawn_num;
			}
		}
	}
	return -1;
}

static int solution_part_2(bingo_data_t bingo_data)
{
	for (auto const drawn_num : bingo_data.drawn_numbers)
	{
		for (auto &board : bingo_data.boards)
		{
			for (auto &[num, is_drawn] : board)
			{
				is_drawn |= num == drawn_num;
			}
		}
	}

	for (auto const drawn_num : bingo_data.drawn_numbers.reversed())
	{
		for (auto &board : bingo_data.boards)
		{
			for (auto &[num, is_drawn] : board)
			{
				is_drawn &= !(num == drawn_num);
			}
			if (!is_winning_board(board))
			{
				auto const non_drawn_sum = utils::to_range(board)
					.filter([](auto const square) { return !square.is_drawn; })
					.member<&int_bool_pair_t::number>()
					.sum() - drawn_num;
				return non_drawn_sum * drawn_num;
			}
		}
	}
	return -1;
}

int main(void)
{
	auto const bingo_data = read_bingo_file("input.txt");
	auto const part_1_result = solution_part_1(bingo_data);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(bingo_data);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
