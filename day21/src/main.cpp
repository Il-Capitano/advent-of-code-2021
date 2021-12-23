#include "common.h"

static std::pair<int, int> read_input(fs::path const &filename)
{
	auto file = std::ifstream(filename);
	string line;
	std::getline(file, line);
	auto const player1 = parse_int<int>(std::string_view(line).substr(std::string_view("Player 1 starting position: ").length()));
	std::getline(file, line);
	auto const player2 = parse_int<int>(std::string_view(line).substr(std::string_view("Player 2 starting position: ").length()));
	return { player1, player2 };
}

struct deterministic_die
{
	int current_value = 1;
	int total_rolls = 0;

	int operator () (void)
	{
		this->total_rolls += 1;
		auto const result = this->current_value;
		this->current_value %= 100;
		this->current_value += 1;
		return result;
	}

	int get_total_rolls(void) const
	{
		return this->total_rolls;
	}
};

static int solution_part_1(std::pair<int, int> starting_positions)
{
	auto [player1_pos, player2_pos] = starting_positions;
	auto player1_score = 0;
	auto player2_score = 0;

	deterministic_die die{};
	bool is_player1s_turn = true;

	while (player1_score < 1000 && player2_score < 1000)
	{
		auto const first_roll = die();
		auto const second_roll = die();
		auto const third_roll = die();
		auto const total_roll = first_roll + second_roll + third_roll;
		if (is_player1s_turn)
		{
			player1_pos += total_roll;
			player1_pos = (player1_pos - 1) % 10 + 1;
			player1_score += player1_pos;
		}
		else
		{
			player2_pos += total_roll;
			player2_pos = (player2_pos - 1) % 10 + 1;
			player2_score += player2_pos;
		}
		is_player1s_turn = !is_player1s_turn;
	}

	return player1_score < 1000 ? player1_score * die.get_total_rolls() : player2_score * die.get_total_rolls();
}

// the total roll is in the range [3, 9]
static constexpr std::array<std::size_t, 7> die_universe_counts = {
	1, 3, 6, 7, 6, 3, 1
};

using table_t = std::array<std::array<std::array<std::array<std::size_t, 21>, 21>, 10>, 10>;

static table_t do_step(table_t const &table, bool is_player1s_turn, std::size_t &player1_wins, std::size_t &player2_wins)
{
	table_t result{};
	// player 1's turn
	for (auto const roll : utils::iota(3, std::size_t(10)))
	{
		for (auto const i : utils::iota(0, table.size()))
		{
			for (auto const j : utils::iota(0, table[i].size()))
			{
				for (auto const p1_score : utils::iota(0, table[i][j].size()))
				{
					for (auto const p2_score : utils::iota(0, table[i][j][p1_score].size()))
					{
						auto const universe_count = die_universe_counts[roll - 3];
						if (is_player1s_turn)
						{
							auto const new_i = (i + roll) % table.size();
							auto const score = new_i + 1;
							if (p1_score + score < 21)
							{
								result[new_i][j][p1_score + score][p2_score] += table[i][j][p1_score][p2_score] * universe_count;
							}
							else
							{
								player1_wins += table[i][j][p1_score][p2_score] * universe_count;
							}
						}
						else
						{
							auto const new_j = (j + roll) % table.size();
							auto const score = new_j + 1;
							if (p2_score + score < 21)
							{
								result[i][new_j][p1_score][p2_score + score] += table[i][j][p1_score][p2_score] * universe_count;
							}
							else
							{
								player2_wins += table[i][j][p1_score][p2_score] * universe_count;
							}
						}
					}
				}
			}
		}
	}

	return result;
}

static bool is_all_zero(table_t const &table)
{
	for (auto const i : utils::iota(0, table.size()))
	{
		for (auto const j : utils::iota(0, table[i].size()))
		{
			for (auto const p1_score : utils::iota(0, table[i][j].size()))
			{
				for (auto const p2_score : utils::iota(0, table[i][j][p1_score].size()))
				{
					if (table[i][j][p1_score][p2_score] != 0)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

static std::size_t solution_part_2(std::pair<int, int> starting_positions)
{
	table_t table{};
	bool is_player1s_turn = true;
	std::size_t player1_wins = 0;
	std::size_t player2_wins = 0;
	table[starting_positions.first - 1][starting_positions.second - 1][0][0] = 1;
	while (!is_all_zero(table))
	{
		table = do_step(table, is_player1s_turn, player1_wins, player2_wins);
		is_player1s_turn = !is_player1s_turn;
	}

	return std::max(player1_wins, player2_wins);
}

int main(void)
{
	auto const starting_positions = read_input("input.txt");
	auto const part_1_result = solution_part_1(starting_positions);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(starting_positions);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
