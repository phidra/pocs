// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_STOP_H_
#define AD_CPPGTFS_GTFS_STOP_H_

#include <stdint.h>

#include <cassert>
#include <string>

#include "flat/Stop.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

class Stop {
   public:
    typedef Stop* Ref;
    static std::string getId(Ref r) { return r->getId(); }
    typedef flat::Stop::LOCATION_TYPE LOCATION_TYPE;
    typedef flat::Stop::WHEELCHAIR_BOARDING WHEELCHAIR_BOARDING;

    Stop() {}

    Stop(string const& id,
         string const& code,
         string const& name,
         string const& desc,
         float lat,
         float lng,
         string zone_id,
         string const& stop_url,
         flat::Stop::LOCATION_TYPE location_type,
         Stop* parent_station,
         string const& stop_timezone,
         flat::Stop::WHEELCHAIR_BOARDING wheelchair_boarding,
         std::string const& platform_code) :
        _id(id),
        _code(code),
        _name(name),
        _desc(desc),
        _zone_id(zone_id),
        _stop_url(stop_url),
        _stop_timezone(stop_timezone),
        _platform_code(platform_code),
        _parent_station(parent_station),
        _lat(lat),
        _lng(lng),
        _wheelchair_boarding(wheelchair_boarding),
        _location_type(location_type) {}

    std::string const& getId() const { return _id; }

    std::string const& getCode() const { return _code; }

    std::string const& getName() const { return _name; }

    std::string const& getPlatformCode() const { return _platform_code; }

    std::string const& getDesc() const { return _desc; }

    float getLat() const { return _lat; }

    float getLng() const { return _lng; }

    std::string const& getZoneId() const { return _zone_id; }

    std::string const& getStopUrl() const { return _stop_url; }

    flat::Stop::LOCATION_TYPE getLocationType() const { return _location_type; }

    Stop const* getParentStation() const { return _parent_station; }

    Stop* getParentStation() { return _parent_station; }

    void setParentStation(Stop* p) { _parent_station = p; }

    std::string const& getStopTimezone() const { return _stop_timezone; }

    flat::Stop::WHEELCHAIR_BOARDING getWheelchairBoarding() const { return _wheelchair_boarding; }

    flat::Stop getFlat() const {
        flat::Stop r;
        r.id = _id;
        r.code = _code;
        r.name = _name;
        r.desc = _desc;
        r.zone_id = _zone_id;
        r.stop_url = _stop_url;
        r.stop_timezone = _stop_timezone;
        r.platform_code = _platform_code;
        r.parent_station = (_parent_station ? _parent_station->getId() : "");
        r.lat = _lat;
        r.lng = _lng;
        r.wheelchair_boarding = _wheelchair_boarding;
        r.location_type = _location_type;
        return r;
    }

    // TODO(patrick): implement setters

   private:
    string _id, _code, _name, _desc, _zone_id, _stop_url, _stop_timezone, _platform_code;
    Stop* _parent_station;
    float _lat, _lng;
    flat::Stop::WHEELCHAIR_BOARDING _wheelchair_boarding;
    flat::Stop::LOCATION_TYPE _location_type;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_STOP_H_
