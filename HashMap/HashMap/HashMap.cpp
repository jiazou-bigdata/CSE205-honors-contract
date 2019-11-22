// HashMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <chrono>
#include <random>
#include <fstream>
#include <string>

typedef std::unordered_map<int, float> batch;

//https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats/5289624
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int main()
{
	long int numWeights = 0;
	int numBatches = 0;

	std::string line;
	std::ifstream configFile ("config.txt");
	int lineNum = 0;
	if (configFile.is_open()) {
		while (std::getline(configFile, line)) {
			if (lineNum == 0) {
				numWeights = std::stol(line.c_str());
			}
			else if (lineNum == 1) {
				numBatches = std::atoi(line.c_str());
			}
			lineNum++;
		}
	}

	std::vector<float> weights;
	for (int i = 0; i < numWeights; i++) {
		weights.push_back(RandomFloat(0.0, 255.0));
	}

	auto start = std::chrono::high_resolution_clock::now();
	std::vector<batch> deltas;

	for (int i = 0; i < numBatches; i++) {
		batch delta;
		for (int j = 0; j < numWeights; j++) {
			delta[j] = weights.at(j);
		}
		deltas.push_back(delta);
	}


	batch final_batch;
	for (int i = 0; i < numBatches; i++) {
		for (int j = 0; j < numWeights; j++) {
			final_batch[i * numWeights + j] = deltas.at(i)[j];
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "Test Duration" << std::endl;
	std::cout << duration.count() << std::endl;
}