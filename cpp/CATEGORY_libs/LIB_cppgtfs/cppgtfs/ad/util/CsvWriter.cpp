// Copyright 2018, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include "CsvWriter.h"

#include <algorithm>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include "dtoa_milo.h"

using ad::util::CsvWriter;

// _____________________________________________________________________________
CsvWriter::CsvWriter(std::ostream* str, HeaderList const& headers) :
    _stream(str), _headers(headers), _hWritten(false), _first(true), _delim(',') {}

// _____________________________________________________________________________
void CsvWriter::writeDouble(double d) {
    ad::util::dtoa_milo(d, _dblBuf);
    if (!_first)
        _stream->write(&_delim, 1);
    _first = false;
    _stream->write(_dblBuf, strlen(_dblBuf));
}

// _____________________________________________________________________________
void CsvWriter::skip() {
    if (!_first)
        _stream->write(&_delim, 1);
    _first = false;
}

// _____________________________________________________________________________
void CsvWriter::writeString(std::string const& str) {
    if (!_first)
        _stream->write(&_delim, 1);
    _first = false;

    if (str.find(_delim) != std::string::npos) {
        if (str.find('"') != std::string::npos) {
            (*_stream) << "\"" << escStr(str) << "\"";
        } else {
            (*_stream) << "\"" << str << "\"";
        }
    } else {
        _stream->write(str.c_str(), str.size());
    }
}

// _____________________________________________________________________________
std::string CsvWriter::escStr(std::string const& str) const {
    std::stringstream ret;
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '\"')
            ret << "\"\"";
        else
            ret << str[i];
    }

    return ret.str();
}

// _____________________________________________________________________________
void CsvWriter::writeRawString(std::string const& str) {
    if (!_first)
        _stream->write(&_delim, 1);
    _first = false;
    _stream->write(str.c_str(), str.size());
}

// _____________________________________________________________________________
void CsvWriter::writeInt(int i) {
    if (!_first)
        _stream->write(&_delim, 1);
    _first = false;
    if (i == 0)
        _stream->write("0", 1);
    else if (i == 1)
        _stream->write("1", 1);
    else if (i == 2)
        _stream->write("2", 1);
    else
        (*_stream) << i;
}

// _____________________________________________________________________________
void CsvWriter::flushLine() {
    if (!_hWritten)
        writeHeader();
    (*_stream) << "\n";
    _first = true;
}

// _____________________________________________________________________________
void CsvWriter::writeHeader() {
    _hWritten = true;
    writeStrArr(_headers);
}

// _____________________________________________________________________________
void CsvWriter::writeStrArr(std::vector<std::string> const& arr) {
    if (!arr.size())
        return;
    bool first = false;
    for (auto const& str : arr) {
        if (first)
            (*_stream) << _delim;
        first = true;
        (*_stream) << str.c_str();
    }
}
