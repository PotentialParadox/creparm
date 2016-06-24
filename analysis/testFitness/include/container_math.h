#ifndef CONTAINER_MATH_H
#define CONTAINER_MATH_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <armadillo>

namespace dmath{

    //////////////////////////////////////////////
// Average
// Calculate the average of a container using
// the iterator approach found in the STL
    //////////////////////////////////////////////
    template <typename Iter>
    double Average(Iter it, Iter end){
        if (it == end)
            throw "Cannot perform Average on empty container";
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
        if (it == end)
            throw "Cannot perform STDEV on empty container";
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
        if (it1 == end1)
            throw "Cannot perform Distance on empty container";
        double variance{0};
        for (; it1 != end1; ++it1, ++it2){
            variance += pow(*it1 - *it2, 2);
        }
        return sqrt(variance);
    }

    //////////////////////////////////////////////
// Differences
// Reads a one dimensional container
// and returns a size - 1 container with
// values equal to the difference of the
// original values
// {1 , 2, 5} -> {1, 3}
    //////////////////////////////////////////////
    template <typename T, typename Iter>
    std::vector<T> Differences(Iter it, Iter end){
        if (it == end)
            throw "Cannot perform Differences on empty container";
        std::vector<T> v;
        Iter it1 = std::next(it, 1);
        for (; it1 != end; ++it, ++it1){
            v.emplace_back(*it1 - *it);
        }
        return v;
    }

    //////////////////////////////////////////////
// VectorDifference
// Returns a vector representing the difference
// between two vectors
    //////////////////////////////////////////////
    template <typename T, typename Iter>
    std::vector<T> VectorDifference(Iter it, Iter end, Iter it1){
        if (it == end)
            throw "Cannot perform VectorDifference on empty container";
        std::vector<T> v;
        for (; it != end; ++it, ++it1)
            v.emplace_back(*it - *it1);
        return v;
    }


    //////////////////////////////////////////////
// VectorOneToTwo
// Converts a one dimensional vector into two
    //////////////////////////////////////////////
    template <typename T>
    std::vector<std::vector<T> > VectorOneToTwo(const std::vector<T> &v, const int columns){
        std::vector<std::vector<T> > array;
        if ((v.size() % columns) || columns <= 0)
            throw "cannot not convert dimensions";
        for (size_t i = 0; i < v.size(); i+=columns){
            std::vector<T> temp_v;
            for (size_t j = 0; j != static_cast<size_t>(columns); ++j){
                temp_v.push_back(v[i+j]);
            }
            array.push_back(temp_v);
        }
        return array;
    }

    //////////////////////////////////////////////
// DifferenceMatrix
// Converts a one dimensional vector into a
// two dimensional array representing the
// differences between each member of the vector
    //////////////////////////////////////////////
    template <typename T>
    arma::Mat<T> DifferenceMatrix(const std::vector<T> &v){
        std::vector<T> v_temp(v.size() * v.size());
        for (size_t i = 0; i != v.size(); ++i){
            for (size_t j = 0; j != v.size(); ++j){
                /* Armadillo matrices are column major. */
                v_temp[i + v.size() * j] = std::abs(v[i] - v[j]);
            }
        }
        return arma::Mat<T>(v_temp);
    }

    //////////////////////////////////////////////
// DifferenceVectorMatrix
// Converts a one dimensional vector of vectors
// into a two dimensional array representing the
// differences between each member of the original
// vector
    //////////////////////////////////////////////
    template <typename T>
    std::vector<std::vector<std::vector<T> > >
    DifferenceVectorMatrix(const std::vector<std::vector<T> > &v){
        /* The size of a vector */
        size_t v_length = v[0].size();
        /* The number of vectors */
        size_t number_vectors = v.size();

        /* Allocate the array */
        std::vector<T> vec(v_length);
        std::vector<std::vector<T> > row(number_vectors, vec);
        std::vector<std::vector<std::vector<T> > > mat(number_vectors, row);

        /* Populated the difference matrix */
        for (size_t i = 0; i != number_vectors; ++i)
            for (size_t j = i + 1; j < number_vectors; ++j){
                mat[i][j] = VectorDifference<T>(std::begin(v[i]), std::end(v[i]), std::begin(v[j]));
                mat[j][i] = mat[i][j];
            }


        return mat;
    }

    //////////////////////////////////////////////
// DistanceVectorMatrix
// Finds the distances between the vector
// components of two arrays of vectos
    //////////////////////////////////////////////
    template <typename T>
    std::vector<std::vector<T> >
    DistanceVectorMatrix(const std::vector<std::vector<std::vector<T> > > &v,
                         const std::vector<std::vector<std::vector<T> > > &w){
        std::vector<T> row(v.size());
        std::vector<std::vector<T> > mat(v.size(), row);

        for (size_t i = 0; i != v.size(); ++i)
            for (size_t j = 0; j != v.size(); ++j){
                mat[i][j] = Distance(std::begin(v[i][j]), std::end(v[i][j]), std::begin(w[i][j]));
            }

        return mat;
    }


    //////////////////////////////////////////////
// MatrixAverage
// Calculate the average of a vector of vectors
    //////////////////////////////////////////////
    template <typename T>
    T MatrixAverage(const std::vector<std::vector<T> > &mat){
        if (mat.size() == 0 || mat[0].size() == 0)
            throw "Dimensions of matrix must be > 0";
        T value = 0;
        for (const auto &i: mat)
            for (const auto &j: i){
                value += j;
            }
        value /= (mat.size() * mat[0].size());
        return value;
    }
}

#endif /* CONTAINER_MATH_H */
