// Copyright 2018, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_WRITER_H_
#define AD_CPPGTFS_WRITER_H_

#include <stdint.h>

#include <exception>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ad/util/CsvWriter.h"
#include "gtfs/Feed.h"
#include "gtfs/flat/Agency.h"
#include "gtfs/flat/Fare.h"
#include "gtfs/flat/Frequency.h"
#include "gtfs/flat/Route.h"
#include "gtfs/flat/Shape.h"
#include "gtfs/flat/StopTime.h"
#include "gtfs/flat/Transfer.h"
#include "gtfs/flat/Trip.h"

using ad::util::CsvWriter;
using std::string;

// A GTFS writer

namespace ad {
namespace cppgtfs {

class WriterException : public std::exception {
   public:
    WriterException(std::string msg, std::string file_name) : _msg(msg), _file_name(file_name) {}
    WriterException(std::string msg) : _msg(msg), _file_name("?") {}
    ~WriterException() throw() {}

    virtual char const* what() const throw() {
        std::stringstream ss;
        ss << _file_name << ":";
        ss << " " << _msg;
        _what_msg = ss.str();
        return _what_msg.c_str();
    }

   private:
    mutable std::string _what_msg;
    std::string _msg;
    std::string _file_name;
};

class Writer {
   public:
    // Default initialization.
    Writer() {}

    // write a GtfsFeed to a zip/folder
    bool write(gtfs::Feed* sourceFeed, std::string const& path) const;

    bool writeAgency(gtfs::flat::Agency const& ag, CsvWriter* csvw) const;
    bool writeAgencies(gtfs::Feed* f, std::ostream* os) const;

    bool writeStop(gtfs::flat::Stop const& ag, CsvWriter* csvw) const;
    bool writeStops(gtfs::Feed* f, std::ostream* os) const;

    bool writeShapePoint(gtfs::flat::ShapePoint const& ag, CsvWriter* csvw) const;
    bool writeShapes(gtfs::Feed* f, std::ostream* os) const;

    bool writeTrip(gtfs::flat::Trip const& ag, CsvWriter* csvw) const;
    bool writeTrips(gtfs::Feed* f, std::ostream* os) const;

    bool writeStopTime(gtfs::flat::StopTime const& ag, CsvWriter* csvw) const;
    bool writeStopTimes(gtfs::Feed* f, std::ostream* os) const;

    bool writeRoute(gtfs::flat::Route const& ag, CsvWriter* csvw) const;
    bool writeRoutes(gtfs::Feed* f, std::ostream* os) const;

    bool writeFeedInfo(gtfs::Feed* f, std::ostream* os) const;

    bool writeTransfer(gtfs::flat::Transfer const& ag, CsvWriter* csvw) const;
    bool writeTransfers(gtfs::Feed* f, std::ostream* os) const;

    bool writeCalendar(gtfs::flat::Calendar const& ag, CsvWriter* csvw) const;
    bool writeCalendars(gtfs::Feed* f, std::ostream* os) const;

    bool writeCalendarDate(gtfs::flat::CalendarDate const& ag, CsvWriter* csvw) const;
    bool writeCalendarDates(gtfs::Feed* f, std::ostream* os) const;

    bool writeFrequency(gtfs::flat::Frequency const& ag, CsvWriter* csvw) const;
    bool writeFrequencies(gtfs::Feed* f, std::ostream* os) const;

    bool writeFare(gtfs::flat::Fare const& ag, CsvWriter* csvw) const;
    bool writeFares(gtfs::Feed* f, std::ostream* os) const;

    bool writeFareRule(gtfs::flat::FareRule const& ag, CsvWriter* csvw) const;
    bool writeFareRules(gtfs::Feed* f, std::ostream* os) const;

    static void cannotWrite(std::string const& file);

    static CsvWriter getAgencyCsvw(std::ostream* os);
    static CsvWriter getStopsCsvw(std::ostream* os);
    static CsvWriter getRoutesCsvw(std::ostream* os);
    static CsvWriter getCalendarCsvw(std::ostream* os);
    static CsvWriter getCalendarDatesCsvw(std::ostream* os);
    static CsvWriter getFrequencyCsvw(std::ostream* os);
    static CsvWriter getTransfersCsvw(std::ostream* os);
    static CsvWriter getFaresCsvw(std::ostream* os);
    static CsvWriter getFareRulesCsvw(std::ostream* os);
    static CsvWriter getShapesCsvw(std::ostream* os);
    static CsvWriter getTripsCsvw(std::ostream* os);
    static CsvWriter getStopTimesCsvw(std::ostream* os);
    static CsvWriter getFeedInfoCsvw(std::ostream* os);
};
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_WRITER_H_
