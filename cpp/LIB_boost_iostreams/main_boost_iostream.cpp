#include <iostream>
#include <fstream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace std;
namespace io = boost::iostreams;

void basic_writes() {
    // stream is directly usable :
    io::stream<io::file_sink> buf("data_GITIGNORE/log1.txt");
    buf << "[WRITING]  Tom" << endl;

    // stream_buffer to use with an ostream :
    io::stream_buffer<io::file_sink> buf2("data_GITIGNORE/log2.txt");
    std::ostream out(&buf2);
    out << "[WRITING]  Jerry" << endl;
}

void basic_compress() {
    ofstream file("data_GITIGNORE/output.gz", ios_base::out | ios_base::binary);
    io::filtering_streambuf<io::output> out;
    out.push(io::gzip_compressor());
    out.push(file);
    ostream o(&out);
    o << "[COMPRESSED WRITING]  Luke SKYWALKER" << endl;
    o << "[COMPRESSED WRITING]  Han SOLO" << endl;
}

void basic_uncompress() {
    ifstream file("data_GITIGNORE/already_compressed.gz", ios_base::in | ios_base::binary);
    io::filtering_streambuf<io::input> in;
    in.push(io::gzip_decompressor());
    in.push(file);
    cout << endl << "===== contenu de already_compressed.gz :" << endl;
    boost::iostreams::copy(in, cout);
}

int main(int argc, char* argv[]) {
    basic_writes();
    basic_compress();
    basic_uncompress();
    return 0;
}
