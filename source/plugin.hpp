#pragma once
#include "version.hpp"

struct PentaneUUID {
	std::uint8_t data[16];
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