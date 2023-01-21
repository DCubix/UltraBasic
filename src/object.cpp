#include "object.h"

namespace ulang {
  bool operator==(const Object &a, const Object &b) {
    return a.type == b.type;
  }

} // namespace ulang
