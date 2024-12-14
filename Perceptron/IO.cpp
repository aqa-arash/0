#include "IO.hpp"
#include <iostream>
#include <fstream>

namespace IO_MNIST {
    void readMnistHeader(const std::string &path, int &magicNumber, int &numImages, int &rows, int &cols,
                         const char ImageOrLabel = 'I') {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }
        file.read(reinterpret_cast<char *>(&magicNumber), 4);
        file.read(reinterpret_cast<char *>(&numImages), 4);
        magicNumber = static_cast<int>(__builtin_bswap32(magicNumber)); // Convert big-endian to little-endian
        numImages = static_cast<int>(__builtin_bswap32(numImages));
        if (ImageOrLabel == 'I') {
            file.read(reinterpret_cast<char *>(&rows), 4);
            file.read(reinterpret_cast<char *>(&cols), 4);
            rows = static_cast<int>(__builtin_bswap32(rows));
            cols = static_cast<int>(__builtin_bswap32(cols));
        } else {
            rows = cols = 0; // Labels file has no row/col data
        }

        file.close();
    }

    Eigen::MatrixXd loadMnistImages(const std::string &filePath, const int numImages, const int rows, const int cols) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }

        // Skip the header
        file.seekg(16, std::ios::beg);

        Eigen::MatrixXd images(numImages, rows * cols);
        for (int i = 0; i < numImages; ++i) {
            std::vector<unsigned char> buffer(rows * cols);
            file.read(reinterpret_cast<char *>(buffer.data()), rows * cols);
            for (int j = 0; j < rows * cols; ++j) {
                images(i, j) = static_cast<double>(buffer[j] / 255.0);
            }
        }

        file.close();
        return images;
    }


    Eigen::MatrixXd loadMnistLabels(const std::string &filePath, const int numLabels) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }

        // Skip the header
        file.seekg(8, std::ios::beg);

        // Initialize matrix to hold one-hot encoded labels, with all values set to zero
        Eigen::MatrixXd labels = Eigen::MatrixXd::Zero(numLabels, 10);

        for (int i = 0; i < numLabels; ++i) {
            unsigned char label;
            file.read(reinterpret_cast<char *>(&label), 1);
            int j = static_cast<int>(label);
            labels(i, j) = 1;
        }

        file.close();
        return labels;
    }


    void displayImage(const Eigen::MatrixXd &image, const int rows, const int cols) {
        for (int j = 0; j < rows; ++j) {
            for (int i = 0; i < cols; ++i) {
                if (const double pixel = image(i, j) * 255; pixel > 200) {
                    std::cout << "@";
                } else if (pixel > 150) {
                    std::cout << "#";
                } else if (pixel > 100) {
                    std::cout << "8";
                } else if (pixel > 50) {
                    std::cout << ":";
                } else {
                    std::cout << ".";
                }
            }
            std::cout << "\n";
        }
    }
}
