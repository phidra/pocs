// Copyright 2014, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Hannah Bast <bast@informatik.uni-freiburg.de>,
//          Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_UTIL_CSVPARSER_H_
#define AD_UTIL_CSVPARSER_H_

#include <stdint.h>

#include <exception>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using std::exception;
using std::string;

/**
 * A parser for CSV files, as defined at
 * http://tools.ietf.org/html/rfc4180#page-2
 *
 * This parser is an extension of the original AD CSV parser. It can handle
 * quoted strings and quote escapes as well as BOMs.
 */
namespace ad {
namespace util {

class CsvParserException : public exception {
   public:
    CsvParserException(std::string msg, int index, std::string fieldName, uint64_t line) :
        _msg(msg), _index(index), _fieldName(fieldName), _line(line) {}
    ~CsvParserException() throw() {}

    virtual char const* what() const throw() {
        std::stringstream ss;
        ss << _msg;
        if (_index > -1)
            ss << " for field #" << (_index + 1) << " (" << _fieldName << ")   ";
        _what_msg = ss.str();
        return _what_msg.c_str();
    }

    virtual uint64_t getLine() const { return _line; }

    std::string const& getMsg() const { return _msg; }
    std::string const& getFieldName() const { return _fieldName; }

   private:
    mutable std::string _what_msg;
    std::string _msg;
    int _index;
    std::string _fieldName;
    uint64_t _line;
};

class CsvParser {
   public:
    // Default initialization.
    CsvParser();

    // Initializes the parser by opening the file and reading the table header.
    explicit CsvParser(std::istream* stream);

    // Returns true iff same function of underlying stream returns true.
    bool eof() const;

    // Read next line.
    // Returns true iff the line was read successfully.
    bool readNextLine();

    // Getters for i-th column from current line. Prerequisite: i < _numColumns.
    // Second arguments are default values.

    // returns the i-th column as a trimmed string
    char const* getTString(const size_t i) const;

    // returns the i-th column as a double
    double getDouble(const size_t i) const;

    // returns the i-th columns as a 32bit integer
    int32_t getLong(const size_t i) const;

    // returns the column with given field name.
    // these methods behave exactly the same as the ones above, except that
    // columns are accessed by their identifier.
    char const* getTString(std::string const& fieldName) const;

    double getDouble(std::string const& fieldName) const;

    int32_t getLong(std::string const& fieldName) const;

    // returns the line number the parser is currently at
    int32_t getCurLine() const;

    // checks whether a column with a specific name exists in this file
    bool hasItem(std::string const& fieldName) const;

    // checks whether the field is empty in the current line
    bool fieldIsEmpty(std::string const& fieldName) const;
    bool fieldIsEmpty(size_t field) const;

    // Get the number of columns. Will be zero before openFile has been called.
    size_t getNumColumns() const;

    // returns the index number of a field name
    size_t getFieldIndex(string const& fieldName) const;

    size_t getOptFieldIndex(string const& fieldName) const;

    const string getFieldName(size_t i) const;

   private:
    int32_t _curLine;

    // The handle to the file.
    std::istream* _stream;

    // Parses the header row and fills the header map.
    void parseHeader();

    // returns a trimmed version of a const char*
    //
    // careful: this function is not idempotent. it will leave t
    // right-trimmed.
    char const* inlineRightTrim(char const* t) const;

    // Map of field names to column indices. Parsed from the
    // table header (first row in a CSV file).
    std::unordered_map<std::string, size_t> _headerMap;
    std::vector<std::string> _headerVec;

    // Pointers to the items in the current line.
    std::vector<char const*> _currentItems;

    // modified, quote-escaped strings
    std::vector<std::string> _currentModItems;

    char _buff[10000] = {0};

    static double atof(char const* p, uint8_t mn, bool* fail);
    static uint32_t atoi(char const* p, bool* fail);
};
}  // namespace util
}  // namespace ad

#endif  // AD_UTIL_CSVPARSER_H_
