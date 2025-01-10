// Copyright 2016, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Authors: Patrick Brosi <brosi@informatik.uni-freiburg.de>

#ifndef AD_CPPGTFS_GTFS_AGENCY_H_
#define AD_CPPGTFS_GTFS_AGENCY_H_

#include <string>

#include "flat/Agency.h"

using std::exception;
using std::string;

namespace ad {
namespace cppgtfs {
namespace gtfs {

class Agency {
   public:
    typedef Agency* Ref;
    static std::string getId(Ref r) { return r->getId(); }

    Agency() {}

    Agency(std::string const& id,
           std::string const& name,
           std::string const& url,
           std::string const& timezone,
           std::string const& lang,
           std::string const& phone,
           std::string const& fare_url,
           std::string const& agency_email) :
        _id(id),
        _name(name),
        _url(url),
        _timezone(timezone),
        _lang(lang),
        _phone(phone),
        _fare_url(fare_url),
        _agency_email(agency_email) {}

    Agency(char const* id,
           char const* name,
           char const* url,
           char const* timezone,
           char const* lang,
           char const* phone,
           char const* fare_url,
           char const* agency_email) :
        _id(id),
        _name(name),
        _url(url),
        _timezone(timezone),
        _lang(lang),
        _phone(phone),
        _fare_url(fare_url),
        _agency_email(agency_email) {}

    std::string const& getId() const { return _id; }

    std::string const& getName() const { return _name; }

    std::string const& getUrl() const { return _url; }

    std::string const& getTimezone() const { return _timezone; }

    std::string const& getLang() const { return _lang; }

    std::string const& getPhone() const { return _phone; }

    std::string const& getFareUrl() const { return _fare_url; }

    std::string const& getAgencyEmail() const { return _agency_email; }

    flat::Agency getFlat() const {
        flat::Agency r;
        r.id = _id;
        r.name = _name;
        r.url = _url;
        r.timezone = _timezone;
        r.lang = _lang;
        r.phone = _phone;
        r.fare_url = _fare_url;
        r.agency_email = _agency_email;
        return r;
    }

    // TODO(patrick): implement setters

   private:
    std::string _id, _name, _url, _timezone, _lang, _phone, _fare_url, _agency_email;
};

}  // namespace gtfs
}  // namespace cppgtfs
}  // namespace ad

#endif  // AD_CPPGTFS_GTFS_AGENCY_H_
