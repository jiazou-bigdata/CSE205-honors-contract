//The Tensor class describes an arbitrary array with a specified shape

#include "TensorShape.h" 
#include "Delta.h"
#include <vector>

template<typename T>
class Tensor {

private:
    //the shape of the tensor
    std::shared_ptr<TensorShape> shape = nullptr;

    //the data of the tensor 
    std::vector<T> * data = nullptr;

public:

    //constructor
    //@param dimensions dimensions of the tensor

    Tensor(std::vector<int> &dimensions) {
      this->shape = std::make_shared<TensorShape>(dimensions);
      this->data = new std::vector<T>(dimensions.size());
    }

    //constructor
    //@param dimensions dimensions of the tensor
    //@param data vector to initialize the data
    Tensor(std::vector<int> &dimensions, std::vector<T> &data) {
      this->shape = std::make_shared<TensorShape>(dimensions);
      this->data = new std::vector<T>(data);
    }

    //constructor
    //@param dimensions dimensions of the tensor
    //@param data pointer to the vector for initializing data
    Tensor(std::vector<int> &dimensions, std::vector<T> *data) {
      this->shape = std::make_shared<TensorShape>(dimensions);
      this->data = data;
    }

    //destructor
    ~Tensor() {
       this->shape = nullptr;
       delete this->data;
       this->data = nullptr;
    }

    //get position in the array, given an index
    int getPos(std::vector<int> index) {
	int ret = 0;
	int numDimensions = shape->getNumDimensions();
        for(int i = 0; i < numDimensions; i++) {
	    ret += shape->getDimension(i+1)*index[i];
	}
	return ret;
    }

    //update the value at the given index
    void update(std::vector<int> index, T newValue) {
        int pos = getPos(index);
	(*data)[pos] = newValue;
    }

    //return the value at the given index
    T get(std::vector<int> index) {
        int pos = getPos(index);
	return (*data)[pos];
    }

    //assign a new data vector
    void assign(std::vector<T>* myData) {
	if (this->data != nullptr) 
             delete this->data;
        this->data = myData;
    }

    //merge delta to this tensor
    void merge(std::unordered_map<std::vector<int>, T, IndexHash, IndexEqual>
		    delta) {
        for (std::pair<std::vector<int>, T> pair : delta) {
	     update(pair.first, pair.second);
	}
    }
};
