#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;

// Shape class contains dimensions of a layer
class Shape {
    protected:
    vector <int> dimension;
    public:
        Shape()= default;;
        explicit Shape(vector<int> array) : dimension(std::move(array)) {};
        void push_back(const int &val) { dimension.push_back(val); }
        int getDimensions() {
            int total = 1;
            for (int i : dimension) total*=i;
            return total;
        }
};

// Tensor class holds elements of specified type in the dimensions specified by Shape object
template<class TypeContained>
class Tensor {
    private:
        vector<TypeContained> myArray;
        Shape myShape;
    public:
        explicit Tensor(Shape shape) : myShape(shape) {}
        void push_back(const TypeContained &val) { myArray.push_back(val); }
        void push_back(const vector<TypeContained> &array) { myArray = array; }
        std::vector<TypeContained> getArray() { return myArray; }
        int getDimensions() { return myShape.getDimensions(); }
};


int main() {

    // Initialize CNN configuration file input
    cout << "Reading file..." << endl;
    ifstream inFile ("..\\config");

    // Check file I/O
    if (!inFile) {
        cerr << "Unable to open config file";
        exit(1);   // stop program
    }

    // Vector:CNN, Tensor:Layer, Shape:Layer Dimension
    std::shared_ptr<vector<Tensor<float>>>  neuralNet = std::make_shared<vector<Tensor<float>>>();

    // Initialize layers
    cout << "Generating CNN..." << endl;
    // Read each layer as a line
    string fileLine;
    while(getline(inFile, fileLine)) {
        cout << fileLine << endl;

        // Convert input string to string stream
        stringstream ss;
        ss << fileLine;

        // Extract all numbers from line
        // Store dimensions in Shape object
        Shape shape;
        int num;
        while(!ss.eof()){
            ss >> num;
            shape.push_back(num);
        }

        // Add layer to CNN
        neuralNet->push_back(Tensor<float>(shape));
    }

    // Fill layers with random data
    for (Tensor<float> i : *neuralNet){
        // Get shape of layer
        int dataCount = i.getDimensions();
        // Generate random data to fill shape
        vector <float> randData (dataCount);
        for( float j : randData) randData.at(j) = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        // Push random data to layer
        i.push_back(randData);
    }



}// END OF PROGRAM


