#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <initializer_list>

namespace stu {

  namespace vec {
    
    template <typename T>
    bool contains(const std::vector<T>& vec, T value) {
      return std::find(vec.begin(), vec.end(), value) != vec.end();
    }

    template <typename R, typename T>
    R reduce(const std::vector<T>& vec, const std::function<R(T, T)>& reducer) {
      R accumulator = R(0);
      for (size_t i = 0; i < vec.size(); i++) {
        accumulator = reducer(accumulator, vec[i]);
      }
      return accumulator;
    }

    template <typename T>
    T at(const std::initializer_list<T>& vec, size_t index) {
      return (*(vec.begin() + index));
    }

  }

}
