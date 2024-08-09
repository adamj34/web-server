#include "zlib.h"


namespace compress {
    std::string compress(const std::string& str, int compression_level = Z_BEST_COMPRESSION);
    void decompress(std::string& body);
} // namespace compress
