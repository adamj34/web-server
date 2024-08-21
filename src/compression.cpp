#include "utilities/compression.hpp"
#include "zlib.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>

// void compress::decompress(std::string & body) {

// }

std::string compress::compress(const std::string& str, int compression_level = Z_BEST_COMPRESSION) {
    z_stream zs{}; // z_stream is zlib's control structure
    // memset instead of std::fill for perf reasons
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compression_level) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size(); // set the z_stream's input

    int ret;
    char outbuffer[10240];
    std::string outstring;

    while (ret == Z_OK) {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    }

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) { // an error occurred that was not EOF
        std::ostringstream oss{};
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}
