#pragma once

// This header is for utility functions needed just for LAS application
// (can possibly added to later release of UTL library)

#include <unordered_map>


namespace utl {

	namespace las {

		template <typename key, typename item>
		item* getItem(std::unordered_map<key, item> map, key searchFor) {
			if (map.contains(searchFor)) {
				return &map.at(searchFor);
			}
			else
				return nullptr; 
		}

		template <typename key, typename item>
		bool containsItem(std::unordered_map<key, item> map, key searchFor ) {
			return map.contains(searchFor);
		}

	} // las

} // end utl