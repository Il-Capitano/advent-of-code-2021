#include "common.h"

static std::size_t get_fish_count(span<int const> initial_fish_timers, std::size_t days)
{
	std::size_t fish_count = initial_fish_timers.size();
	std::array<std::size_t, 9> new_fishes_each_day{};
	for (auto const time : initial_fish_timers)
	{
		new_fishes_each_day[time] += 1;
	}

	for ([[maybe_unused]] auto const day : utils::iota(0, days))
	{
		auto const new_fish_count = new_fishes_each_day[0];
		std::copy_n(new_fishes_each_day.begin() + 1, new_fishes_each_day.size() - 1, new_fishes_each_day.begin());
		new_fishes_each_day[6] += new_fish_count;
		new_fishes_each_day[8] = new_fish_count;
		fish_count += new_fish_count;
	}
	return fish_count;
}

static std::size_t solution_part_1(span<int const> fish_timers)
{
	return get_fish_count(fish_timers, 80);
}

static std::size_t solution_part_2(span<int const> fish_timers)
{
	return get_fish_count(fish_timers, 256);
}

int main(void)
{
	auto const fish_timers = read_file(
		"input.txt",
		[](auto const &line) {
			return split_by(line, ",")
			.transform([](auto const s) { return parse_int<int>(s); })
			.template collect<vector>();
		}
	).front();
	auto const part_1_result = solution_part_1(fish_timers);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(fish_timers);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
