#pragma once
#include "version.hpp"

constexpr uint8_t hex_char_to_byte(char c) {
	return (c >= '0' && c <= '9') ? (c - '0') : (c >= 'a' && c <= 'f') ? (c - 'a' + 10) : (c >= 'A' && c <= 'F') ? (c - 'A' + 10) : throw std::invalid_argument("Invalid Character");
}

struct PentaneUUID {
	std::uint8_t data[16];
	static constexpr PentaneUUID from_str(const std::string_view input) {
		PentaneUUID uuid{};
		std::size_t idx = 0;
		for (std::size_t i = 0; input[i] != '\0';) {
			if (input[i] == '-') {
				i++;
				continue;
			}
			if (input[i + 1] == '\0') {
				throw std::invalid_argument("Incomplete hex nibble in UUID");
			}
			uuid.data[idx++] = (hex_char_to_byte(input[i]) << 4) | hex_char_to_byte(input[i + 1]);
			i += 2;
			if (idx > 16) {
				throw std::invalid_argument("UUID string too long");
			}
		}
		if (idx != 16) {
			throw std::invalid_argument("UUID string too short");
		}
		return uuid;
	}
};

inline bool operator<(const PentaneUUID& lhs, const PentaneUUID& rhs) {
	return memcmp(lhs.data, rhs.data, 16) < 0;
}

inline bool operator>(const PentaneUUID& lhs, const PentaneUUID& rhs) {
	return memcmp(lhs.data, rhs.data, 16) < 0;
}

inline bool operator==(const PentaneUUID& lhs, const PentaneUUID& rhs) {
	return memcmp(lhs.data, rhs.data, 16) == 0;
}

struct PluginInformation {
	char name[256];
	char author[256];
	PentaneUUID uuid;
	SemVer version;
	SemVer minimum_pentane_version;
};

using PentaneMainFunc = void(__stdcall*)();

static inline std::uint64_t fnv1_hash(const std::uint8_t* buf, std::size_t size) {
	std::uint64_t hashval = 0xcbf29ce484222325ULL;
	const std::uint8_t* first = buf;
	const std::uint8_t* last = first + size;
	while (first < last) {
		auto shift_sum = (hashval << 1) + (hashval << 4) + (hashval << 5) + (hashval << 7) + (hashval << 8) + (hashval << 40);
		hashval += shift_sum;
		hashval ^= static_cast<std::uint64_t>(*first++);
	}
	return hashval;
}

struct PentaneUUIDHasher {
	inline std::size_t operator()(const PentaneUUID& uuid) const noexcept {
		return static_cast<std::size_t>(fnv1_hash(uuid.data, sizeof(uuid.data)));
	}
};