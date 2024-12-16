#include <fstream>
#include <iostream>
#include <filesystem>
#include "IO.hpp"

using namespace IO_MNIST;

int main(const int argc, char *argv[]) {
    if (argc == 5) {
        const std::string inputPath = argv[1];
        const std::string outputPath = argv[2];
        const int index = std::stoi(argv[3]);
        const char ImageOrLabel = argv[4][0];

        try {
            int magic, numEntries, rows, cols;
            readMnistHeader(inputPath, magic, numEntries, rows, cols, ImageOrLabel);
            std::cout << ImageOrLabel << " File Header:\n"
                    << "Magic Number: " << magic << "\n"
                    << "Number of entries: " << numEntries << "\n";
            if (ImageOrLabel == 'I') {
                std::cout << "Rows: " << rows << ", Columns: " << cols << "\n";
            }
            else {rows = 1; cols=10;}

            if (ImageOrLabel == 'I') {
                // Load a couple of pictures after the required index
                const auto dataset = loadMnistImages(inputPath, index + 1, rows, cols);
                writeTensorToFile(dataset.row(index).reshaped(rows,cols),outputPath);

            } else if (ImageOrLabel == 'L') {
                const auto dataset = loadMnistLabels(inputPath, index + 1);
                writeTensorToFile(dataset.row(index).reshaped(rows,cols),outputPath);

            }
            else {
                std::cout<< "Input Error:  Expected I for image or L for labels"<<std::endl;
                return 0;
            }


        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        return 0;
    }
    if (argc == 1) {
        try {
            // print current working directory

            std::cout << "Current working directory: " << std::filesystem::current_path() << "\n";

            const bool is_in_build = std::filesystem::current_path().string().find("cmake-build-debug") !=
                                     std::string::npos;

            const std::string imagePath = is_in_build
                                              ? "./../dataset/train-images.idx3-ubyte"
                                              : "./dataset/train-images.idx3-ubyte";
            const std::string labelPath = is_in_build
                                              ? "./../dataset/train-labels.idx1-ubyte"
                                              : "./dataset/train-labels.idx1-ubyte";

            int labelMagic, numLabels, nc, nr;
            readMnistHeader(labelPath, labelMagic, numLabels, nr, nc, 'L');
            std::cout << "Labels File Header:\n"
                    << "Magic Number: " << labelMagic << "\n"
                    << "Number of Labels: " << numLabels << "\n";


            int magic, numImages, rows, cols;
            readMnistHeader(imagePath, magic, numImages, rows, cols, 'I');
            std::cout << "Images File Header:\n"
                    << "Magic Number: " << magic << "\n"
                    << "Number of Images: " << numImages << "\n"
                    << "Rows: " << rows << ", Columns: " << cols << "\n";


            auto images = loadMnistImages(imagePath, 10, rows, cols); // Load first 10 images
            // print number of non zero elements in images
            std::cout << "Number of non zero elements in images: " << images.nonZeros() << "\n";


            auto labels = loadMnistLabels(labelPath, 10); // Load first 10 labels

            int index = 0;
            labels.row(0).maxCoeff(&index);
            std::cout << "First Label: " << index << "\n";
            std::cout << "First Image:\n";

            displayImage(images.row(0).reshaped(rows, cols), rows, cols);
            writeTensorToFile (images.row(0).reshaped(rows, cols),"image");
            writeTensorToFile (labels.row(0),"labels");

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        return 1;
    } else {
        std::cout << "incorrect number of arguments" << std::endl;
        return -1;
    }
}
