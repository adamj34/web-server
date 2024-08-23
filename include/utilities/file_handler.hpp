#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>

namespace fileHandler {
std::string send_file(std::string_view file_path, std::string base_path = "");
void write_file(const std::string& file_path, std::string_view content);
}; // namespace fileHandler

#endif // FILE_HANDLER_HPP
