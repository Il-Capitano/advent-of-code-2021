#include "common.h"
#include <process.h>

struct vec3
{
	int x;
	int y;
	int z;
};

struct mat3
{
	int c[3][3];
};

static constexpr mat3 operator * (mat3 const &lhs, mat3 const &rhs)
{
	mat3 result{};
	for (std::size_t i = 0; i < 3; ++i)
	{
		for (std::size_t j = 0; j < 3; ++j)
		{
			for (std::size_t k = 0; k < 3; ++k)
			{
				result.c[i][j] += lhs.c[i][k] * rhs.c[k][j];
			}
		}
	}
	return result;
}

static constexpr bool operator == (mat3 const &lhs, mat3 const &rhs)
{
	for (std::size_t i = 0; i < 3; ++i)
	{
		for (std::size_t j = 0; j < 3; ++j)
		{
			if (lhs.c[i][j] != rhs.c[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

static constexpr bool operator != (mat3 const &lhs, mat3 const &rhs)
{
	return !(lhs == rhs);
}

static constexpr bool operator == (vec3 const &lhs, vec3 const &rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

static constexpr bool operator != (vec3 const &lhs, vec3 const &rhs)
{
	return !(lhs == rhs);
}

static constexpr vec3 operator * (mat3 const &lhs, vec3 const &rhs)
{
	return {
		lhs.c[0][0] * rhs.x + lhs.c[0][1] * rhs.y + lhs.c[0][2] * rhs.z,
		lhs.c[1][0] * rhs.x + lhs.c[1][1] * rhs.y + lhs.c[1][2] * rhs.z,
		lhs.c[2][0] * rhs.x + lhs.c[2][1] * rhs.y + lhs.c[2][2] * rhs.z,
	};
}

static constexpr vec3 operator + (vec3 const &lhs, vec3 const &rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

static constexpr vec3 operator - (vec3 const &lhs, vec3 const &rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

using beacon_positions_t = vector<vec3>;

static vector<beacon_positions_t> read_input(fs::path const &filename)
{
	std::ifstream file(filename);
	string line;

	vector<beacon_positions_t> result;

	while (std::getline(file, line) && line.starts_with("---"))
	{
		result.emplace_back();
		while (std::getline(file, line) && line != "")
		{
			auto const line_sv = std::string_view(line);
			auto const first_comma_it = line_sv.find(',');
			auto const second_comma_it = line_sv.find(',', first_comma_it + 1);
			auto const x = parse_int<int>(line_sv.substr(0, first_comma_it));
			auto const y = parse_int<int>(line_sv.substr(first_comma_it + 1, second_comma_it));
			auto const z = parse_int<int>(line_sv.substr(second_comma_it + 1));
			result.back().push_back({ x, y, z });
		}
	}

	return result;
}

static constexpr mat3 id = {{
	{ 1, 0, 0 },
	{ 0, 1, 0 },
	{ 0, 0, 1 },
}};

static constexpr mat3 rot_x = {{
	{ 1, 0,  0 },
	{ 0, 0, -1 },
	{ 0, 1,  0 },
}};

static constexpr mat3 rot_y = {{
	{  0, 0, 1 },
	{  0, 1, 0 },
	{ -1, 0, 0 },
}};

static constexpr mat3 rot_z = {{
	{ 0, -1, 0 },
	{ 1,  0, 0 },
	{ 0,  0, 1 },
}};

static constexpr mat3 flip_x = {{
	{ -1,  0,  0 },
	{  0,  0, -1 },
	{  0, -1,  0 },
}};

static constexpr mat3 flip_y = {{
	{  0,  0, -1 },
	{  0, -1,  0 },
	{ -1,  0,  0 },
}};

static constexpr mat3 flip_z = {{
	{  0, -1,  0 },
	{ -1,  0,  0 },
	{  0,  0, -1 },
}};

static constexpr mat3 rot_axes = {{
	{ 0, 0, 1 },
	{ 1, 0, 0 },
	{ 0, 1, 0 },
}};

static_assert(rot_x * rot_x * rot_x * rot_x == id);
static_assert(rot_y * rot_y * rot_y * rot_y == id);
static_assert(rot_z * rot_z * rot_z * rot_z == id);
static_assert(flip_x * flip_x == id);
static_assert(flip_y * flip_y == id);
static_assert(flip_z * flip_z == id);
static_assert(rot_axes * rot_axes * rot_axes == id);

static constexpr mat3 orientations[] = {
	id, rot_z, rot_z * rot_z, rot_z * rot_z * rot_z,
	flip_x, rot_z * flip_x, rot_z * rot_z * flip_x, rot_z * rot_z * rot_z * flip_x,
	rot_axes, rot_z * rot_axes, rot_z * rot_z * rot_axes, rot_z * rot_z * rot_z * rot_axes,
	flip_x * rot_axes, rot_z * flip_x * rot_axes, rot_z * rot_z * flip_x * rot_axes, rot_z * rot_z * rot_z * flip_x * rot_axes,
	rot_axes * rot_axes, rot_z * rot_axes * rot_axes, rot_z * rot_z * rot_axes * rot_axes, rot_z * rot_z * rot_z * rot_axes * rot_axes,
	flip_x * rot_axes * rot_axes, rot_z * flip_x * rot_axes * rot_axes, rot_z * rot_z * flip_x * rot_axes * rot_axes, rot_z * rot_z * rot_z * flip_x * rot_axes * rot_axes,
};

static_assert(std::size(orientations) == 24);
static_assert([]() {
	for (std::size_t i = 0; i < std::size(orientations); ++i)
	{
		for (std::size_t j = i + 1; j < std::size(orientations); ++j)
		{
			if (orientations[i] == orientations[j])
			{
				return false;
			}
		}
	}
	return true;
}());

struct overlap_result_t
{
	int count;
	vec3 delta;
	mat3 orientation;
};

static overlap_result_t get_overlap_count(beacon_positions_t const &sensor1, beacon_positions_t const &sensor2)
{
	overlap_result_t result{};
	for (auto const &orientation : orientations)
	{
		for (auto const &base1 : sensor1)
		{
			for (auto const &base2 : sensor2)
			{
				auto const delta = base1 - orientation * base2;
				auto const overlap_count = sensor2
					.transform([&](auto const &pos) { return delta + orientation * pos; })
					.transform([&](auto const &pos) { return sensor1.contains(pos) ? 1 : 0; })
					.sum();
				if (overlap_count > result.count)
				{
					result.count = overlap_count;
					result.delta = delta;
					result.orientation = orientation;
				}
			}
		}
	}
	return result;
}

struct position_transform_t
{
	vec3 delta;
	mat3 orientation;
};

static vector<vector<position_transform_t>> iterate_transforms(vector<vector<position_transform_t>> const &transforms)
{
	auto const size = transforms.size();
	auto result = transforms;
	for (auto const i : utils::iota(0, size))
	{
		for (auto const j : utils::iota(0, size))
		{
			if (i == j || result[i][j].delta != vec3{ 0, 0, 0 })
			{
				continue;
			}
			for (auto const k : utils::iota(0, size))
			{
				if (
					i != k && j != k
					&& transforms[i][k].delta != vec3{ 0, 0, 0 }
					&& transforms[k][j].delta != vec3{ 0, 0, 0 }
				)
				{
					result[i][j].orientation = transforms[i][k].orientation * transforms[k][j].orientation;
					result[i][j].delta = transforms[i][k].delta + transforms[i][k].orientation * transforms[k][j].delta;
					break;
				}
			}
		}
	}
	return result;
}

static vector<vector<position_transform_t>> get_final_transforms(span<beacon_positions_t const> sensors)
{
	auto transforms = vector<vector<position_transform_t>>(sensors.size(), vector<position_transform_t>(sensors.size()));
	for (auto const i : utils::iota(0, sensors.size()))
	{
		for (auto const j : utils::iota(0, sensors.size()))
		{
			if (i != j)
			{
				auto const [count, delta, orientation] = get_overlap_count(sensors[i], sensors[j]);
				if (count >= 12)
				{
					transforms[i][j].delta = delta;
					transforms[i][j].orientation = orientation;
				}
			}
		}
	}

	while (transforms[0].slice(1).is_any([](auto const &transform) { return transform.delta == vec3{ 0, 0, 0 }; }))
	{
		transforms = iterate_transforms(transforms);
	}

	return transforms;
}

static std::size_t solution_part_1(span<beacon_positions_t const> sensors)
{
	auto const transforms = get_final_transforms(sensors);

	beacon_positions_t all_beacons = sensors[0];
	auto const size = sensors.size();
	for (auto const i : utils::iota(1, size))
	{
		for (
			auto const pos
			: sensors[i].transform([i, &transforms](auto const &pos) {
				return transforms[0][i].delta + transforms[0][i].orientation * pos;
			})
		)
		{
			if (!all_beacons.contains(pos))
			{
				all_beacons.push_back(pos);
			}
		}
	}

	return all_beacons.size();
}

static int manhattan_distance(vec3 const &lhs, vec3 const &rhs)
{
	auto const diff = lhs - rhs;
	return std::abs(diff.x) + std::abs(diff.y) + std::abs(diff.z);
}

static std::size_t solution_part_2(span<beacon_positions_t const> sensors)
{
	auto const transforms = get_final_transforms(sensors);
	auto const sensor_positions = transforms[0].member<&position_transform_t::delta>().collect<vector>();
	return sensor_positions
		.transform([&sensor_positions](auto const pos) {
			return sensor_positions
				.transform([&pos](auto const inner_pos) { return manhattan_distance(pos, inner_pos); })
				.max();
		})
		.max();
}


int main(void)
{
	auto const sensors = read_input("input.txt");
	auto const part_1_result = solution_part_1(sensors);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(sensors);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
