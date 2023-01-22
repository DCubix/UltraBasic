#pragma once

#include <variant>
#include <functional>

namespace ulang {
  class VirtualMachine;

  enum class ObjectType : uint8_t {
    null = 0,
    number,
    string,
    function
    // more types in the future
  };

  using UFunc = size_t(*)(VirtualMachine*);
  using Object = std::pair<ObjectType, std::variant<nullptr_t, double, std::string, UFunc>>;
 
  // struct Object {
  //   ObjectType type{ ObjectType::null };

  //   union {
  //     nullptr_t null;
  //     double number;
  //     size_t stringId;
  //     UFunc function;
  //   };
  // };

  bool operator ==(const Object& a, const Object& b);

}
