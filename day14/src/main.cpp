#include "common.h"
#include <array>

static constexpr std::size_t N = 'Z' - 'A' + 1;

struct polymerization_instructions_t
{
	string starting_polymer;
	std::array<std::array<char, N>, N> rules{};

	char get_rule(char left, char right) const
	{
		assert(left >= 'A' && left <= 'Z');
		assert(right >= 'A' && right <= 'Z');
		return this->rules[left - 'A'][right - 'A'];
	}

	void set_rule(std::string_view element_pair, char inserted_element)
	{
		assert(element_pair.length() == 2);
		assert(element_pair[0] >= 'A' && element_pair[0] <= 'Z');
		assert(element_pair[1] >= 'A' && element_pair[1] <= 'Z');
		this->rules[element_pair[0] - 'A'][element_pair[1] - 'A'] = inserted_element;
	}
};

static polymerization_instructions_t read_input(fs::path const &filename)
{
	polymerization_instructions_t instructions;

	auto file = std::ifstream(filename);
	std::getline(file, instructions.starting_polymer);

	string line;
	std::getline(file, line);
	assert(line == "");

	while (std::getline(file, line))
	{
		auto const line_sv = std::string_view(line);
		assert(line_sv.length() == 7);
		assert(line_sv.substr(2, 4) == " -> ");
		auto const element_pair = line_sv.substr(0, 2);
		auto const inserted_element = line_sv[6];
		instructions.set_rule(element_pair, inserted_element);
	}

	return instructions;
}

static string build_next_polymer(std::string_view polymer, polymerization_instructions_t const &instructions)
{
	string next_polymer = "";
	next_polymer.reserve(polymer.size() * 2 - 1);
	next_polymer += polymer[0];
	for (auto const [left, right] : utils::to_range(polymer).adjacent())
	{
		auto const rule = instructions.get_rule(left, right);
		if (rule != 0)
		{
			next_polymer += rule;
		}
		next_polymer += right;
	}
	return next_polymer;
}

static std::size_t get_min_max_distance(std::string_view polymer)
{
	std::array<std::size_t, N> counts{};
	for (auto const element : polymer)
	{
		counts[element - 'A'] += 1;
	}

	auto const max = utils::to_range(counts).max();
	auto const min = utils::to_range(counts).filter([](auto const count) { return count != 0; }).min();
	return max - min;
}

static int solution_part_1(polymerization_instructions_t const &instructions)
{
	string polymer = instructions.starting_polymer;
	for ([[maybe_unused]] auto const _ : utils::iota(0, 10))
	{
		polymer = build_next_polymer(polymer, instructions);
	}
	return get_min_max_distance(polymer);
}

static std::size_t solution_part_2(polymerization_instructions_t const &instructions)
{
	std::array<std::size_t, N> element_counts = {};
	for (auto const element : instructions.starting_polymer)
	{
		element_counts[element - 'A'] += 1;
	}

	std::array<std::array<std::size_t, N>, N> pair_counts = {};

	for (auto const [left, right] : instructions.starting_polymer.adjacent())
	{
		pair_counts[left - 'A'][right - 'A'] += 1;
	}

	for ([[maybe_unused]] auto const _ : utils::iota(0, 40))
	{
		std::array<std::array<std::size_t, N>, N> new_pair_counts = {};
		for (auto const left : utils::iota('A', 'Z' + 1))
		{
			for (auto const right : utils::iota('A', 'Z' + 1))
			{
				auto const rule = instructions.get_rule(left, right);
				auto const pair_count = pair_counts[left - 'A'][right - 'A'];
				new_pair_counts[left - 'A'][rule - 'A']  += pair_count;
				new_pair_counts[rule - 'A'][right - 'A'] += pair_count;
				element_counts[rule - 'A'] += pair_count;
			}
		}
		pair_counts = new_pair_counts;
	}

	auto const max = utils::to_range(element_counts).max();
	auto const min = utils::to_range(element_counts).filter([](auto const count) { return count != 0; }).min();
	return max - min;
}

int main(void)
{
	auto const instructions = read_input("input.txt");
	auto const part_1_result = solution_part_1(instructions);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(instructions);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
