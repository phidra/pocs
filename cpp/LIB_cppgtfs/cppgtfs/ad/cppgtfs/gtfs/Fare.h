// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FARE_H_
#define AD_CPPGTFS_GTFS_FARE_H_

#include <string>
#include <vector>

#include "Agency.h"
#include "Route.h"
#include "flat/Fare.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

template <typename RouteT>
class FareRule {
   public:
    FareRule() {}

    FareRule(typename RouteT::Ref route,
             std::string const& originId,
             std::string const& destId,
             std::string const& containsId) :
        _route(route), _originId(originId), _destId(destId), _containsId(containsId) {}

    typename RouteT::Ref getRoute() const { return _route; }
    std::string const& getOriginId() const { return _originId; }
    std::string const& getDestId() const { return _destId; }
    std::string const& getContainsId() const { return _containsId; }

   private:
    typename RouteT::Ref _route;
    std::string _originId;
    std::string _destId;
    std::string _containsId;
};

template <typename RouteT>
class Fare {
   public:
    typedef Fare<RouteT>* Ref;
    static std::string getId(Ref r) { return r->getId(); }
    typedef flat::Fare::PAYMENT_METHOD PAYMENT_METHOD;
    typedef flat::Fare::NUM_TRANSFERS NUM_TRANSFERS;

    Fare() {}

    Fare(std::string const& id,
         double price,
         std::string const& currencyType,
         PAYMENT_METHOD paymentMethod,
         NUM_TRANSFERS numTransfers,
         Agency* agency,
         int64_t dur) :
        _id(id),
        _price(price),
        _currencyType(currencyType),
        _paymentMethod(paymentMethod),
        _numTransfers(numTransfers),
        _agency(agency),
        _duration(dur) {}

    std::string const& getId() const { return _id; }

    double getPrice() const { return _price; }

    std::string const& getCurrencyType() const { return _currencyType; }

    PAYMENT_METHOD getPaymentMethod() const { return _paymentMethod; }

    NUM_TRANSFERS getNumTransfers() const { return _numTransfers; }

    Agency* getAgency() const { return _agency; }

    int64_t getDuration() const { return _duration; }

    std::vector<FareRule<RouteT>> const& getFareRules() const { return _fareRules; }

    void addFareRule(FareRule<RouteT> const& rule) { _fareRules.push_back(rule); }

    flat::Fare getFlat() const {
        return flat::Fare{
            _id, _price, _currencyType, _paymentMethod, _numTransfers, _agency ? _agency->getId() : "", _duration};
    }

    // TODO(patrick): implement setters

   private:
    std::string _id;
    double _price;
    std::string _currencyType;
    PAYMENT_METHOD _paymentMethod;
    NUM_TRANSFERS _numTransfers;
    Agency* _agency;
    int64_t _duration;

    std::vector<FareRule<RouteT>> _fareRules;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FARE_H_
