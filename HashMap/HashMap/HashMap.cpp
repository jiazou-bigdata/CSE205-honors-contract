// HashMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <chrono>
#include <random>
#include <fstream>
#include <string>

typedef std::unordered_map<int, float> HashMap;

//https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats/5289624
//Returns a random float in beteween two given floats.
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int main()
{
	long int deltaSize = 0;
	int numDeltas = 0;

	//Read deltaSize and numDeltas from config.txt
	std::string line;
	std::ifstream configFile ("config.txt");
	int lineNum = 0;
	if (configFile.is_open()) {
		while (std::getline(configFile, line)) {
			if (lineNum == 0) {
				deltaSize = std::stol(line.c_str());
			}
			else if (lineNum == 1) {
				numDeltas = std::atoi(line.c_str());
			}
			lineNum++;
		}
	}
	
	std::cout << "Delta Size " << std::to_string(deltaSize) << std::endl;
	std::cout << "Num Deltas " << std::to_string(numDeltas) << std::endl;
	std::cout << "Running..." << std::endl;

	//Pre-populates random weight values
	std::vector<float> weights;
	for (int i = 0; i < deltaSize; i++) {
		weights.push_back(RandomFloat(0.0, 255.0));
	}

	auto start = std::chrono::high_resolution_clock::now();

	//Allocates smart pointers and values
	std::shared_ptr<std::vector<std::shared_ptr<HashMap>>> deltas_ptr = std::make_shared<std::vector<std::shared_ptr<HashMap>>>();
	std::shared_ptr<HashMap> final_batch_ptr = std::make_shared<HashMap>();
	
	std::vector<std::shared_ptr<HashMap>> deltas = *deltas_ptr;
	HashMap final_batch = *final_batch_ptr;

	//Populates deltas vector with mini-deltas of weights
	for (int i = 0; i < numDeltas; i++) {
		HashMap delta;
		for (int j = 0; j < deltaSize; j++) {
			delta[j] = weights.at(j);
		}

		deltas.push_back(std::make_shared<HashMap>(delta));
	}
	
	//"unzips" mini-deltas into large delta of weights
	for (int i = 0; i < numDeltas; i++) {
		for (int j = 0; j < deltaSize; j++) {
			final_batch[i * deltaSize + j] = (*deltas.at(i)).at(j);
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "Test Duration: " << duration.count() << " microseconds" << std::endl;

}