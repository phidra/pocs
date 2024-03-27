// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_TRIP_H_
#define AD_CPPGTFS_GTFS_TRIP_H_

#include <stdint.h>

#include <algorithm>
#include <set>
#include <string>

#include "Frequency.h"
#include "Route.h"
#include "Service.h"
#include "Shape.h"
#include "Stop.h"
#include "StopTime.h"
#include "flat/Trip.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

template <typename StopTimeT, typename ServiceT, typename RouteT, typename ShapeT>
class TripB {
    // typedef std::set<StopTimeT, StopTimeCompare<StopTimeT>> StopTimes;
    typedef std::vector<StopTimeT> StopTimes;
    typedef std::vector<Frequency> Frequencies;

   public:
    typedef TripB<StopTimeT, ServiceT, RouteT, ShapeT>* Ref;
    static std::string getId(Ref r) { return r->getId(); }

    typedef flat::Trip::WC_BIKE_ACCESSIBLE WC_BIKE_ACCESSIBLE;
    typedef flat::Trip::DIRECTION DIRECTION;

    TripB() {}
    TripB(std::string const& id,
          typename RouteT::Ref r,
          typename ServiceT::Ref s,
          std::string const& hs,
          std::string const& short_name,
          DIRECTION dir,
          std::string const& blockid,
          typename ShapeT::Ref shp,
          WC_BIKE_ACCESSIBLE wc,
          WC_BIKE_ACCESSIBLE ba);

    std::string const& getId() const;
    const typename RouteT::Ref getRoute() const;
    typename RouteT::Ref getRoute();
    typename ServiceT::Ref getService();
    const typename ServiceT::Ref getService() const;
    std::string const& getHeadsign() const;
    std::string const& getShortname() const;
    DIRECTION getDirection() const;
    std::string const& getBlockId() const;
    const typename ShapeT::Ref getShape() const;
    typename ShapeT::Ref getShape();
    void setShape(typename ShapeT::Ref shp);
    WC_BIKE_ACCESSIBLE getWheelchairAccessibility() const;
    WC_BIKE_ACCESSIBLE getBikesAllowed() const;
    StopTimes const& getStopTimes() const;
    StopTimes& getStopTimes();
    Frequencies& getFrequencies();
    Frequencies const& getFrequencies() const;
    bool addStopTime(StopTimeT const& t);
    void addFrequency(Frequency const& t);

    gtfs::flat::Trip getFlat() const {
        return gtfs::flat::Trip{_id,
                                RouteT::getId(_route),
                                ServiceT::getId(_service),
                                _headsign,
                                _short_name,
                                _dir,
                                _block_id,
                                ShapeT::getId(_shape),
                                _wc,
                                _ba};
    };

   private:
    std::string _id;
    typename RouteT::Ref _route;
    typename ServiceT::Ref _service;
    std::string _headsign;
    std::string _short_name;
    DIRECTION _dir;
    std::string _block_id;
    typename ShapeT::Ref _shape;
    WC_BIKE_ACCESSIBLE _wc;
    WC_BIKE_ACCESSIBLE _ba;

    StopTimes _stoptimes;
    Frequencies _frequencies;
};

typedef TripB<StopTime<Stop>, Service, Route, Shape> Trip;

#include "Trip.tpp"

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_TRIP_H_
