//by Jia

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctime>
#include <chrono>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <unordered_map>
#include "STLSlabAllocator.h"
#include "STLTlsfAllocator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "Tensor.h"

using namespace std;




int main(int argc, char ** argv) {

   if(argc < 2) {
       std::cout << "[Usage] conf_file" << std::endl;
       std::cout << "@param conf_file that defines the network architecture" << std::endl;
       return 1;
   }
   std::string filePath = argv[1];

   int numIterations = 1000000;
   double totalAllocationTime = 0;
   double totalDeallocationTime = 0;
   for (int i = 0; i < numIterations; i++) {
       auto begin = std::chrono::high_resolution_clock::now();
       ifstream inFile(filePath);
       //initialize the neuralNet
       std::vector<std::shared_ptr<Tensor<float>>> * neuralNet = new std::vector<std::shared_ptr<Tensor<float>>>();


       if (!inFile) {
           std::cerr << "Error: file does not exist: " << filePath << std::endl;
           exit(1);   
       }   



       std::string line="";
       //create the neuralNet based on specification
       while (std::getline (inFile, line)) {
           stringstream ss;
           ss << line;
           std::vector<int> dimensions;
           int num;
           while (!ss.eof()) {
               ss >> num;
	       dimensions.push_back(num);
           }
           std::shared_ptr<Tensor<float>> layer = std::make_shared<Tensor<float>> (dimensions);
           neuralNet->push_back(layer);
        }

        auto creation_end = std::chrono::high_resolution_clock::now();
	totalAllocationTime += std::chrono::duration_cast<std::chrono::duration<float>>(creation_end - begin).count();

        //free the neuralNet
        for (auto layer : *neuralNet){
            layer = nullptr;
        }
        delete neuralNet;

	inFile.close();

        auto end = std::chrono::high_resolution_clock::now();
	totalDeallocationTime += std::chrono::duration_cast<std::chrono::duration<float>>(end - creation_end).count();

    }
    std::cout << "Allocation Duration: " << totalAllocationTime << " secs." << std::endl;
    std::cout << "Deallocation Duration: " << totalDeallocationTime << " secs." << std::endl;


}
