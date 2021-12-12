#include "common.h"
#include <list>

struct node_t
{
	string name;
	bool is_small_cave;
	bool is_visited;
	vector<node_t *> connections;
};

struct network_t
{
	std::list<node_t> nodes;
	node_t *start_node;
	node_t *end_node;
};

static network_t build_network(span<std::pair<string, string> const> node_pairs)
{
	network_t result;

	for (auto const &[lhs, rhs] : node_pairs)
	{
		auto const lhs_it = std::find_if(
			result.nodes.begin(), result.nodes.end(),
			[&lhs = lhs](auto const &node) { return node.name == lhs; }
		);
		node_t *lhs_ptr = nullptr;
		if (lhs_it == result.nodes.end())
		{
			result.nodes.push_back(node_t{ lhs, (lhs[0] >= 'a' && lhs[0] <= 'z'), false, {} });
			lhs_ptr = &result.nodes.back();
		}
		else
		{
			lhs_ptr = &*lhs_it;
		}

		auto const rhs_it = std::find_if(
			result.nodes.begin(), result.nodes.end(),
			[&rhs = rhs](auto const &node) { return node.name == rhs; }
		);
		node_t *rhs_ptr = nullptr;
		if (rhs_it == result.nodes.end())
		{
			result.nodes.push_back(node_t{ rhs, (rhs[0] >= 'a' && rhs[0] <= 'z'), false, {} });
			rhs_ptr = &result.nodes.back();
		}
		else
		{
			rhs_ptr = &*rhs_it;
		}

		lhs_ptr->connections.push_back(rhs_ptr);
		rhs_ptr->connections.push_back(lhs_ptr);

		if (lhs == "start")
		{
			result.start_node = lhs_ptr;
		}
		else if (rhs == "start")
		{
			result.start_node = rhs_ptr;
		}

		if (lhs == "end")
		{
			result.end_node = lhs_ptr;
		}
		else if (rhs == "end")
		{
			result.end_node = rhs_ptr;
		}
	}

	return result;
}

using path_list_t = std::list<vector<node_t *>>;
using path_it_t = path_list_t::iterator;

static path_it_t continue_path_1(path_it_t path, path_list_t &paths, node_t *end_node)
{
	auto const last_node = path->back();
	if (last_node == end_node)
	{
		++path;
		return path;
	}

	for (auto const next_node : last_node->connections)
	{
		if (next_node->is_small_cave && path->contains(next_node))
		{
			continue;
		}

		auto &new_path = paths.emplace_back(*path);
		new_path.push_back(next_node);
	}

	return paths.erase(path);
}

static std::size_t solution_part_1(span<std::pair<string, string> const> node_pairs)
{
	auto const cave_network = build_network(node_pairs);

	path_list_t paths;
	{
		auto &first_path = paths.emplace_back();
		first_path.push_back(cave_network.start_node);
	}

	while (utils::to_range(paths).is_any([end_node = cave_network.end_node](auto const &path) { return path.back() != end_node; }))
	{
		auto it = paths.begin();
		while (it != paths.end())
		{
			it = continue_path_1(it, paths, cave_network.end_node);
		}
	}

	return paths.size();
}

struct path_t
{
	vector<node_t *> path{};
	bool is_small_cave_visited_twice = false;
};

using path_list_2_t = std::list<path_t>;
using path_it_2_t = path_list_2_t::iterator;

static path_it_2_t continue_path_2(path_it_2_t path, path_list_2_t &paths, node_t *start_node, node_t *end_node)
{
	auto const last_node = path->path.back();
	if (last_node == end_node)
	{
		++path;
		return path;
	}

	for (auto const next_node : last_node->connections)
	{
		if (next_node == start_node)
		{
			continue;
		}
		auto const is_small_twice = next_node->is_small_cave && path->path.contains(next_node);
		if (path->is_small_cave_visited_twice && is_small_twice)
		{
			continue;
		}

		auto &new_path = paths.emplace_back(*path);
		new_path.path.push_back(next_node);
		new_path.is_small_cave_visited_twice |= is_small_twice;
	}

	return paths.erase(path);
}

static std::size_t solution_part_2(span<std::pair<string, string> const> node_pairs)
{
	auto const cave_network = build_network(node_pairs);

	path_list_2_t paths;
	{
		auto &first_path = paths.emplace_back();
		first_path.path.push_back(cave_network.start_node);
	}

	while (utils::to_range(paths).is_any([end_node = cave_network.end_node](auto const &path) { return path.path.back() != end_node; }))
	{
		auto it = paths.begin();
		while (it != paths.end())
		{
			it = continue_path_2(it, paths, cave_network.start_node, cave_network.end_node);
		}
	}

	return paths.size();
}

int main(void)
{
	auto const node_pairs = read_file(
		"input.txt",
		[](auto const &line) {
			auto const connected_nodes = split_by(line, "-");
			assert(connected_nodes.size() == 2);
			return std::pair<string, string>{ connected_nodes[0], connected_nodes[1] };
		}
	);
	auto const part_1_result = solution_part_1(node_pairs);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(node_pairs);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
