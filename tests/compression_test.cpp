#include "utilities/compression.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

TEST_CASE("Compresssion - compress") {
    std::string str = "This is a test string.";
    std::string compressed = compression::compress(str);

    REQUIRE(compressed != str);
}

TEST_CASE("Compression - decompress") {
    std::string str = "This is a test string.";
    std::string compressed = compression::compress(str);
    std::string decompressed = compression::decompress(compressed);

    REQUIRE(decompressed == str);
}

// Test decompression of an empty string
TEST_CASE("Compression - decompress empty string") {
    std::string empty = "";
    std::string compressed = compression::compress(empty);
    std::string decompressed = compression::decompress(compressed);

    REQUIRE(decompressed == empty);
}

// Test decompression of an invalid compressed string
TEST_CASE("Compression - decompress invalid string") {
    std::string invalid = "This is not a valid compressed string";
    REQUIRE_THROWS_AS(compression::decompress(invalid), std::runtime_error);
}

// Test compression and decompression of a large string
TEST_CASE("Compression - large string") {
    std::string large_str(10000, 'a'); // A large string of 10,000 'a' characters
    std::string compressed = compression::compress(large_str);
    std::string decompressed = compression::decompress(compressed);

    REQUIRE(decompressed == large_str);
}

// Test compression and decompression with different compression levels
TEST_CASE("Compression - different levels") {
    std::string str = "This is a test string for compression.";

    SECTION("Compression level 0 - no compression") {
        int level = Z_NO_COMPRESSION;

        std::string compressed = compression::compress(str, level);
        std::string decompressed = compression::decompress(compressed);

        REQUIRE(decompressed == str);
    }

    SECTION("Compression level 1 - no compression") {
        int level = Z_BEST_SPEED;

        std::string compressed = compression::compress(str, level);
        std::string decompressed = compression::decompress(compressed);

        REQUIRE(decompressed == str);
    }

    SECTION("Compression level 9 - no compression") {
        int level = Z_BEST_COMPRESSION;

        std::string compressed = compression::compress(str, level);
        std::string decompressed = compression::decompress(compressed);

        REQUIRE(decompressed == str);
    }

    SECTION("Compression level - default") {
        int level = Z_DEFAULT_COMPRESSION;

        std::string compressed = compression::compress(str, level);
        std::string decompressed = compression::decompress(compressed);

        REQUIRE(decompressed == str);
    }
}
