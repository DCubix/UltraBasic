#pragma once

#include <variant>

namespace ulang {
  enum class ObjectType : uint8_t {
    null = 0,
    number
    // more types in the future
  };

  using Object = std::pair<ObjectType, std::variant<nullptr_t, double>>;

  bool operator ==(const Object& a, const Object& b);

}
