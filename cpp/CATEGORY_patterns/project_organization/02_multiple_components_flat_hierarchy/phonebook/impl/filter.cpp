#include "filter.h"

bool is_forbidden(std::string name) {
    if (name.size() == 0) {
        return false;
    }
    return name.front() == 'M';  // we forbid names beginning by an M
}
