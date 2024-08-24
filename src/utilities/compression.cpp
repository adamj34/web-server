#include "utilities/compression.hpp"
#include "zlib.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>

std::string compression::decompress(const std::string& str) {
    z_stream zs{}; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    // 15 + 16 is the windowBits value that will read a gzip file
    if (inflateInit2(&zs, 15 + 16) != Z_OK)
        throw(std::runtime_error("inflateInit2 failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret{};
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    while (ret == Z_OK) {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer, zs.total_out - outstring.size());
        }
    }

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) { // an error occurred that was not EOF
        std::ostringstream oss{};
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

std::string compression::compress(const std::string& str, int compression_level) {
    z_stream zs{}; // z_stream is zlib's control structure

    // memset instead of std::fill for perf reasons
    memset(&zs, 0, sizeof(zs));

    // 15 + 16 is the windowBits value that will write a gzip file
    if (deflateInit2(&zs, compression_level, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        throw(std::runtime_error("deflateInit2 failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size(); // set the z_stream's input

    int ret = { 0 };
    char outbuffer[10240];
    std::string outstring{};

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
