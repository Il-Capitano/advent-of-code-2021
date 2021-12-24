#include "common.h"
#include <unordered_set>
#include <queue>

struct burrow_t
{
	std::array<char, 7> hallway;
	std::array<std::array<char, 2>, 4> side_rooms;

	bool operator == (burrow_t const &other) const = default;
};

struct full_burrow_t
{
	std::array<char, 7> hallway;
	std::array<std::array<char, 4>, 4> side_rooms;

	bool operator == (full_burrow_t const &other) const = default;
};

static void print_burrow(burrow_t const &burrow)
{
	string result = "#############\n";

	result += '#';
	result += burrow.hallway[0] == 0 ? '.' : burrow.hallway[0];
	for (auto const i : utils::iota(1, 5))
	{
		auto const h = burrow.hallway[i];
		if (h == 0)
		{
			result += '.';
		}
		else
		{
			result += h;
		}
		result += '.';
	}
	result += burrow.hallway[5] == 0 ? '.' : burrow.hallway[5];
	result += burrow.hallway[6] == 0 ? '.' : burrow.hallway[6];
	result += "#\n";

	result += "###";
	for (auto const [top, _] : burrow.side_rooms)
	{
		if (top == 0)
		{
			result += '.';
		}
		else
		{
			result += top;
		}
		result += '#';
	}
	result += "##\n";

	result += "  #";
	for (auto const [_, bottom] : burrow.side_rooms)
	{
		if (bottom == 0)
		{
			result += '.';
		}
		else
		{
			result += bottom;
		}
		result += '#';
	}
	result += '\n';
	result += "  #########\n";
	fmt::print("{}", result);
}

static void print_burrow(full_burrow_t const &burrow)
{
	string result = "#############\n";

	result += '#';
	result += burrow.hallway[0] == 0 ? '.' : burrow.hallway[0];
	for (auto const i : utils::iota(1, 5))
	{
		auto const h = burrow.hallway[i];
		if (h == 0)
		{
			result += '.';
		}
		else
		{
			result += h;
		}
		result += '.';
	}
	result += burrow.hallway[5] == 0 ? '.' : burrow.hallway[5];
	result += burrow.hallway[6] == 0 ? '.' : burrow.hallway[6];
	result += "#\n";

	for (auto const room_pos_index : utils::iota(0, burrow.side_rooms[0].size()))
	{
		result += "  #";
		for (auto const room_index : utils::iota(0, burrow.side_rooms.size()))
		{
			auto const c = burrow.side_rooms[room_index][room_pos_index];
			if (c == 0)
			{
				result += '.';
			}
			else
			{
				result += c;
			}
			result += '#';
		}
		result += '\n';
	}
	result += "  #########\n";
	fmt::print("{}", result);
}

template<>
struct std::hash<burrow_t>
{
	std::size_t operator () (burrow_t const &burrow) const
	{
		std::size_t result = 0;
		for (auto const h : burrow.hallway)
		{
			result *= 100;
			result += h;
		}
		for (auto const [top, bottom] : burrow.side_rooms)
		{
			result *= 100;
			result += top;
			result *= 100;
			result += bottom;
		}
		return result;
	}
};

template<>
struct std::hash<full_burrow_t>
{
	std::size_t operator () (full_burrow_t const &burrow) const
	{
		std::size_t result = 0;
		for (auto const h : burrow.hallway)
		{
			result *= 100;
			result += h;
		}
		for (auto const spaces : burrow.side_rooms)
		{
			for (auto const c : spaces)
			{
				result *= 100;
				result += c;
			}
		}
		return result;
	}
};

static constexpr int amphipod_movement_costs[] = { 1, 10, 100, 1000 };
static constexpr burrow_t end_burrow = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{{ { 'A', 'A' }, { 'B', 'B' }, { 'C', 'C' }, { 'D', 'D' } }}
};
static constexpr full_burrow_t end_full_burrow = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{{ { 'A', 'A', 'A', 'A' }, { 'B', 'B', 'B', 'B' }, { 'C', 'C', 'C', 'C' }, { 'D', 'D', 'D', 'D' } }}
};

static burrow_t read_input(fs::path const &filename)
{
	burrow_t result{};
	auto file = std::ifstream(filename);
	string line;
	std::size_t room_pos_index = 0;
	while (std::getline(file, line))
	{
		if (!(line.contains('A') || line.contains('B') || line.contains('C') || line.contains('D')))
		{
			continue;
		}
		std::size_t room_index = 0;
		for (auto const c : line.filter([](auto const c) { return c == 'A' || c == 'B' || c == 'C' || c == 'D'; }))
		{
			result.side_rooms[room_index][room_pos_index] = c;
			++room_index;
		}
		assert(room_index == 4);
		++room_pos_index;
	}
	assert(room_pos_index == 2);

	return result;
}

static full_burrow_t read_full_input(fs::path const &filename)
{
	full_burrow_t result{};
	auto file = std::ifstream(filename);
	string line;
	std::size_t room_pos_index = 0;
	while (std::getline(file, line))
	{
		if (!(line.contains('A') || line.contains('B') || line.contains('C') || line.contains('D')))
		{
			continue;
		}
		std::size_t room_index = 0;
		for (auto const c : line.filter([](auto const c) { return c == 'A' || c == 'B' || c == 'C' || c == 'D'; }))
		{
			result.side_rooms[room_index][room_pos_index] = c;
			++room_index;
		}
		assert(room_index == 4);
		++room_pos_index;
	}
	assert(room_pos_index == 4);

	return result;
}

static int get_movement_count(std::size_t hallway_index, std::size_t room_index, std::size_t room_pos_index)
{
	switch (room_index)
	{
	case 0:
		return static_cast<int>(room_pos_index) + (
			hallway_index == 0 ? 3 :
			hallway_index == 6 ? 9 :
			hallway_index == 1 ? 2 :
			((static_cast<int>(hallway_index) - 2) * 2 + 2)
		);
	case 1:
		return static_cast<int>(room_pos_index) + (
			hallway_index == 0 ? 5 :
			hallway_index == 6 ? 7 :
			(std::min(std::abs(2 - static_cast<int>(hallway_index)), std::abs(static_cast<int>(hallway_index) - 3)) * 2 + 2)
		);
	case 2:
		return static_cast<int>(room_pos_index) + (
			hallway_index == 0 ? 7 :
			hallway_index == 6 ? 5 :
			(std::min(std::abs(3 - static_cast<int>(hallway_index)), std::abs(static_cast<int>(hallway_index) - 4)) * 2 + 2)
		);
	case 3:
		return static_cast<int>(room_pos_index) + (
			hallway_index == 0 ? 9 :
			hallway_index == 6 ? 3 :
			hallway_index == 5 ? 2 :
			((4 - static_cast<int>(hallway_index)) * 2 + 2)
		);
	default:
		assert(false);
		return -1;
	}
}

static bool is_accessible(burrow_t const &burrow, std::size_t hallway_index, std::size_t room_index, std::size_t room_pos_index)
{
	if (room_pos_index == 1 && burrow.side_rooms[room_index][0] != 0)
	{
		return false;
	}

	auto const [left, right] = std::make_pair(room_index + 1, room_index + 2);

	if (hallway_index < left)
	{
		for (auto const i : utils::iota(hallway_index + 1, left + 1))
		{
			if (burrow.hallway[i] != 0)
			{
				return false;
			}
		}
	}
	else if (hallway_index > right)
	{
		for (auto const i : utils::iota(right, hallway_index))
		{
			if (burrow.hallway[i] != 0)
			{
				return false;
			}
		}
	}

	return true;
}

static vector<std::pair<int, burrow_t>> get_next_burrows(burrow_t const &burrow)
{
	vector<std::pair<int, burrow_t>> result;

	for (auto const hallway_index : utils::iota(0, burrow.hallway.size()))
	{
		if (burrow.hallway[hallway_index] == 0)
		{
			continue;
		}

		std::size_t const amphipod_kind = burrow.hallway[hallway_index] - 'A';
		auto const room_pos_index = burrow.side_rooms[amphipod_kind] == std::array<char, 2>{ 0, 0 } ? 1 : 0;
		if (
			burrow.side_rooms[amphipod_kind][room_pos_index] == 0
			&& utils::to_range(burrow.side_rooms[amphipod_kind]).is_all([&](auto const c) { return c == burrow.hallway[hallway_index] || c == 0; })
			&& is_accessible(burrow, hallway_index, amphipod_kind, room_pos_index)
		)
		{
			auto const movement_cost = amphipod_movement_costs[amphipod_kind] * get_movement_count(hallway_index, amphipod_kind, room_pos_index);
			burrow_t new_burrow = burrow;
			std::swap(new_burrow.hallway[hallway_index], new_burrow.side_rooms[amphipod_kind][room_pos_index]);
			result.push_back({ movement_cost, new_burrow });
		}
	}

	for (auto const room_index : utils::iota(0, burrow.side_rooms.size()))
	{
		auto const [top, bottom] = burrow.side_rooms[room_index];
		if (top == 0 && bottom == 0)
		{
			continue;
		}

		auto const room_pos_index = top == 0 ? 1 : 0;
		for (auto const hallway_index : utils::iota(0, burrow.hallway.size()))
		{
			if (burrow.hallway[hallway_index] == 0 && is_accessible(burrow, hallway_index, room_index, room_pos_index))
			{
				std::size_t const amphipod_kind = burrow.side_rooms[room_index][room_pos_index] - 'A';
				auto const movement_cost = amphipod_movement_costs[amphipod_kind] * get_movement_count(hallway_index, room_index, room_pos_index);
				burrow_t new_burrow = burrow;
				std::swap(new_burrow.hallway[hallway_index], new_burrow.side_rooms[room_index][room_pos_index]);
				result.push_back({ movement_cost, new_burrow });
			}
		}
	}

	return result;
}

static int get_energy_cost_estimate(burrow_t const &burrow)
{
	int result = 0;
	for (auto const hallway_index : utils::iota(0, burrow.hallway.size()))
	{
		if (burrow.hallway[hallway_index] != 0)
		{
			std::size_t const amphipod_kind = burrow.hallway[hallway_index] - 'A';
			result += amphipod_movement_costs[amphipod_kind] * get_movement_count(hallway_index, amphipod_kind, 0);
		}
	}
	for (auto const room_index : utils::iota(0, burrow.side_rooms.size()))
	{
		auto const [top, bottom] = burrow.side_rooms[room_index];
		char const room_kind = 'A' + room_index;
		if (top != 0 && top != room_kind)
		{
			std::size_t const amphipod_kind = top - 'A';
			result += amphipod_movement_costs[amphipod_kind] * (std::abs(int(amphipod_kind) - int(room_index)) * 2 + 2);
		}
		if (bottom != 0 && bottom != room_kind)
		{
			std::size_t const amphipod_kind = bottom - 'A';
			result += amphipod_movement_costs[amphipod_kind] * (std::abs(int(amphipod_kind) - int(room_index)) * 2 + 3);
		}
	}

	return result;
}

struct estimate_and_burrow_t
{
	int energy_cost_estimate;
	int previous_energy_cost;
	burrow_t burrow;
};

bool operator < (estimate_and_burrow_t const &lhs, estimate_and_burrow_t const &rhs)
{
	return (lhs.energy_cost_estimate + lhs.previous_energy_cost) > (rhs.energy_cost_estimate + rhs.previous_energy_cost);
}

static int solution_part_1(burrow_t const &burrow)
{
	std::priority_queue<estimate_and_burrow_t> queue;
	std::unordered_set<burrow_t> past_burrows;
	queue.push({ get_energy_cost_estimate(burrow), 0, burrow });
	while (queue.top().burrow != end_burrow)
	{
		auto const next_burrows = get_next_burrows(queue.top().burrow);
		auto const current_energy_cost = queue.top().previous_energy_cost;
		past_burrows.insert(queue.top().burrow);
		queue.pop();
		for (auto const &[movement_cost, next_burrow] : next_burrows)
		{
			if (!past_burrows.contains(next_burrow))
			{
				queue.push({ get_energy_cost_estimate(next_burrow), current_energy_cost + movement_cost, next_burrow });
			}
		}
	}
	return queue.top().previous_energy_cost;
}

static bool is_accessible(full_burrow_t const &burrow, std::size_t hallway_index, std::size_t room_index, std::size_t room_pos_index)
{
	if (utils::iota(0, room_pos_index).is_any([&](auto const i) { return burrow.side_rooms[room_index][i] != 0; }))
	{
		return false;
	}

	auto const [left, right] = std::make_pair(room_index + 1, room_index + 2);

	if (hallway_index < left)
	{
		for (auto const i : utils::iota(hallway_index + 1, left + 1))
		{
			if (burrow.hallway[i] != 0)
			{
				return false;
			}
		}
	}
	else if (hallway_index > right)
	{
		for (auto const i : utils::iota(right, hallway_index))
		{
			if (burrow.hallway[i] != 0)
			{
				return false;
			}
		}
	}

	return true;
}

static vector<std::pair<int, full_burrow_t>> get_next_burrows(full_burrow_t const &burrow)
{
	vector<std::pair<int, full_burrow_t>> result;

	for (auto const hallway_index : utils::iota(0, burrow.hallway.size()))
	{
		if (burrow.hallway[hallway_index] == 0)
		{
			continue;
		}

		std::size_t const amphipod_kind = burrow.hallway[hallway_index] - 'A';
		auto const room_pos_index = [&]() -> std::size_t {
			if (burrow.side_rooms[amphipod_kind][0] != 0)
			{
				return 0;
			}
			for (auto const i : utils::iota(1, burrow.side_rooms[amphipod_kind].size()))
			{
				if (burrow.side_rooms[amphipod_kind][i] != 0)
				{
					return i - 1;
				}
			}
			return burrow.side_rooms[amphipod_kind].size() - 1;
		}();
		if (
			burrow.side_rooms[amphipod_kind][room_pos_index] == 0
			&& utils::to_range(burrow.side_rooms[amphipod_kind]).is_all([&](auto const c) { return c == burrow.hallway[hallway_index] || c == 0; })
			&& is_accessible(burrow, hallway_index, amphipod_kind, room_pos_index)
		)
		{
			auto const movement_cost = amphipod_movement_costs[amphipod_kind] * get_movement_count(hallway_index, amphipod_kind, room_pos_index);
			full_burrow_t new_burrow = burrow;
			std::swap(new_burrow.hallway[hallway_index], new_burrow.side_rooms[amphipod_kind][room_pos_index]);
			result.push_back({ movement_cost, new_burrow });
		}
	}

	for (auto const room_index : utils::iota(0, burrow.side_rooms.size()))
	{
		if (utils::to_range(burrow.side_rooms[room_index]).is_all([](auto const c) { return c == 0; }))
		{
			continue;
		}

		auto const room_pos_index = utils::iota(0, burrow.side_rooms[room_index].size())
			.filter([&](auto const i) { return burrow.side_rooms[room_index][i] != 0; })
			.front();
		for (auto const hallway_index : utils::iota(0, burrow.hallway.size()))
		{
			if (burrow.hallway[hallway_index] == 0 && is_accessible(burrow, hallway_index, room_index, room_pos_index))
			{
				std::size_t const amphipod_kind = burrow.side_rooms[room_index][room_pos_index] - 'A';
				auto const movement_cost = amphipod_movement_costs[amphipod_kind] * get_movement_count(hallway_index, room_index, room_pos_index);
				full_burrow_t new_burrow = burrow;
				std::swap(new_burrow.hallway[hallway_index], new_burrow.side_rooms[room_index][room_pos_index]);
				result.push_back({ movement_cost, new_burrow });
			}
		}
	}

	return result;
}

static int get_energy_cost_estimate(full_burrow_t const &burrow)
{
	int result = 0;
	for (auto const hallway_index : utils::iota(0, burrow.hallway.size()))
	{
		if (burrow.hallway[hallway_index] != 0)
		{
			std::size_t const amphipod_kind = burrow.hallway[hallway_index] - 'A';
			result += amphipod_movement_costs[amphipod_kind] * get_movement_count(hallway_index, amphipod_kind, 0);
		}
	}
	for (auto const room_index : utils::iota(0, burrow.side_rooms.size()))
	{
		for (auto const room_pos_index : utils::iota(0, burrow.side_rooms[room_index].size()))
		{
			auto const space = burrow.side_rooms[room_index][room_pos_index];
			char const room_kind = 'A' + room_index;
			if (space != 0 && space != room_kind)
			{
				std::size_t const amphipod_kind = space - 'A';
				result += amphipod_movement_costs[amphipod_kind] * (std::abs(int(amphipod_kind) - int(room_index)) * 2 + 2 + int(room_pos_index));
			}
		}
	}

	return result;
}

struct estimate_and_full_burrow_t
{
	int energy_cost_estimate;
	int previous_energy_cost;
	full_burrow_t burrow;
};

bool operator < (estimate_and_full_burrow_t const &lhs, estimate_and_full_burrow_t const &rhs)
{
	return (lhs.energy_cost_estimate + lhs.previous_energy_cost) > (rhs.energy_cost_estimate + rhs.previous_energy_cost);
}

static int solution_part_2(full_burrow_t const &burrow)
{
	std::priority_queue<estimate_and_full_burrow_t> queue;
	std::unordered_set<full_burrow_t> past_burrows;
	queue.push({ get_energy_cost_estimate(burrow), 0, burrow });
	while (queue.top().burrow != end_full_burrow)
	{
		// fmt::print("{}\n", queue.top().previous_energy_cost + queue.top().energy_cost_estimate);
		auto const &current = queue.top();
		auto const next_burrows = get_next_burrows(current.burrow);
		auto const current_energy_cost = current.previous_energy_cost;
		past_burrows.insert(current.burrow);
		queue.pop();
		for (auto const &[movement_cost, next_burrow] : next_burrows)
		{
			if (!past_burrows.contains(next_burrow))
			{
				queue.push({ get_energy_cost_estimate(next_burrow), current_energy_cost + movement_cost, next_burrow });
			}
		}
	}
	return queue.top().previous_energy_cost;
}

int main(void)
{
	auto const burrow = read_input("input.txt");
	auto const part_1_result = solution_part_1(burrow);
	fmt::print("part 1: {}\n", part_1_result);
	auto const full_burrow = read_full_input("full_input.txt");
	auto const part_2_result = solution_part_2(full_burrow);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
