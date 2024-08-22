#include "utilities/file_handler.hpp"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

TEST_CASE("FileHandler send file - Existing file") {
    // Setup: Create the test.txt file before running the test
    std::string file_path = "test.txt";
    std::string expected_content = "This is a test file.";
    std::ofstream test_file{ file_path };
    test_file << expected_content;
    test_file.close();

    SECTION("File content is returned correctly with no base path specified") {
        std::string f_path = "test.txt";

        std::string result = FileHandler::send_file(f_path);

        REQUIRE(result == expected_content);
    }

    SECTION("File content is returned correctly with a full path specified") {
        std::string f_path = "test.txt";
        std::string test_dir_path = std::filesystem::current_path().string();

        std::string result = FileHandler::send_file(f_path, test_dir_path);

        REQUIRE(result == expected_content);
    }

    SECTION("File content is returned correctly when path has a leading slash") {
        std::string f_path = "/test.txt";

        std::string result = FileHandler::send_file(f_path);
        std::cout << result << std::endl;
        REQUIRE(result == expected_content);
    }

    // Cleanup: Remove the test.txt file after the test
    std::remove(file_path.c_str());
}

TEST_CASE("FileHandler send file - Non-existing file") {
    std::string file_path = "non_existing_file.txt";

    SECTION("Exception is thrown") { REQUIRE_THROWS_AS(FileHandler::send_file(file_path), std::runtime_error); }
}

TEST_CASE("FileHandler write file") {
    std::string file_path = "test_write.txt";
    std::string content = "This is a test content.";

    SECTION("File is written correctly") {
        FileHandler::write_file(file_path, content);

        std::ifstream fin{ file_path };
        std::ostringstream oss{};
        oss << fin.rdbuf();
        std::string file_content = oss.str();
        fin.close();

        REQUIRE(file_content == content);
    }

    SECTION("Exception is thrown when file cannot be opened") {
        std::string invalid_file_path = "/root/does_not_exist/test_write.txt";
        REQUIRE_THROWS_AS(FileHandler::write_file(invalid_file_path, content), std::runtime_error);
    }

    // Cleanup: Remove the test_write.txt file after the test
    std::remove(file_path.c_str());
}
