#include "zlib.h"
#include <string>

namespace compression {

std::string compress(const std::string& str, int compression_level = Z_BEST_COMPRESSION);
std::string decompress(const std::string& body);

} // namespace compression
