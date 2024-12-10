// #pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include "Eigen/Dense"

void readMnistHeader(const std::string& path, int& magicNumber, int& numImages, int& rows, int& cols) {
    bool isLabel = path.find("labels") != std::string::npos;
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    file.read(reinterpret_cast<char*>(&magicNumber), 4);
    file.read(reinterpret_cast<char*>(&numImages), 4);
    magicNumber = __builtin_bswap32(magicNumber); // Convert big-endian to little-endian
    numImages = __builtin_bswap32(numImages);
    if (!isLabel) {
        file.read(reinterpret_cast<char*>(&rows), 4);
        file.read(reinterpret_cast<char*>(&cols), 4);
        rows = __builtin_bswap32(rows);
        cols = __builtin_bswap32(cols);
    } else {
        rows = cols = 0; // Labels file has no row/col data
    }
    
    file.close();
    
}

Eigen::MatrixXd loadMnistImages(const std::string& filePath, int numImages, int rows, int cols) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    // Skip the header
    file.seekg(16, std::ios::beg);

    Eigen::MatrixXd images(numImages, rows * cols);
    for (int i = 0; i < numImages; ++i) {
        std::vector<unsigned char> buffer(rows * cols);
        file.read(reinterpret_cast<char*>(buffer.data()), rows * cols);
        for (int j = 0; j < rows * cols; ++j) {
            images(i, j) = static_cast<double>(buffer[j]/255.0);
        }
    }

    file.close();
    return images;
}


Eigen::VectorXi loadMnistLabels(const std::string& filePath, int numLabels) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    // Skip the header
    file.seekg(8, std::ios::beg);

    Eigen::VectorXi labels(numLabels);
    for (int i = 0; i < numLabels; ++i) {
        unsigned char label;
        file.read(reinterpret_cast<char*>(&label), 1);
        labels(i) = static_cast<int>(label);
    }

    file.close();
    return labels;
}


void displayImage(const Eigen::MatrixXd& image, int rows, int cols) {
    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < cols; ++i) {
            double pixel = image(i, j)*255;
            if (pixel > 200) {
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

int main() {
    try {
        const std::string imagePath = "./dataset/t10k-images.idx3-ubyte";
        const std::string labelPath = "./dataset/t10k-labels.idx1-ubyte";

        int labelMagic, numLabels,nc,nr;
        readMnistHeader(labelPath, labelMagic, numLabels,nr,nc);
        std::cout << "Labels File Header:\n"
                  << "Magic Number: " << labelMagic << "\n"
                  << "Number of Labels: " << numLabels << "\n";

        
        int magic, numImages, rows, cols;
        readMnistHeader(imagePath, magic, numImages, rows, cols);
        std::cout << "Images File Header:\n"
                  << "Magic Number: " << magic << "\n"
                  << "Number of Images: " << numImages << "\n"
                  << "Rows: " << rows << ", Columns: " << cols << "\n";



        auto images = loadMnistImages(imagePath, 10, rows, cols); // Load first 10 images
        // print number of non zero elements in images
        std::cout << "Number of non zero elements in images: " << images.nonZeros() << "\n";


        auto labels = loadMnistLabels(labelPath, 10);            // Load first 10 labels

        std::cout << "First Label: " << labels(0) << "\n";
        std::cout << "First Image:\n" ;

        displayImage(images.row(0).reshaped(rows, cols), rows, cols);


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
