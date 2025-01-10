// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_SERVICE_H_
#define AD_CPPGTFS_GTFS_SERVICE_H_

#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "flat/Service.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

typedef flat::ServiceDate ServiceDate;

class Service {
   public:
    typedef Service* Ref;
    static std::string getId(Ref r) { return r->getId(); }

    typedef flat::Calendar::SERVICE_DAY SERVICE_DAY;
    typedef flat::CalendarDate::EXCEPTION_TYPE EXCEPTION_TYPE;

    explicit Service(string const& id);
    Service(string const& id, uint8_t serviceDays, ServiceDate start, ServiceDate end);

    std::string const& getId() const;
    std::map<ServiceDate, Service::EXCEPTION_TYPE> const& getExceptions() const;

    void addException(ServiceDate const& d, EXCEPTION_TYPE t);

    bool isActiveOn(ServiceDate const& d) const;

    static SERVICE_DAY getServiceDay(ServiceDate const& d);
    uint8_t getServiceDates() const;

    EXCEPTION_TYPE getExceptionOn(ServiceDate const& d) const;

    ServiceDate const& getBeginDate() const;
    ServiceDate const& getEndDate() const;

    bool hasServiceDays() const;

    flat::Calendar getFlat() const {
        flat::Calendar c;
        c.id = _id;
        c.serviceDays = _serviceDays;
        c.begin = _begin;
        c.end = _end;
        return c;
    }

   private:
    string _id;
    uint8_t _serviceDays;
    std::map<ServiceDate, Service::EXCEPTION_TYPE> _exceptions;
    ServiceDate _begin, _end;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_SERVICE_H_
