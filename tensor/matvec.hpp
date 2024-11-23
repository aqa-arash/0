#pragma once

#include "tensor.hpp"

template<typename ComponentType>
class Vector {
public:
    // Default-constructor.
    Vector() = default;

    // Constructor for vector of certain size with constant fill-value.
    Vector(size_t size, const ComponentType &fillValue=0);

    // Constructing vector from file.
    Vector(const std::string &filename);

    // Number of elements in this vector.
    [[nodiscard]] size_t size() const;

    // Element access function
    const ComponentType &
    operator()(size_t idx) const;

    // Element mutation function
    ComponentType &
    operator()(size_t idx);

    // Reference to internal tensor.
    Tensor<ComponentType> &tensor();

private:
    Tensor<ComponentType> tensor_;
};

template<typename ComponentType>
class Matrix {
public:
    // Default-constructor.
    Matrix() = default;

    // Constructor for matrix of certain size with constant fill-value.
    Matrix(size_t rows, size_t cols, const ComponentType &fillValue =0 );

    // Constructing matrix from file.
    Matrix(const std::string &filename);

    // Number of rows.
    [[nodiscard]] size_t rows() const;

    // Number of columns.
    [[nodiscard]] size_t cols() const;

    // Element access function
    const ComponentType &
    operator()(size_t row, size_t col) const;

    // Element mutation function
    ComponentType &
    operator()(size_t row, size_t col);

    // Reference to internal tensor.
    Tensor<ComponentType> &tensor();

private:
    Tensor<ComponentType> tensor_;
};

// TODO: Implement all methods.
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// vector 
////////////////////////////////////////////////////////////////////////////////
template<typename ComponentType>
Vector<ComponentType>::Vector(size_t size,const ComponentType &fillValue) : tensor_(std::vector<size_t>{size}, fillValue) {
}

template<typename ComponentType>
Vector<ComponentType>::Vector(const std::string &filename) : tensor_(readTensorFromFile<ComponentType>(filename)) {
}

template<typename ComponentType>
size_t Vector<ComponentType>::size() const {
    return tensor_.shape()[0];
}

template<typename ComponentType>
const ComponentType &Vector<ComponentType>::operator()(size_t idx) const {
    return tensor_({idx});
}

template<typename ComponentType>
ComponentType &Vector<ComponentType>::operator()(size_t idx) {
    return tensor_({idx});
}

template<typename ComponentType>
Tensor<ComponentType> &Vector<ComponentType>::tensor() {
    return tensor_;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// matrix
////////////////////////////////////////////////////////////////////////////////
template<typename ComponentType>
Matrix<ComponentType>::Matrix(size_t rows, size_t cols, const ComponentType &fillValue) : tensor_(std::vector<size_t>{rows, cols},fillValue) {
}

template<typename ComponentType>
Matrix<ComponentType>::Matrix(const std::string &filename) : tensor_(readTensorFromFile<ComponentType>(filename)) {
}

template<typename ComponentType>
size_t Matrix<ComponentType>::rows() const {
    return tensor_.shape()[0];
}

template<typename ComponentType>
size_t Matrix<ComponentType>::cols() const {
    return tensor_.shape()[1];
}

template<typename ComponentType>
const ComponentType &Matrix<ComponentType>::operator()(size_t row, size_t col) const {
    return tensor_({row, col});
}

template<typename ComponentType>
ComponentType &Matrix<ComponentType>::operator()(size_t row, size_t col) {
    return tensor_({row, col});
}

template<typename ComponentType>
Tensor<ComponentType> &Matrix<ComponentType>::tensor() {
    return tensor_;
}

////////////////////////////////////////////////////////////////////////////////

// Performs a matrix-vector multiplication.
template<typename ComponentType>
Vector<ComponentType> matvec(const Matrix<ComponentType> &mat, const Vector<ComponentType> &vec) {
    
    Vector<ComponentType> result(mat.rows());
    for (size_t i = 0; i < mat.rows(); ++i) {
        for (size_t j = 0; j < mat.cols(); ++j) {
            result(i) += mat(i, j) * vec(j);
        }
    }
    return result;
}
