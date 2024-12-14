#ifndef IO_HPP
#define IO_HPP

#pragma once

#include <fstream>
#include <Eigen/Dense>

namespace IO_MNIST {
    void readMnistHeader(const std::string &path, int &magicNumber, int &numImages, int &rows, int &cols,
                         char ImageOrLabel);

    Eigen::MatrixXd loadMnistImages(const std::string &filePath, int numImages, int rows, int cols);

    Eigen::MatrixXd loadMnistLabels(const std::string &filePath, int numLabels);

    void displayImage(const Eigen::MatrixXd &image, int rows, int cols);
};

#endif //IO_HPP
