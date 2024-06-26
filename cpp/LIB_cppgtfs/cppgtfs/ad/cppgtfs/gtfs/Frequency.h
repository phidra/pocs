// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_FREQUENCY_H_
#define AD_CPPGTFS_GTFS_FREQUENCY_H_

#include "StopTime.h"
#include "flat/Frequency.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

class Frequency {
   public:
    Frequency(Time const& startTime, Time const& endTime, uint16_t headwaySecs, bool exactTimes) :
        _startTime(startTime), _endTime(endTime), _headwaySecs(headwaySecs), _exactTimes(exactTimes) {}

    Time const& getStartTime() const { return _startTime; }
    Time const& getEndTime() const { return _endTime; }
    uint16_t getHeadwaySecs() const { return _headwaySecs; }
    bool hasExactTimes() const { return _exactTimes; }

   private:
    Time _startTime, _endTime;
    uint16_t _headwaySecs;
    bool _exactTimes;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_FREQUENCY_H_
