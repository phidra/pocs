#pragma once

#include <istream>

#include "common/polygon.h"

static const std::string NO_POLYGON = "NONE";

BgPolygon get_polygon(std::string polygonfile_path);
