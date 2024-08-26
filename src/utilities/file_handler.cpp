#include "utilities/file_handler.hpp"
#include "spdlog/spdlog.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::string fileHandler::send_file(std::string_view file_path, std::string base_path) {
    if (base_path.empty()) {
        base_path = std::filesystem::current_path();
    }

    // Remove leading slash if present
    if (file_path.starts_with("/")) {
        file_path = file_path.substr(1);
    }
    auto full_path = std::filesystem::path(base_path) / file_path;

    if (std::filesystem::exists(full_path)) {
        std::ifstream fin{ full_path };
        if (fin.fail()) {
            spdlog::error("Failed to open file: {}", file_path);
            throw std::runtime_error("Failed to open file");
        }

        std::ostringstream oss{};
        oss << fin.rdbuf();
        std::string file_content = oss.str();
        fin.close();

        return file_content;
    } else {
        spdlog::error("File does not exist: {}", file_path);
        throw std::runtime_error("File does not exist");
    }
}

void fileHandler::write_file(const std::string& file_path, std::string_view content) {
    auto base_path{ std::filesystem::current_path() };
    auto full_path = base_path / file_path;
    std::ofstream fout{ full_path };
    if (fout.fail()) {
        spdlog::error("Failed to open file: {}", file_path);
        throw std::runtime_error("Failed to open file");
    }
    fout << content;
    fout.close();
}
