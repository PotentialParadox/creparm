#ifndef CONTAINER_MATH_H
#define CONTAINER_MATH_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

namespace dmath{

  //////////////////////////////////////////////
// Average
// Calculate the average of a container using 
// the iterator approach found in the STL
  //////////////////////////////////////////////
  template <typename Iter>
    double Average(Iter it, Iter end){
    double sum{0.0};
    int count{0};
    for (; it != end; ++it){
      ++count;
      sum += *it;
    }
    return sum / count;
  }

  //////////////////////////////////////////////
// Standard Diviation
// Calculate the standard deviation of
// a one dimensional container
  //////////////////////////////////////////////
  template <typename Iter>
    double STDEV(Iter it, Iter end){
    double average = Average(it, end);
    double variance{0};
    int count{0};
    for (; it != end; ++it){
      ++count;
      variance += pow(*it - average, 2);
    }
    return std::sqrt(variance / count);
  }

  //////////////////////////////////////////////
// Distance 
// Calculate the distance between two
// one dimensional containers
  //////////////////////////////////////////////
  template <typename Iter>
    double Distance(Iter it1, Iter end1, Iter it2){
    double variance{0};
    for (; it1 != end1; ++it1, ++it2){
      variance += pow(*it1 - *it2, 2);
    }
    return sqrt(variance);
  }

  //////////////////////////////////////////////
// Distance 
// Reads a one dimensional container
// and returns a size - 1 container with
// values equal to the difference of the
// original values
// {1 , 2, 5} -> {1, 3}
  //////////////////////////////////////////////
  template <typename T, typename Iter>
    std::vector<T> Difference(Iter it, Iter end){
    std::vector<T> v;
    Iter it1 = std::next(it, 1);
    for (; it1 != end; ++it, ++it1){
      v.emplace_back(*it1 - *it);
    }
    return v;
  }

  
}

#endif /* CONTAINER_MATH_H */
