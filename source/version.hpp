#pragma once
#include <cstdint>

// A struct that represents a stripped-down Semantic Version.
struct SemVer {
    std::uint16_t major;
    std::uint16_t minor;
    std::uint16_t patch;
};

constexpr auto semver_compare(const SemVer& self, const SemVer& other) -> int {
    if (self.major != other.major) {
        return self.major - other.major;
    }

    if (self.minor != other.minor) {
        return self.minor - other.minor;
    }

    if (self.patch != other.patch) {
        return self.patch - other.patch;
    }

    return 0;
}

constexpr bool operator==(const SemVer& lhs, const SemVer& rhs) {
    return semver_compare(lhs, rhs) == 0;
}

constexpr bool operator!=(const SemVer& lhs, const SemVer& rhs) {
    return semver_compare(lhs, rhs) != 0;
}

constexpr bool operator>(const SemVer& lhs, const SemVer& rhs) {
    return semver_compare(lhs, rhs) > 0;
}

constexpr bool operator>=(const SemVer& lhs, const SemVer& rhs) {
    return semver_compare(lhs, rhs) >= 0;
}

constexpr bool operator<(const SemVer& lhs, const SemVer& rhs) {
    return semver_compare(lhs, rhs) < 0;
}

constexpr bool operator<=(const SemVer& lhs, const SemVer& rhs) {
    return semver_compare(lhs, rhs) <= 0;
}

constexpr inline SemVer PENTANE_FRAMEWORK_VERSION = { 1, 0, 0 };