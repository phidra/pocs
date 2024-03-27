// Copyright 2016, University of Freiburg,
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#include "Service.h"

#include <iostream>
#include <map>
#include <string>

using ad::cppgtfs::gtfs::Service;
using ad::cppgtfs::gtfs::ServiceDate;

// _____________________________________________________________________________
Service::Service(std::string const& id) : _id(id), _serviceDays(Service::SERVICE_DAY::NEVER), _begin(), _end() {}

// _____________________________________________________________________________
Service::Service(std::string const& id, uint8_t serviceDays, ServiceDate start, ServiceDate end) :
    _id(id), _serviceDays(serviceDays), _begin(start), _end(end) {}

// _____________________________________________________________________________
std::string const& Service::getId() const {
    return _id;
}

// _____________________________________________________________________________
std::map<ServiceDate, Service::EXCEPTION_TYPE> const& Service::getExceptions() const {
    return _exceptions;
}

// _____________________________________________________________________________
void Service::addException(ServiceDate const& d, Service::EXCEPTION_TYPE t) {
    _exceptions[d] = t;
}

// _____________________________________________________________________________
bool Service::isActiveOn(ServiceDate const& d) const {
    return ((d >= _begin && d <= _end) && (_serviceDays & getServiceDay(d)) &&
            getExceptionOn(d) != EXCEPTION_TYPE::SERVICE_REMOVED) ||
           getExceptionOn(d) == EXCEPTION_TYPE::SERVICE_ADDED;
}

// _____________________________________________________________________________
ServiceDate const& Service::getBeginDate() const {
    return _begin;
}

// _____________________________________________________________________________
ServiceDate const& Service::getEndDate() const {
    return _end;
}

// _____________________________________________________________________________
uint8_t Service::getServiceDates() const {
    return _serviceDays;
}

// _____________________________________________________________________________
Service::SERVICE_DAY Service::getServiceDay(ServiceDate const& d) {
    return static_cast<SERVICE_DAY>(1 << (((d.getTimeStrc().tm_wday + 6) % 7)));
}

// _____________________________________________________________________________
Service::EXCEPTION_TYPE Service::getExceptionOn(ServiceDate const& d) const {
    auto ex = _exceptions.find(d);
    if (ex != _exceptions.end())
        return ex->second;
    return EXCEPTION_TYPE::NOT_SET;
}

// _____________________________________________________________________________
bool Service::hasServiceDays() const {
    return !_begin.empty() && !_end.empty();
}
