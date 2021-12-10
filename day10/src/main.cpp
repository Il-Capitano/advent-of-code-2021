#include "common.h"
#include <stack>

static int get_score(char c)
{
	switch (c)
	{
	case ')':
		return 3;
	case ']':
		return 57;
	case '}':
		return 1197;
	case '>':
		return 25137;
	}
	assert(false);
	return 0;
}

static char get_opening_paren(char c)
{
	switch (c)
	{
	case ')':
		return '(';
	case ']':
		return '[';
	case '}':
		return '{';
	case '>':
		return '<';
	}
	assert(false);
	return 0;
}

static int solution_part_1(span<string const> navigation_file)
{
	int highscore = 0;
	for (auto const &line : navigation_file)
	{
		std::stack<char> parentheses_stack;
		bool is_corrupted = false;
		for (std::size_t i = 0; !is_corrupted && i < line.size(); ++i)
		{
			auto const c = line[i];
			switch (c)
			{
			case '(':
			case '[':
			case '{':
			case '<':
				parentheses_stack.push(c);
				break;

			case ')':
			case ']':
			case '}':
			case '>':
			{
				assert(!parentheses_stack.empty());
				auto const last_paren = parentheses_stack.top();
				parentheses_stack.pop();
				if (last_paren != get_opening_paren(c))
				{
					highscore += get_score(c);
					is_corrupted = true;
					break;
				}
				break;
			}
			default:
				assert(false);
			}
		}
	}
	return highscore;
}

static std::int64_t get_completion_score(char c)
{
	switch (c)
	{
	case '(':
		return 1;
	case '[':
		return 2;
	case '{':
		return 3;
	case '<':
		return 4;
	}
	assert(false);
	return 0;
}

static std::int64_t solution_part_2(span<string const> navigation_file)
{
	vector<std::int64_t> completion_scores;
	vector<char> parentheses_stack;
	for (auto const &line : navigation_file)
	{
		parentheses_stack.clear();
		bool is_corrupted = false;
		for (std::size_t i = 0; !is_corrupted && i < line.size(); ++i)
		{
			auto const c = line[i];
			switch (c)
			{
			case '(':
			case '[':
			case '{':
			case '<':
				parentheses_stack.push_back(c);
				break;

			case ')':
			case ']':
			case '}':
			case '>':
			{
				assert(!parentheses_stack.empty());
				auto const last_paren = parentheses_stack.back();
				parentheses_stack.pop_back();
				if (last_paren != get_opening_paren(c))
				{
					is_corrupted = true;
					break;
				}
				break;
			}
			default:
				assert(false);
			}
		}

		if (!is_corrupted)
		{
			auto const completion_score = parentheses_stack
				.reversed()
				.transform([](auto const c) { return get_completion_score(c); })
				.reduce([](auto const lhs, auto const rhs) { return lhs * 5 + rhs; });
			completion_scores.push_back(completion_score);
		}
	}
	completion_scores.sort();
	return completion_scores[completion_scores.size() / 2];
}

int main(void)
{
	auto const navigation_file = read_file("input.txt", [](auto const &line) { return line; });
	auto const part_1_result = solution_part_1(navigation_file);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(navigation_file);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
