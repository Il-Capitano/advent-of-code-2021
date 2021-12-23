#include "common.h"

struct vec3
{
	int x;
	int y;
	int z;
};

struct region_t
{
	vec3 begin;
	vec3 end;
};

struct instruction_t
{
	bool state;
	region_t region;
};

static int solution_part_1(span<instruction_t const> instructions)
{
	std::array<std::array<std::array<bool, 101>, 101>, 101> init_region{};

	for (auto const &[state, region] : instructions)
	{
		auto const &[begin, end] = region;
		for (int x = std::max(-50, begin.x); x <= std::min(50, end.x); ++x)
		{
			for (int y = std::max(-50, begin.y); y <= std::min(50, end.y); ++y)
			{
				for (int z = std::max(-50, begin.z); z <= std::min(50, end.z); ++z)
				{
					init_region[x + 50][y + 50][z + 50] = state;
				}
			}
		}
	}

	int result = 0;
	for (auto const x : utils::iota(0, init_region.size()))
	{
		for (auto const y : utils::iota(0, init_region[x].size()))
		{
			for (auto const z : utils::iota(0, init_region[x][y].size()))
			{
				result += init_region[x][y][z] ? 1 : 0;
			}
		}
	}
	return result;
}

static bool overlaps(region_t region1, region_t region2)
{
	return !(
		(region1.end.x < region2.begin.x || region1.begin.x > region2.end.x)
		&& (region1.end.y < region2.begin.y || region1.begin.y > region2.end.y)
		&& (region1.end.z < region2.begin.z || region1.begin.z > region2.end.z)
	);
}

static void split_region(vector<region_t> &result_regions, region_t const &region1, region_t const &region2)
{
	auto const add_region = [&](region_t const &region) {
		if (region.end.x >= region.begin.x && region.end.y >= region.begin.y && region.end.z >= region.begin.z)
		{
			result_regions.push_back(region);
		}
	};
	// -x
	add_region({
		vec3{
			region1.begin.x,
			region1.begin.y,
			region1.begin.z,
		},
		vec3{
			std::min(region1.end.x, region2.begin.x - 1),
			region1.end.y,
			region1.end.z,
		}
	});
	// +x
	add_region({
		vec3{
			std::max(region1.begin.x, region2.end.x + 1),
			region1.begin.y,
			region1.begin.z,
		},
		vec3{
			region1.end.x,
			region1.end.y,
			region1.end.z,
		}
	});
	// -y (mid x)
	add_region({
		vec3{
			std::max(region1.begin.x, region2.begin.x),
			region1.begin.y,
			region1.begin.z,
		},
		vec3{
			std::min(region1.end.x, region2.end.x),
			std::min(region1.end.y, region2.begin.y - 1),
			region1.end.z,
		}
	});
	// +y (mid x)
	add_region({
		vec3{
			std::max(region1.begin.x, region2.begin.x),
			std::max(region1.begin.y, region2.end.y + 1),
			region1.begin.z,
		},
		vec3{
			std::min(region1.end.x, region2.end.x),
			region1.end.y,
			region1.end.z,
		}
	});
	// -z (mid x, mid y)
	add_region({
		vec3{
			std::max(region1.begin.x, region2.begin.x),
			std::max(region1.begin.y, region2.begin.y),
			region1.begin.z,
		},
		vec3{
			std::min(region1.end.x, region2.end.x),
			std::min(region1.end.y, region2.end.y),
			std::min(region1.end.z, region2.begin.z - 1),
		}
	});
	// +z (mid x, mid y)
	add_region({
		vec3{
			std::max(region1.begin.x, region2.begin.x),
			std::max(region1.begin.y, region2.begin.y),
			std::max(region1.begin.z, region2.end.z + 1),
		},
		vec3{
			std::min(region1.end.x, region2.end.x),
			std::min(region1.end.y, region2.end.y),
			region1.end.z,
		}
	});
}

static vector<region_t> remove_region(span<region_t const> regions, region_t const &region)
{
	vector<region_t> result;

	for (auto const &original_region : regions)
	{
		if (!overlaps(original_region, region))
		{
			result.push_back(original_region);
		}
		else
		{
			split_region(result, original_region, region);
		}
	}

	return result;
}

static std::size_t solution_part_2(span<instruction_t const> instructions)
{
	vector<region_t> regions;
	for (auto const &[state, region] : instructions)
	{
		regions = remove_region(regions, region);
		if (state)
		{
			regions.push_back(region);
		}
	}

	return regions
		.transform([](auto const &region) {
			return std::size_t(region.end.x - region.begin.x + 1)
				* std::size_t(region.end.y - region.begin.y + 1)
				* std::size_t(region.end.z - region.begin.z + 1);
		})
		.sum();
}

int main(void)
{
	auto const instructions = read_file("input.txt", [](auto const &line) {
		auto line_sv = std::string_view(line);
		auto const state = line_sv.starts_with("on");
		line_sv = state ? line_sv.substr(3) : line_sv.substr(4);
		auto const ranges = split_by(line_sv, ",")
			.transform([](auto const coords) {
				auto const dots_it = coords.find("..");
				auto const begin = parse_int<int>(coords.substr(2, dots_it - 2));
				auto const end = parse_int<int>(coords.substr(dots_it + 2));
				return std::make_pair(begin, end);
			})
			.collect();

		return instruction_t{
			state,
			{
				vec3{ ranges[0].first,  ranges[1].first,  ranges[2].first  },
				vec3{ ranges[0].second, ranges[1].second, ranges[2].second },
			}
		};
	});
	auto const part_1_result = solution_part_1(instructions);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(instructions);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
