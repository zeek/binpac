#define binpac_regex_h

#include "binpac_bytestring.h"

#include <stdlib.h>

namespace binpac {

std::string std_string(bytestring const* s) {
    return std::string(reinterpret_cast<const char*>(s->begin()), reinterpret_cast<const char*>(s->end()));
}

int bytestring_to_int(bytestring const* s) { return atoi(reinterpret_cast<const char*>(s->begin())); }

double bytestring_to_double(bytestring const* s) { return atof(reinterpret_cast<const char*>(s->begin())); }

} // namespace binpac
