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
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "Tensor.h"

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
       std::vector<std::shared_ptr<Delta<float>>> * all_gradients = new std::vector<std::shared_ptr<Delta<float>>>();


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
           std::shared_ptr<Delta<float>> gradients = std::make_shared<Delta<float>> ();
	   int numDimensions = dimensions.size();
	   int totalNumPossibleIndexes = 1;
	   for (int curDimension : dimensions) {
	       totalNumPossibleIndexes *= curDimension;
	   }
	   std::cout << "totalNumParameters at this layer: " << totalNumPossibleIndexes << std::endl;
	   for (int indexOrder = 0; indexOrder < totalNumPossibleIndexes; indexOrder++) {
	       //first, let's sample it
               if ((double)(rand()) / (double)(RAND_MAX) < activation_rate) {
	            //OK, it is sampled
		    //Now let's create index from it;
		    std::vector<int> curIndex;
		    int total = indexOrder;
		    int factor = totalNumPossibleIndexes;
		    for (int curDimension : dimensions) {
			 //the index value at current pos
			 int curPosVal = total/(factor/curDimension);
		         curIndex.push_back(curPosVal);
			 factor = factor/curDimension;
			 total = total - curPosVal* factor;
		    }
		    //now we add the gradients, we simply assume all gradients have 0.01 in its value, as the value will not affect the allocation overhead
                    gradients->update(curIndex,0.01);
		    std::cout << "sampled index " << indexOrder << ": [ ";
		    for (int indexVal : curIndex) {
		        std::cout << indexVal << " ";
		    }
		    std::cout << " ]" << std::endl;
	       }
	   
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
