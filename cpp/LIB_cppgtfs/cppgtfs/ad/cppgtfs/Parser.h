// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_PARSER_H_
#define AD_CPPGTFS_PARSER_H_

#include <stdint.h>

#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ad/util/CsvParser.h"
#include "gtfs/Feed.h"
#include "gtfs/flat/Agency.h"
#include "gtfs/flat/Frequency.h"
#include "gtfs/flat/Route.h"
#include "gtfs/flat/Service.h"
#include "gtfs/flat/Shape.h"
#include "gtfs/flat/Transfer.h"

using ad::cppgtfs::gtfs::Agency;
using ad::cppgtfs::gtfs::Fare;
using ad::cppgtfs::gtfs::FareRule;
using ad::cppgtfs::gtfs::Route;
using ad::cppgtfs::gtfs::Service;
using ad::cppgtfs::gtfs::ServiceDate;
using ad::cppgtfs::gtfs::Shape;
using ad::cppgtfs::gtfs::ShapePoint;
using ad::cppgtfs::gtfs::Stop;
using ad::cppgtfs::gtfs::StopTime;
using ad::cppgtfs::gtfs::Time;
using ad::cppgtfs::gtfs::Transfer;
using ad::cppgtfs::gtfs::TripB;
using ad::util::CsvParser;
using ad::util::CsvParserException;
using std::string;

// A GTFS parser

namespace ad {
namespace cppgtfs {

class ParserException : public std::exception {
   public:
    ParserException(std::string msg, std::string field_name, int64_t line, std::string file_name) :
        _msg(msg), _field_name(field_name), _line(line), _file_name(file_name) {}
    ParserException(std::string msg, std::string field_name, int64_t line) :
        _msg(msg), _field_name(field_name), _line(line), _file_name("?") {}
    ~ParserException() throw() {}

    virtual char const* what() const throw() {
        std::stringstream ss;
        ss << _file_name << ":";
        if (_line > -1)
            ss << _line << ":";
        if (!_field_name.empty())
            ss << " in field '" << _field_name << "',";
        ss << " " << _msg;
        _what_msg = ss.str();
        return _what_msg.c_str();
    }

    virtual uint64_t getLine() const throw() { return _line; }

    void setFileName(std::string const& fn) { _file_name = fn; }

   private:
    mutable std::string _what_msg;
    std::string _msg;
    std::string _field_name;
    int64_t _line;
    std::string _file_name;
};

class Parser {
   public:
    // Default initialization.
    Parser() : _strict(false) {}
    Parser(bool strict) : _strict(strict) {}

    // parse a zip/folder into a GtfsFeed
    FEEDTPL
    bool parse(gtfs::FEEDB* targetFeed, std::string const& path) const;

    inline std::string getString(CsvParser const& csv, size_t field) const;
    inline std::string getString(CsvParser const& csv, size_t field, std::string const& def) const;

    inline double getDouble(CsvParser const& csv, size_t field) const;
    inline double getDouble(CsvParser const& csv, size_t fld, double def) const;

    inline int64_t getRangeInteger(CsvParser const& csv, size_t field, int64_t minv, int64_t maxv) const;
    inline int64_t getRangeInteger(CsvParser const& csv, size_t field, int64_t minv, int64_t maxv, int64_t def) const;

    inline uint32_t getColorFromHexString(CsvParser const& csv, size_t field, std::string const& def) const;

    inline gtfs::ServiceDate getServiceDate(CsvParser const& csv, size_t field) const;
    inline gtfs::ServiceDate getServiceDate(CsvParser const& csv, size_t field, bool req) const;

    inline gtfs::Time getTime(CsvParser const& csv, size_t field) const;

    inline gtfs::flat::Route::TYPE getRouteType(CsvParser const& csv, size_t field, int64_t t) const;
    inline void fileNotFound(std::string const& file) const;

    inline static gtfs::flat::AgencyFlds getAgencyFlds(CsvParser* csvp);
    inline bool nextAgency(CsvParser* csvp, gtfs::flat::Agency* a, gtfs::flat::AgencyFlds const&) const;

    inline static gtfs::flat::StopFlds getStopFlds(CsvParser* csvp);
    inline bool nextStop(CsvParser* csvp, gtfs::flat::Stop* s, gtfs::flat::StopFlds const&) const;

    inline static gtfs::flat::RouteFlds getRouteFlds(CsvParser* csvp);
    inline bool nextRoute(CsvParser* csvp, gtfs::flat::Route* s, gtfs::flat::RouteFlds const&) const;

    inline static gtfs::flat::CalendarFlds getCalendarFlds(CsvParser* csvp);
    inline bool nextCalendar(CsvParser* csvp, gtfs::flat::Calendar* s, gtfs::flat::CalendarFlds const&) const;

    inline static gtfs::flat::CalendarDateFlds getCalendarDateFlds(CsvParser* csvp);
    inline bool nextCalendarDate(CsvParser* csvp,
                                 gtfs::flat::CalendarDate* s,
                                 gtfs::flat::CalendarDateFlds const&) const;

    inline static gtfs::flat::FrequencyFlds getFrequencyFlds(CsvParser* csvp);
    inline bool nextFrequency(CsvParser* csvp, gtfs::flat::Frequency* s, gtfs::flat::FrequencyFlds const&) const;

    inline static gtfs::flat::TransfersFlds getTransfersFlds(CsvParser* csvp);
    inline bool nextTransfer(CsvParser* csvp, gtfs::flat::Transfer* s, gtfs::flat::TransfersFlds const&) const;

    inline static gtfs::flat::FareFlds getFareFlds(CsvParser* csvp);
    inline bool nextFare(CsvParser* csvp, gtfs::flat::Fare* s, gtfs::flat::FareFlds const&) const;

    inline static gtfs::flat::FareRuleFlds getFareRuleFlds(CsvParser* csvp);
    inline bool nextFareRule(CsvParser* csvp, gtfs::flat::FareRule* s, gtfs::flat::FareRuleFlds const&) const;

    inline static gtfs::flat::ShapeFlds getShapeFlds(CsvParser* csvp);
    inline bool nextShapePoint(CsvParser* csvp, gtfs::flat::ShapePoint* s, gtfs::flat::ShapeFlds const&) const;

    inline static gtfs::flat::TripFlds getTripFlds(CsvParser* csvp);
    inline bool nextTrip(CsvParser* csvp, gtfs::flat::Trip* s, gtfs::flat::TripFlds const&) const;

    inline static gtfs::flat::StopTimeFlds getStopTimeFlds(CsvParser* csvp);
    inline bool nextStopTime(CsvParser* csvp, gtfs::flat::StopTime* s, gtfs::flat::StopTimeFlds const&) const;

    FEEDTPL
    void parseAgencies(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseStops(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseRoutes(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseTrips(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseStopTimes(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseCalendar(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseCalendarDates(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseFareAttributes(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseFareRules(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseShapes(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseFrequencies(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseTransfers(gtfs::FEEDB* targetFeed, std::string const& path) const;

    FEEDTPL
    void parseFeedInfo(gtfs::FEEDB* targetFeed, std::string const& path) const;

   private:
    bool _strict;

    static uint32_t atoi(char const** p);

    FEEDTPL
    void parseAgencies(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseStops(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseRoutes(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseTrips(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseStopTimes(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseCalendar(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseCalendarDates(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseFareAttributes(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseFareRules(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseShapes(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseFrequencies(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseTransfers(gtfs::FEEDB* targetFeed, std::istream*) const;

    FEEDTPL
    void parseFeedInfo(gtfs::FEEDB* targetFeed, std::istream*) const;
};
#include "Parser.tpp"
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_PARSER_H_
