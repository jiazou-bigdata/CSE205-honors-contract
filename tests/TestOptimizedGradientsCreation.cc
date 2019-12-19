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
#include <vector>
#include "STLSlabAllocator.h"
#include "STLTlsfAllocator.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "OptimizedDelta.h"

using namespace std;




int main(int argc, char ** argv) {

   if(argc < 4) {
       std::cout << "[Usage] conf_file activation_rate num_iterations" << std::endl;
       std::cout << "@param conf_file path to the configuration file" << std::endl;
       std::cout << "@param activation_rate defines the relative size of the gradients compared to the total size of the model" << std::endl;
       std::cout << "@param num_iterations defines the number of iterations to repeat the test" << std::endl;
       return 1;
   }
   std::string filePath = argv[1];
   double activation_rate = atof(argv[2]);
   int numIterations = atoi(argv[3]);
   double totalAllocationTime = 0;
   double totalDeallocationTime = 0;
   for (int i = 0; i < numIterations; i++) {
       auto begin = std::chrono::high_resolution_clock::now();
       //initialize random seeds
       srand((unsigned) time(0));
       //opening configuration file
       ifstream inFile(filePath);
       //initialize the gradients for the whole network
       std::vector<std::shared_ptr<OptimizedDelta<float>>> * all_gradients = new std::vector<std::shared_ptr<OptimizedDelta<float>>>();


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
           std::shared_ptr<OptimizedDelta<float>> gradients = std::make_shared<OptimizedDelta<float>> ();
	   int numDimensions = dimensions.size();
	   int totalNumPossibleIndexes = 1;
	   for (int curDimension : dimensions) {
	       totalNumPossibleIndexes *= curDimension;
	   }
	   std::cout << "totalNumParameters at this layer: " << totalNumPossibleIndexes << std::endl;
	   int totalNumActivated = totalNumPossibleIndexes*activation_rate;
	   for (int indexOrder = 0; indexOrder < totalNumActivated; indexOrder++) {
                    gradients->add(indexOrder,0.01);
	   }
	   all_gradients->push_back(gradients);
        }

        auto creation_end = std::chrono::high_resolution_clock::now();
	totalAllocationTime += std::chrono::duration_cast<std::chrono::duration<float>>(creation_end - begin).count();

        //free the gradients
        for (auto cur_gradients : *all_gradients){
            cur_gradients = nullptr;
        }
        delete all_gradients;

	inFile.close();

        auto end = std::chrono::high_resolution_clock::now();
	totalDeallocationTime += std::chrono::duration_cast<std::chrono::duration<float>>(end - creation_end).count();

    }
    std::cout << "Allocation Duration: " << totalAllocationTime << " secs." << std::endl;
    std::cout << "Deallocation Duration: " << totalDeallocationTime << " secs." << std::endl;


}
