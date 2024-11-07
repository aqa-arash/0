#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <numeric>


template<class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic ComponentType>
class Tensor {
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor() = default;

    // Constructs a tensor with arbitrary shape and zero-initializes all elements. Only use positive values in the shape.
    explicit Tensor(const std::vector<size_t> &shape);

    // Constructs a tensor with arbitrary shape and fills it with the specified value. Only use positive values in the shape.
    explicit Tensor(const std::vector<size_t> &shape, const ComponentType &fillValue);

    // Copy-constructor.
    Tensor(const Tensor<ComponentType> &other);

    // Move-constructor.
    Tensor(Tensor<ComponentType> &&other) noexcept;

    // Copy-assignment
    // just using std::vector -> deep copy from std library sufficient
    Tensor &
    operator=(const Tensor<ComponentType> &other) = default;

    // Move-assignment
    Tensor &
    operator=(Tensor<ComponentType> &&other) noexcept;

    // Destructor
    ~Tensor() = default;

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const;

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector<size_t> shape() const;

    // Returns the number of elements of this tensor.
    [[nodiscard]] size_t numElements() const;

    // Element access function
    const ComponentType &
    operator()(const std::vector<size_t> &idx) const;

    // Element mutation function
    ComponentType &
    operator()(const std::vector<size_t> &idx);

private:
    // TODO: Probably you need some members here...
    std::vector<size_t> _tensor_shape;
    std::vector<ComponentType> _data;

    // calculates the index in the flattened array from the rank dim vector
    [[nodiscard]] size_t coord_to_index(const std::vector<size_t> &coords) const;

    [[nodiscard]] std::vector<size_t> index_to_coord(size_t index) const;

    // only insert non-negative values
    static size_t calc_size(const std::vector<size_t> &shape) noexcept;
};


// TODO: Implement all methods of the Tensor class template.
template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(const std::vector<size_t> &shape) : _tensor_shape(shape),
                                                                  _data(calc_size(shape)) {
}

template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(const std::vector<size_t> &shape, const ComponentType &fillValue) :
    _tensor_shape(shape),
    _data(calc_size(shape), fillValue) {
}

template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(const Tensor<ComponentType> &other) : _tensor_shape(other._tensor_shape),
                                                                    _data(other._data) {
}

template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(Tensor<ComponentType> &&other) noexcept : _tensor_shape(std::move(other._tensor_shape)),
                                                                        _data(std::move(other._data)) {
    other._tensor_shape.clear();
    other._data.clear();
}

template<Arithmetic ComponentType>
Tensor<ComponentType> &Tensor<ComponentType>::operator=(Tensor<ComponentType> &&other) noexcept {
    if (this != &other) {
        // Check for self-assignment
        _tensor_shape = std::move(other._tensor_shape);
        _data = std::move(other._data);
        // no need to clear the other, since it is a temporary
    }
    return *this;
}

template<Arithmetic ComponentType>
const ComponentType &Tensor<ComponentType>::operator()(const std::vector<size_t> &idx) const {
    if (idx.size() != _tensor_shape.size()) {
        throw std::out_of_range("Index does not match tensor rank");
    }

    // Check if all indices are within bounds
    for (size_t i = 0; i < idx.size(); ++i) {
        if (idx[i] >= _tensor_shape[i]) {
            throw std::out_of_range("Index out of bounds");
        }
    }

    return _data[coord_to_index(idx)];
}

template<Arithmetic ComponentType>
ComponentType &Tensor<ComponentType>::operator()(const std::vector<size_t> &idx) {
    if (idx.size() != _tensor_shape.size()) {
        throw std::out_of_range("Index size does not match tensor rank");
    }

    // Check if all indices are within bounds
    for (size_t i = 0; i < idx.size(); ++i) {
        if (idx[i] >= _tensor_shape[i]) {
            throw std::out_of_range("Index out of bounds");
        }
    }

    return _data[coord_to_index(idx)];
}

// Returns true if the shapes and all elements of both tensors are equal.
template<Arithmetic ComponentType>
bool operator==(const Tensor<ComponentType> &a, const Tensor<ComponentType> &b) {
    // TODO: Implement this comparison.

    // Check if a and b are the same instance
    if (&a == &b) return true;

    // Check if shapes are the same
    if (a._tensor_shape != b._tensor_shape) {
        return false;
    }

    // Check if data is the same
    return a._data == b._data;
}

// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).
template<Arithmetic ComponentType>
std::ostream &
operator<<(std::ostream &out, const Tensor<ComponentType> &tensor) {
    // TODO (optional): Implement some nice stdout printer for debugging/exercise.
}

// Reads a tensor from file.
template<Arithmetic ComponentType>
Tensor<ComponentType> readTensorFromFile(const std::string &filename) {
    // TODO: Implement this function to read in tensors from file.
    //       The format is defined in the instructions.
}

// Writes a tensor to file.
template<Arithmetic ComponentType>
void writeTensorToFile(const Tensor<ComponentType> &tensor, const std::string &filename) {
    // TODO: Implement this function to write tensors to file.
    //       The format is defined in the instructions.
}

template<Arithmetic ComponentType>
size_t Tensor<ComponentType>::coord_to_index(const std::vector<size_t> &coords) const {
    size_t index = 0;
    size_t multiplier = 1;
    for (size_t i = _tensor_shape.size(); i-- > 0;) {
        index += coords[i] * multiplier;
        multiplier *= _tensor_shape[i];
    }
    return index;
}

template<Arithmetic ComponentType>
std::vector<size_t> Tensor<ComponentType>::index_to_coord(size_t index) const {
    std::vector<size_t> coords(_tensor_shape.size());
    for (size_t i = _tensor_shape.size(); i-- > 0;) {
        coords[i] = index % _tensor_shape[i];
        index /= _tensor_shape[i];
    }
    return coords;
}

template<Arithmetic ComponentType>
size_t Tensor<ComponentType>::calc_size(const std::vector<size_t> &shape) {
    if (shape.empty()) {
        return 1;
    }
    size_t tensor_size = 1;
    for (const size_t i: shape) {
        tensor_size *= i;
    }
    return tensor_size;
}
