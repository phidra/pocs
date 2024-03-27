// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FEED_H_
#define AD_CPPGTFS_GTFS_FEED_H_

#include <iterator>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "Agency.h"
#include "ContContainer.h"
#include "Container.h"
#include "Fare.h"
#include "Route.h"
#include "Service.h"
#include "Shape.h"
#include "Stop.h"
#include "Transfer.h"
#include "Trip.h"

#define FEEDTPL                   \
    template <typename AgencyT,   \
              typename RouteT,    \
              typename StopT,     \
              typename ServiceT,  \
              template <typename> \
              class StopTimeT,    \
              typename ShapeT,    \
              template <typename> \
              class FareT,        \
              template <typename> \
              class AContainerT,  \
              template <typename> \
              class RContainerT,  \
              template <typename> \
              class SContainerT,  \
              template <typename> \
              class StContainerT, \
              template <typename> \
              class TContainerT,  \
              template <typename> \
              class ShContainerT, \
              template <typename> \
              class FContainerT>
#define FEEDB           \
    FeedB<AgencyT,      \
          RouteT,       \
          StopT,        \
          ServiceT,     \
          StopTimeT,    \
          ShapeT,       \
          FareT,        \
          AContainerT,  \
          RContainerT,  \
          SContainerT,  \
          StContainerT, \
          TContainerT,  \
          ShContainerT, \
          FContainerT>

namespace ad {
namespace cppgtfs {
namespace gtfs {

FEEDTPL
class FeedB {
    typedef AContainerT<AgencyT> Agencies;
    typedef StContainerT<StopT> Stops;
    typedef RContainerT<RouteT> Routes;
    typedef TContainerT<TripB<StopTimeT<StopT>, ServiceT, RouteT, ShapeT>> Trips;
    typedef ShContainerT<ShapeT> Shapes;
    typedef SContainerT<ServiceT> Services;
    typedef FContainerT<FareT<RouteT>> Fares;
    typedef std::vector<Transfer> Transfers;
    typedef std::set<std::string> Zones;

   public:
    FeedB() :
        _maxLat(std::numeric_limits<double>::lowest()),
        _maxLon(std::numeric_limits<double>::lowest()),
        _minLat(std::numeric_limits<double>::max()),
        _minLon(std::numeric_limits<double>::max()) {}

    Agencies const& getAgencies() const;
    Agencies& getAgencies();

    Stops const& getStops() const;
    Stops& getStops();

    Routes const& getRoutes() const;
    Routes& getRoutes();

    Trips const& getTrips() const;
    Trips& getTrips();

    Shapes const& getShapes() const;
    Shapes& getShapes();
    Services const& getServices() const;
    Services& getServices();

    Transfers const& getTransfers() const;
    Transfers& getTransfers();

    Zones const& getZones() const;
    Zones& getZones();

    Fares const& getFares() const;
    Fares& getFares();

    std::string const& getPublisherName() const;
    std::string const& getPublisherUrl() const;
    std::string const& getLang() const;
    std::string const& getVersion() const;
    ServiceDate const& getStartDate() const;
    ServiceDate const& getEndDate() const;

    void setPublisherName(std::string const& name);
    void setPublisherUrl(std::string const& url);
    void setLang(std::string const& lang);
    void setVersion(std::string const& version);
    void setStartDate(ServiceDate const& start);
    void setEndDate(ServiceDate const& end);

    void updateBox(double lat, double lon);
    double getMinLat() const;
    double getMinLon() const;
    double getMaxLat() const;
    double getMaxLon() const;

    std::string const& getPath() const { return _path; }
    void setPath(std::string const& p) { _path = p; }

   private:
    Agencies _agencies;
    Stops _stops;
    Routes _routes;
    Trips _trips;
    Shapes _shapes;
    Services _services;
    Transfers _transfers;
    Zones _zones;
    Fares _fares;

    double _maxLat, _maxLon, _minLat, _minLon;

    std::string _publisherName, _publisherUrl, _lang, _version, _path;
    ServiceDate _startDate, _endDate;
};

typedef FeedB<Agency,
              Route,
              Stop,
              Service,
              StopTime,
              Shape,
              Fare,
              Container,
              Container,
              Container,
              Container,
              Container,
              Container,
              Container>
    Feed;
typedef FeedB<Agency,
              Route,
              Stop,
              Service,
              StopTime,
              Shape,
              Fare,
              ContContainer,
              ContContainer,
              ContContainer,
              ContContainer,
              ContContainer,
              ContContainer,
              ContContainer>
    ContFeed;

#include "Feed.tpp"

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FEED_H_
