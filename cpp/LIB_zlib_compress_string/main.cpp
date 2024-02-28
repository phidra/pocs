#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "zcompression.h"

void print_poc_description() {
    std::cout << R"DELIM(

CE QUE MONTRE CETTE POC = l'utilisation de la zlib pour compresser une string :

- on compresse+décompresse une string en entrée, et on vérifie qu'on retombe bien sur la string en entrée
- la POC permet deux cas qui diffèrent à peine : compression depuis/vers une string, et depuis/vers un ostream
- le code est issu puis adapté de : https://gist.github.com/halloweeks/8bb0a745229b4674aaacffc7a23e5956

À noter : de façon contre-intuitive, la string compressée n'est pas décompressable par gunzip.
Le TL;DR est que l'algo de compression est certes le même, mais que gzip nécessite des headers supplémentaires.
Voici quelques liens sur le sujet :
    - Cette réponse (de Mark ADLER himself !)
        https://stackoverflow.com/questions/20762094/how-are-zlib-gzip-and-zip-related-what-do-they-have-in-common-and-how-are-they/20765054#20765054
    - Cet item de la FAQ zlib = https://www.zlib.net/zlib_faq.html#faq18
        Why does gzip give an error on a file I make with compress/deflate?
        The compress and deflate functions produce data in the zlib format, which is different and incompatible with the gzip format.
        The gz* functions in zlib on the other hand use the gzip format.
        Both the zlib and gzip formats use the same compressed data format internally, but have different headers and trailers around the compressed data.
    - Cet autre item de la FAQ zlib = https://www.zlib.net/zlib_faq.html#faq20
        Well that's nice, but how do I make a gzip file in memory?
        You can request that deflate write the gzip format instead of the zlib format using deflateInit2().
        You can also request that inflate decode the gzip format using inflateInit2(). Read zlib.h for more details.
    - la doc de la zlib sur deflateInit2 :
        https://github.com/madler/zlib/blob/5c42a230b7b468dff011f444161c0145b5efae59/zlib.h#L539

)DELIM";
    std::cout << std::endl;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
    print_poc_description();

    std::string input_data = R"DELIM(
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.
Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.
Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
)DELIM";
    std::cout << "La string en entrée = " << input_data << std::endl;

    // STEP 1 = compression/décompression vers une std::string :
    auto compressed_str = compress_to_string(input_data);
    auto decompressed_str = decompress_to_string(compressed_str);
    if (decompressed_str != input_data) {
        throw std::runtime_error("ERROR> la string compressée+décompressée est différente de la string initiale !");
    }
    std::cout << "OK> La string compressée+décompressée est identique à la string initiale" << std::endl;

    // STEP2 = idem, mais vers un std::ostream :
    std::ostringstream compressed_stream;
    compress_to_stream(input_data, compressed_stream);
    std::ostringstream decompressed_stream;
    decompress_to_stream(compressed_stream.str(), decompressed_stream);
    if (decompressed_stream.str() != input_data) {
        throw std::runtime_error("ERROR> le stream compressé+décompressé est différent de la string initiale !");
    }
    std::cout << "OK> le stream compressé+décompressé est identique à la string initiale" << std::endl;

    // STEP3 = ratio de compression :
    std::cout << "Taille non-compressée    = " << input_data.size() << std::endl;
    std::cout << "Taille compressée str    = " << compressed_str.size() << std::endl;
    std::cout << "Taille compressée stream = " << compressed_stream.str().size() << std::endl;

    return 0;
}
