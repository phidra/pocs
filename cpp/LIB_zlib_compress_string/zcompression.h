#pragma once

#include <iostream>
#include <string>
#include <zlib.h>

// Copied then adapted from : https://gist.github.com/halloweeks/8bb0a745229b4674aaacffc7a23e5956

// Compression to/from string :
std::string compress_to_string(const std::string& str, int compressionlevel = Z_DEFAULT_COMPRESSION);
std::string decompress_to_string(const std::string& str);

// Compression to/from stream :
void compress_to_stream(const std::string& input_str, std::ostream& out, int compressionlevel = Z_DEFAULT_COMPRESSION);
void decompress_to_stream(const std::string& str, std::ostream& out);
