#include "common.h"

struct transmission_data_t
{
	vector<int> bits; // always 0 or 1
};

struct transmission_view_t
{
	span<int const> bits;

	std::size_t consume_bits(std::size_t count)
	{
		assert(count <= this->size());
		assert(count < (sizeof (int) * 4));
		std::size_t result = 0;
		for (auto const i : utils::iota(0, count))
		{
			result <<= 1;
			result |= this->bits[i];
		}
		this->bits = this->bits.slice(count);
		return result;
	}

	transmission_view_t drop_bits(std::size_t count)
	{
		assert(count <= this->size());
		auto const result = this->bits.slice(0, count);
		this->bits = this->bits.slice(count);
		return { result };
	}

	std::size_t size(void) const
	{
		return this->bits.size();
	}
};

static std::size_t parse_literal_value(transmission_view_t &transmission)
{
	std::size_t result = 0;
	while (true)
	{
		auto const bits = transmission.consume_bits(5);
		result <<= 4;
		result |= bits & 0b1111;
		if ((bits & 0b1'0000) == 0)
		{
			break;
		}
	}
	return result;
}

static std::size_t parse_packet_version_numbers(transmission_view_t &transmission)
{
	std::size_t result = 0;

	auto const version = transmission.consume_bits(3);
	result += version;
	auto const type_id = transmission.consume_bits(3);
	switch (type_id)
	{
	// literal value
	case 4:
		parse_literal_value(transmission);
		break;
	// operators
	default:
	{
		auto const length_type_id = transmission.consume_bits(1);
		switch (length_type_id)
		{
		case 0:
		{
			auto const subpacket_lengths = transmission.consume_bits(15);
			auto subpacket_transmission = transmission.drop_bits(subpacket_lengths);
			while (subpacket_transmission.size() != 0)
			{
				result += parse_packet_version_numbers(subpacket_transmission);
			}
			break;
		}
		case 1:
		{
			auto const subpackets_count = transmission.consume_bits(11);
			for ([[maybe_unused]] auto const _ : utils::iota(0, subpackets_count))
			{
				result += parse_packet_version_numbers(transmission);
			}
			break;
		}
		default:
			assert(false);
		}
		break;
	}
	}
	return result;
}

static std::size_t solution_part_1(transmission_data_t const &transmission)
{
	auto transmission_view = transmission_view_t{ transmission.bits };
	return parse_packet_version_numbers(transmission_view);
}

static std::size_t evaluate_packet(transmission_view_t &transmission);

template<typename Fn>
static std::size_t evaluate_operator_packet(transmission_view_t &transmission, Fn reduce_fn)
{
	auto const length_type_id = transmission.consume_bits(1);
	switch (length_type_id)
	{
	case 0:
	{
		auto const subpacket_lengths = transmission.consume_bits(15);
		auto subpacket_transmission = transmission.drop_bits(subpacket_lengths);
		assert(subpacket_transmission.size() != 0);
		std::size_t result = evaluate_packet(subpacket_transmission);
		while (subpacket_transmission.size() != 0)
		{
			result = reduce_fn(result, evaluate_packet(subpacket_transmission));
		}
		return result;
	}
	case 1:
	{
		auto const subpackets_count = transmission.consume_bits(11);
		return utils::iota(0, subpackets_count)
			.transform([&transmission](auto const) { return evaluate_packet(transmission); })
			.reduce(reduce_fn);
	}
	default:
		assert(false);
		return 0;
	}
}

static std::size_t evaluate_packet(transmission_view_t &transmission)
{
	[[maybe_unused]] auto const version = transmission.consume_bits(3);
	auto const type_id = transmission.consume_bits(3);

	switch (type_id)
	{
	case 0:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return lhs + rhs; });
	case 1:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return lhs * rhs; });
	case 2:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return std::min(lhs, rhs); });
	case 3:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return std::max(lhs, rhs); });
	case 4:
		return parse_literal_value(transmission);
	case 5:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return std::size_t(lhs > rhs ? 1 : 0); });
	case 6:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return std::size_t(lhs < rhs ? 1 : 0); });
	case 7:
		return evaluate_operator_packet(transmission, [](std::size_t lhs, std::size_t rhs) { return std::size_t(lhs == rhs ? 1 : 0); });
	default:
		assert(false);
		return 0;
	}
}

static std::size_t solution_part_2(transmission_data_t const &transmission)
{
	auto transmission_view = transmission_view_t{ transmission.bits };
	return evaluate_packet(transmission_view);
}

int main(void)
{
	auto const transmission = read_file("input.txt", [](auto const &line) { return line; })[0]
		.transform([](auto const hex_num) { return hex_num <= '9' ? hex_num - '0' : hex_num - 'A' + 10; })
		.reduce(transmission_data_t{}, [](auto data, int hex_val) {
			for (auto const bit : utils::iota(0, 4).transform([hex_val](auto const i) { return (hex_val >> (3 - i)) & 0x1; }))
			{
				data.bits.push_back(bit);
			}
			return data;
		});
	auto const part_1_result = solution_part_1(transmission);
	fmt::print("part 1: {}\n", part_1_result);
	auto const part_2_result = solution_part_2(transmission);
	fmt::print("part 2: {}\n", part_2_result);
	return 0;
}
