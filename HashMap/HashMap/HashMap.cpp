// HashMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <chrono>
#include <random>

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

	std::vector<float> weights;
	for (int i = 0; i < 1076400; i++) {
		weights.push_back(RandomFloat(0.0, 256.0));
	}

	auto start = std::chrono::high_resolution_clock::now();
	std::vector<batch> batches;

	for (int i = 0; i < 32; i++) {
		batch batch_map;
		for (int j = 0; j < 1076400; j++) {
			batch_map[j] = weights.at(j);
		}
		batches.push_back(batch_map);
	}


	batch final_weights;
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 1076400; j++) {
			final_weights[i * 1076400 + j] = batches.at(i)[j];
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "Test Duration" << std::endl;
	std::cout << duration.count() << std::endl;
}