#include "utilities/file_handler.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

TEST_CASE("FileHandler send file - Existing file") {

    SECTION("File content is returned correctly") {
        std::string file_path = "test.txt";
        std::string expected_content = "This is a test file.";
        std::ofstream test_file{ file_path };
        test_file << expected_content;
        test_file.close();

        std::string result = FileHandler::send_file(file_path);

        REQUIRE(result == expected_content);
    }

    SECTION("File content is returned correctly with a full path specified") {
        std::string file_path = "test.txt";
        std::string test_dir_path = "./";
        std::string expected_content = "This is a test file.";
        std::ofstream test_file{ file_path };
        test_file << expected_content;
        test_file.close();

        std::string result = FileHandler::send_file(file_path, test_dir_path);

        REQUIRE(result == expected_content);
    }

    SECTION("File content is returned correctly when path has a leading slash") {
        std::string file_path = "/test.txt";
        std::string expected_content = "This is a test file.";
        std::ofstream test_file{ file_path };
        test_file << expected_content;
        test_file.close();

        std::string result = FileHandler::send_file(file_path);
        std::cout << result << std::endl;
        REQUIRE(result == expected_content);
    }
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
}
