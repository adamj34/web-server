#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>

class FileHandler {
    public:
        static std::string send_file(std::string_view file_path, std::string base_path = "");
        static void write_file(const std::string& file_path, std::string_view content);
};

#endif // FILE_HANDLER_HPP
