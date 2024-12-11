// #pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h> 
#include "Eigen/Dense"
#include "IO.hpp"

int main(int argc, char *argv[]) { 
    if (argc==5) {
        const std::string inputPath = argv[1];
        const std::string outputPath = argv[2];
        const int index =  std::stoi(argv[3]);
        const char ImageOrLabel = argv[4][0];


        try { 
    
            int magic, numEntries, rows, cols;
            readMnistHeader(inputPath, magic, numEntries,rows,cols,ImageOrLabel);
            std::cout << ImageOrLabel<< " File Header:\n"
                    << "Magic Number: " << magic << "\n"
                    << "Number of entries: " << numEntries << "\n";
            if (ImageOrLabel == 'I') {
            std::cout << "Rows: " << rows << ", Columns: " << cols << "\n";
                }
    
            if (ImageOrLabel == 'I') {
                auto Matrix = loadMnistImages(inputPath, index+1, rows, cols); // Load a couple of pictures after the required index
                if (Matrix.nonZeros()==0) std::cout << "Load unsuccessful " << "\n";
                }
            else if (ImageOrLabel == 'L') {
                auto Matrix = loadMnistLabels(inputPath, index+1);
                if (Matrix.nonZeros()==0) std::cout << "Load unsuccessful " << "\n";
                }
         
            // write tensor should be called here  (function still not implemented)

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            }



    return 0;
    }
    else if (argc==1) {
        try {
        const std::string imagePath = "./dataset/t10k-images.idx3-ubyte";
        const std::string labelPath = "./dataset/t10k-labels.idx1-ubyte";

        int labelMagic, numLabels,nc,nr;
        readMnistHeader(labelPath, labelMagic, numLabels,nr,nc,'L');
        std::cout << "Labels File Header:\n"
                  << "Magic Number: " << labelMagic << "\n"
                  << "Number of Labels: " << numLabels << "\n";

        
        int magic, numImages, rows, cols;
        readMnistHeader(imagePath, magic, numImages, rows, cols,'I');
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
    return 1; 
    }
    else { 
        std::cout<< "incorrect number of arguments"<<std::endl;
        return -1;
    }


}
