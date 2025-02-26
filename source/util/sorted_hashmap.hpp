#pragma once

#include <stdexcept>
#include <functional>
#include <optional>
#include <vector>
#include <initializer_list>
#include <unordered_map>

namespace util {
	template<class KeyType, class ValueType, class Hasher = std::hash<KeyType>>
	class SortedHashMap
	{
	private:
		// Typedefs
		using vector_type = std::vector<ValueType>;
		using index_type = std::size_t;
		using hash_map_type = std::unordered_map<KeyType, index_type, Hasher>;

		// Members:
		vector_type elements;
		hash_map_type key_to_element_index;
	public:
		// Default the no-arguemt constructor, destructor, and assignment operators.
		constexpr SortedHashMap() = default;
		~SortedHashMap() = default;
		SortedHashMap(const SortedHashMap&) = default;
		SortedHashMap& operator=(const SortedHashMap&) = default;

		// Initializer list constructor.
		SortedHashMap(std::initializer_list<std::pair<KeyType, ValueType>> list) {
			for (auto* elem = list.begin(); elem < list.end(); elem++) {
				key_to_element_index.insert({ std::move(elem->first), elements.size() });
				elements.push_back(std::move(elem->second));
			}
		}

		constexpr inline size_t size() const {
			return elements.size();
		}

		void insert(const KeyType& key, const ValueType& value) {
			key_to_element_index.insert({ key, elements.size() });
			elements.push_back(value);
		}

		// Returns a reference to the value at the specified key,
		// throws std::out_of_range if the key is not present.
		ValueType& at(const KeyType& key) {
			return elements.at(key_to_element_index.at(key));
		}

		// Returns an optional copy of the value at the specified key.
		std::optional<ValueType> operator[](const KeyType& key) const {
			try {
				return elements.at(key_to_element_index.at(key));
			}
			catch (std::out_of_range& e) {
				return std::nullopt;
			}
		}

		// Returns a reference to the value at the specified index,
		// throws std::out_of_range if the index is not present.
		ValueType& at_index(index_type index) {
			return elements.at(index);
		}

		// Returns an optional copy of the value at the specified index.
		std::optional<ValueType> operator[](index_type index) const {
			try {
				return elements.at(index);
			}
			catch (std::out_of_range& e) {
				return std::nullopt;
			}
		}

		// Returns the key at the specified index,
		// throws std::out_of_range if the index is not present.
		KeyType& get_key(index_type index) {
			const hash_map_type::const_iterator it = std::find_if(
				key_to_element_index.begin(), key_to_element_index.end(), std::bind(&hash_map_type::value_type::second, std::placeholders::_1) == index
			);
			if (it != key_to_element_index.end()) {
				return it->first;
			}
			throw std::out_of_range();
		}

		// Returns an optional index at the specified key.
		std::optional<index_type> get_index(const KeyType& key) {
			try {
				return key_to_element_index.at(key);
			}
			catch (std::out_of_range& e) {
				return std::nullopt;
			}
		}

		// Returns the key for the specified value,
		// throws std::out_of_range if the value is not present.
		KeyType& find(const ValueType& value) {
			const vector_type::const_iterator vit = std::find_if(
				elements.begin(), elements.end(), [](const ValueType& v) { return v == value; }
			);

			if (vit != elements.end()) {
				index_type index = vit - elements.begin();
				const hash_map_type::const_iterator mit = std::find_if(
					key_to_element_index.begin(), key_to_element_index.end(), std::bind(&hash_map_type::value_type::second, std::placeholders::_1) == index
				);
				if (mit != key_to_element_index.end()) {
					return mit->first;
				}
				throw std::out_of_range();
			}
			throw std::out_of_range();
		}

		// Returns an optional index for the specified value.
		std::optional<index_type> find_index(const ValueType& value) {
			const vector_type::const_iterator it = std::find_if(
				elements.begin(), elements.end(), [](const ValueType& v) { return v == value; }
			);

			if (it != elements.end()) {
				return it - elements.begin();
			}

			return std::nullopt;
		}

		bool contains(const KeyType& key) const {
			return key_to_element_index.contains(key);
		}

		void erase(std::size_t index) {
			for (auto& elem : key_to_element_index) {
				if (elem.second > index) {
					elem.second--;
				}
			}
			elements.erase(elements.begin() + index);
		}
	};
};