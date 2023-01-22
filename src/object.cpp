#include "object.h"

namespace ulang {
  bool operator==(const Object &a, const Object &b) {
    return a.first == b.first;
  }

} // namespace ulang
