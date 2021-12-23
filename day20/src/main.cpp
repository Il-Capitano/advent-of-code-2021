#include "common.h"

static std::pair<vector<bool>, vector<vector<bool>>> read_input(fs::path const &filename)
{
	auto file = std::ifstream(filename);
	string line;
	std::pair<vector<bool>, vector<vector<bool>>> result;

	std::getline(file, line);
	result.first = line.transform([](auto const c) { return c == '#'; }).collect<vector>();
	std::getline(file, line);
	assert(line == "");

	while (std::getline(file, line))
	{
		result.second.emplace_back();
		result.second.back() = line.transform([](auto const c) { return c == '#'; }).collect<vector>();
	}

	return result;
}

static vector<vector<bool>> enhance_image(vector<bool> const &algorithm, vector<vector<bool>> const &image, bool default_value)
{
	auto const height = image.size();
	auto const width = image[0].size();
	auto result = vector<vector<bool>>(height + 2, vector<bool>(width + 2, false));

	auto const get_image_value = [height, width, default_value, &image](int i, int j) -> int {
		if (i >= 1 && i <= int(height) && j >= 1 && j <= int(width))
		{
			return image[i - 1][j - 1] ? 1 : 0;
		}
		else
		{
			return default_value ? 1 : 0;
		}
	};

	for (int const i : utils::iota(0, height + 2))
	{
		for (int const j : utils::iota(0, width + 2))
		{
			std::size_t lookup_index = 0;
			for (int const offset_i : utils::iota(-1, 2))
			{
				for (int const offset_j : utils::iota(-1, 2))
				{
					lookup_index *= 2;
					lookup_index += get_image_value(i + offset_i, j + offset_j);
				}
			}

			result[i][j] = algorithm[lookup_index];
		}
	}

	return result;
}

static int solution_part_1(vector<bool> const &algorithm, vector<vector<bool>> image)
{
	image = enhance_image(algorithm, image, false);
	image = enhance_image(algorithm, image, algorithm.front());
	return image.transform(
		[](auto const &row) {
			return row.transform([](auto const is_light) { return is_light ? 1 : 0; }).sum();
		}
	).sum();
}

static int solution_part_2(vector<bool> const &algorithm, vector<vector<bool>> image)
{
	bool default_value = false;
	for ([[maybe_unused]] auto const _ : utils::iota(0, 50))
	{
		image = enhance_image(algorithm, image, default_value);
		default_value = default_value ? algorithm.back() : algorithm.front();
	}
	assert(default_value == false);
	return image.transform(
		[](auto const &row) {
			return row.transform([](auto const is_light) { return is_light ? 1 : 0; }).sum();
		}
	).sum();
}

int main(void)
{
	auto const [algorithm, image] = read_input("input.txt");
	auto const part_1_result = solution_part_1(algorithm, image);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(algorithm, image);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
