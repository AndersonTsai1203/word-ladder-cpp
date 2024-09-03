#include "word_ladder.h"

#include <catch2/catch.hpp>

#include <string.h> // for strlen()
#include <unistd.h> // for mkstemp()

TEST_CASE("word_ladder::read_lexicon works as expected") {
	// create a temporary file
	char file_name[] = "/tmp/tempfile.XXXXXX";
	int file_descriptor = mkstemp(file_name);
	
	REQUIRE(file_descriptor != -1);

	auto const content = "hit\nhot\ndot\ndog\ncog\nlot\nlog\n";

	ssize_t bytes_written_buffer = write(file_descriptor, content, strlen(content));

	REQUIRE(bytes_written_buffer == strlen(content));

	close(file_descriptor);

	auto lexicon = word_ladder::read_lexicon(file_name);

	std::unordered_set<std::string> expected{"hit", "hot", "dot", "dog", "cog", "lot", "log"};

	CHECK(lexicon == expected);

	REQUIRE_FALSE(lexicon.find("cat") != lexicon.end());

	std::remove(file_name);
}

TEST_CASE("word_ladder::read_lexicon empty file") {
	// create a empty file
	char file_name[] = "/tmp/emptyfile.XXXXXX";
	int file_descriptor = mkstemp(file_name);

	REQUIRE(file_descriptor != -1);

	close(file_descriptor);

	auto lexicon = word_ladder::read_lexicon(file_name);

	REQUIRE(lexicon.empty());

	std::remove(file_name);
}

TEST_CASE("word_ladder::generate empty inputs") {
	auto const lexicon = std::unordered_set<std::string>{"at", "it"};

	const auto expected = std::vector<std::vector<std::string>>{};

	auto const ladders = word_ladder::generate("", "", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("word_ladder::generate empty outputs") {
	auto const lexicon = std::unordered_set<std::string>{"airplane", "abstract", "betrayal", "tricycle"};

	const auto expected = std::vector<std::vector<std::string>>{};

	auto const ladders = word_ladder::generate("airplane", "tricycle", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("word_ladder::generate two letters") {
	auto const lexicon = std::unordered_set<std::string>{"at", "it"};

	const auto expected = std::vector<std::vector<std::string>>{{"at", "it"}};

	auto const ladders = word_ladder::generate("at", "it", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("word_ladder::generate three letters") {
	auto const lexicon = std::unordered_set<std::string>{"hot", "cot", "hog", "cog"};

	const auto expected = std::vector<std::vector<std::string>>{{"hot", "cot", "cog"}, {"hot", "hog", "cog"}};

	auto const ladders = word_ladder::generate("hot", "cog", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("word_ladder::generate four letters") {
	auto const lexicon = std::unordered_set<std::string>{"work", "fork", "form", "foam", "flam", "flay", "play", "pork",
	                                                     "perk", "peak", "pean", "plan", "worm", "peat", "plat", "porn",
	                                                     "pirn", "pert", "pian", "port", "word", "wood", "pood", "plod",
	                                                     "ploy", "wort", "bort", "boat", "blat", "wert", "worn"};

	auto const expected =
	    std::vector<std::vector<std::string>>{{"work", "fork", "form", "foam", "flam", "flay", "play"},
	                                          {"work", "pork", "perk", "peak", "pean", "plan", "play"},
	                                          {"work", "pork", "perk", "peak", "peat", "plat", "play"},
	                                          {"work", "pork", "perk", "pert", "peat", "plat", "play"},
	                                          {"work", "pork", "porn", "pirn", "pian", "plan", "play"},
	                                          {"work", "pork", "port", "pert", "peat", "plat", "play"},
	                                          {"work", "word", "wood", "pood", "plod", "ploy", "play"},
	                                          {"work", "worm", "form", "foam", "flam", "flay", "play"},
	                                          {"work", "worn", "porn", "pirn", "pian", "plan", "play"},
	                                          {"work", "wort", "bort", "boat", "blat", "plat", "play"},
	                                          {"work", "wort", "port", "pert", "peat", "plat", "play"},
	                                          {"work", "wort", "wert", "pert", "peat", "plat", "play"}};

	auto const ladders = word_ladder::generate("work", "play", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("word_ladder::generate five letters") {
	auto const lexicon = std::unordered_set<std::string>{"awake",
	                                                     "aware",
	                                                     "sware",
	                                                     "share",
	                                                     "sharn",
	                                                     "shawn",
	                                                     "shewn",
	                                                     "sheen",
	                                                     "shire",
	                                                     "shirr",
	                                                     "shier",
	                                                     "sheer",
	                                                     "sheep",
	                                                     "sleep"};

	const auto expected = std::vector<std::vector<std::string>>{
	    {"awake", "aware", "sware", "share", "sharn", "shawn", "shewn", "sheen", "sheep", "sleep"},
	    {"awake", "aware", "sware", "share", "shire", "shirr", "shier", "sheer", "sheep", "sleep"}};

	auto const ladders = word_ladder::generate("awake", "sleep", lexicon);

	CHECK(ladders == expected);
}

TEST_CASE("word_ladder::generate six letters") {
	auto const lexicon = std::unordered_set<std::string>{"planet", "planes", "plates", "slates", "states"};

	const auto expected = std::vector<std::vector<std::string>>{{"planet", "planes", "plates", "slates", "states"}};

	auto const ladders = word_ladder::generate("planet", "states", lexicon);

	CHECK(ladders == expected);
}
