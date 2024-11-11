#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <fstream>


template<class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic ComponentType>
class Tensor {
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor() :  _tensor_shape({}), _data({0}) {}

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

    // Friend function for equality comparison
    template <Arithmetic T>
    friend bool operator==(const Tensor<T> &a, const Tensor<T> &b);

    // Destructor
    ~Tensor() = default;

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const {return _tensor_shape.size();}

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector<size_t> shape() const {return _tensor_shape;}

    // Returns the number of elements of this tensor.
    [[nodiscard]] size_t numElements() const {return _data.size();}

    // only insert non-negative values
    static size_t calc_size(const std::vector<size_t> &shape) noexcept;

    // Element access function
    const ComponentType &
    operator()(const std::vector<size_t> &idx) const;

    // Element mutation function
    ComponentType &
    operator()(const std::vector<size_t> &idx);

    // Direct Reference used when reading data from a file or writing data to a file
    ComponentType &Flat_idx(const size_t idx);
    const ComponentType &Flat_idx (const size_t idx) const;

private:
    // TODO: Probably you need some members here...
    std::vector<size_t> _tensor_shape;
    std::vector<ComponentType> _data;

    // calculates the index in the flattened array from the rank dim vector
    [[nodiscard]] size_t coord_to_index(const std::vector<size_t> &coords) const;

    [[nodiscard]] std::vector<size_t> index_to_coord(size_t index) const;


};


// TODO: Implement all methods of the Tensor class template.

// constructor with initial values 0
template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(const std::vector<size_t> &shape) : _tensor_shape(shape),
                                                                  _data(calc_size(shape)) {
}

// constructor with defined initial values
template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(const std::vector<size_t> &shape, const ComponentType &fillValue) :
    _tensor_shape(shape),
    _data(calc_size(shape), fillValue) {
}

// copy contructor
// TODO: check correct data passed ?
template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(const Tensor<ComponentType> &other) : _tensor_shape(other._tensor_shape),
                                                                    _data(other._data) {
}

// move initializer
// TODO: check correct data pass
template<Arithmetic ComponentType>
Tensor<ComponentType>::Tensor(Tensor<ComponentType> &&other) noexcept : _tensor_shape(std::move(other._tensor_shape)),
                                                                        _data(std::move(other._data)) {
    other._data={0};
    other._tensor_shape={};
}

// Move operator
// TODO: What if we want to copy? it is more common imo
template<Arithmetic ComponentType>
Tensor<ComponentType> &Tensor<ComponentType>::operator=(Tensor<ComponentType> &&other) noexcept {
    if (this != &other) {
        // Check for self-assignment
        _tensor_shape = std::move(other._tensor_shape);
        _data = std::move(other._data);
        other._data={0};
        other._tensor_shape={};
        // no need to clear the other, since it is a temporary
    }
    return *this;
}

// Accessors
// Const
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

// Reference
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

// Direct Reference used when reading data from a file or writing data to a file
template<Arithmetic ComponentType>
ComponentType &Tensor<ComponentType>::Flat_idx(const size_t idx) {
    if (idx >= _data.size()) {
        throw std::out_of_range("Flat Index out of bounds");
    }
    return _data[idx];
}

// Direct Reference used when reading data from a file or writing data to a file
template<Arithmetic ComponentType>
const ComponentType &Tensor<ComponentType>::Flat_idx (const size_t idx) const{
    if (idx >= _data.size()) {
        throw std::out_of_range("Flat Index out of bounds");
    }
    return _data[idx];
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
/* template<Arithmetic ComponentType>
std::ostream &
operator<<(std::ostream &out, const Tensor<ComponentType> &tensor) {
    // TODO (optional): Implement some nice stdout printer for debugging/exercise.

} */

// Reads a tensor from file.
template<Arithmetic ComponentType>
Tensor<ComponentType> readTensorFromFile(const std::string &filename) {
    // TODO: Implement this function to read in tensors from file.
    //       The format is defined in the instructions.
    std::ifstream file(filename);
    std::string line="",dataname="";
    std::vector<size_t> shape={};
    size_t rank=0;
    if (file.is_open()){
        getline(file,line);
        rank = std::stoi(line);
        for (size_t i = 0; i < rank; ++i){
            getline(file,line);
            shape.push_back(std::stoi(line));
        }
        Tensor<ComponentType> data(shape);
        size_t idx = 0;
        while (getline(file,line)){
            data.Flat_idx(idx) = std::stod(line);
            ++idx;
        }
        file.close();
        return data;
    }
    else {
        std::cout << "Unable to open file";
        // return empty tensor
        return Tensor<ComponentType>();
    }
}

// Writes a tensor to file.
template<Arithmetic ComponentType>
void writeTensorToFile(const Tensor<ComponentType> &tensor, const std::string &filename) {
    // TODO: Test this function to write tensors to file.
    //       The format is defined in the instructions.
    std::ofstream tensor_file;
    tensor_file.open (filename);
    if (tensor_file.is_open()){
        tensor_file << tensor.rank()<< std::endl;
        for (size_t i = 0; i < tensor.rank(); ++i){
            tensor_file << tensor.shape()[i]<< std::endl;
        }
        for (size_t i = 0; i < tensor.numElements(); ++i){
            tensor_file << tensor.Flat_idx(i)<< std::endl;
        }
    tensor_file.close();
    }
    else {
        std::cout << "Unable to open file";
        }
}

// for a undefined rank the last index is the fastest

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

// calculates number of elements in tensor
template<Arithmetic ComponentType>
size_t Tensor<ComponentType>::calc_size(const std::vector<size_t> &shape) noexcept {
    if (shape.empty()) {
        return 1;
    }
    size_t tensor_size = 1;
    for (const size_t i: shape) {
        tensor_size *= i;
    }
    return tensor_size;
}
