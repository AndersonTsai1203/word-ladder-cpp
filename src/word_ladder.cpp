#include "word_ladder.h"
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <queue>

namespace {
	// recursively building the ladder paths from begin to end words using word map
	auto back_track(const std::string& begin_word,
	                const std::string& end_word,
	                std::unordered_map<std::string, std::vector<std::string>>& word_map,
	                std::vector<std::string>& ladder_path,
	                std::vector<std::vector<std::string>>& results) -> void {
		if (begin_word == end_word) {
			results.push_back(ladder_path);
			return;
		}

		for (const auto& word : word_map[begin_word]) {
			ladder_path.push_back(word);
			back_track(word, end_word, word_map, ladder_path, results);
			ladder_path.pop_back();
		}
	}

	// handle generating the next possible words differed by exactly one character
	// checking if those possible words are in the end set
	// updating the word map and adding those valid words to next BFS level
	auto get_next_possible_words(const std::string& word,
	                             const std::unordered_set<std::string>& end_set,
	                             std::unordered_set<std::string>& dictionary,
	                             std::unordered_set<std::string>& next_bfs_level,
	                             std::unordered_map<std::string, std::vector<std::string>>& word_map,
	                             bool& end_word_found,
	                             bool direction_search_reversed) -> void {
		auto current_word = word;
		for (auto i = size_t{0}; i < current_word.size(); ++i) {
			char original_char = current_word[i];
			for (auto ch = 'a'; ch <= 'z'; ch++) {
				if (ch == original_char)
					continue;
				current_word[i] = ch;
				if (end_set.find(current_word) != end_set.end()) {
					end_word_found = true;
					if (direction_search_reversed) {
						word_map[current_word].push_back(word);
					}
					else {
						word_map[word].push_back(current_word);
					}
				}
				if (!end_word_found && dictionary.find(current_word) != dictionary.end()) {
					next_bfs_level.insert(current_word);
					if (direction_search_reversed) {
						word_map[current_word].push_back(word);
					}
					else {
						word_map[word].push_back(current_word);
					}
				}
			}
			current_word[i] = original_char;
		}
	}
} // namespace

auto word_ladder::read_lexicon(const std::string& path) -> std::unordered_set<std::string> {
	std::unordered_set<std::string> dictionary;
	std::ifstream file(path);
	std::string word;

	// insert the words with unordered into dictionary
	while (file >> word) {
		dictionary.insert(word);
	}

	return dictionary;
}

auto word_ladder::generate(const std::string& from, const std::string& to, const std::unordered_set<std::string>& lexicon)
    -> std::vector<std::vector<std::string>> {
	auto begin_word = from;
	auto end_word = to;
	auto dictionary = lexicon;
	std::vector<std::vector<std::string>> results;

	if (dictionary.find(end_word) == dictionary.end()) {
		return results;
	}

	dictionary.erase(begin_word);
	dictionary.erase(end_word);

	std::unordered_map<std::string, std::vector<std::string>> word_map;

	auto end_word_found = false;
	auto direction_search_reversed = false;

	std::unordered_set<std::string> begin_set{begin_word};
	std::unordered_set<std::string> end_set{end_word};

	while (!begin_set.empty() && !end_set.empty() && !end_word_found) {
		if (begin_set.size() > end_set.size()) {
			std::swap(begin_set, end_set);
			direction_search_reversed = !direction_search_reversed;
		}

		std::unordered_set<std::string> next_bfs_level;
		for (const auto& word : begin_set) {
			get_next_possible_words(word,
			                        end_set,
			                        dictionary,
			                        next_bfs_level,
			                        word_map,
			                        end_word_found,
			                        direction_search_reversed);
		}

		for (const auto& word : next_bfs_level) {
			dictionary.erase(word);
		}

		begin_set = next_bfs_level;
	}

	if (end_word_found) {
		std::vector<std::string> ladder_path{begin_word};
		back_track(begin_word, end_word, word_map, ladder_path, results);
	}

	// sort the result in lexicographical order
	std::sort(results.begin(), results.end());

	return results;
}
