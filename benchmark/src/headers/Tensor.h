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
      this->data = new std::vector<T>(shape->getLength());
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

    void setShape(std::vector<int> &dimensions) {
       this->shape = std::make_shared<TensorShape>(dimensions);
    }

    void setData(std::vector<T> *data) {
       this->data = data;
    }

    std::shared_ptr<TensorShape> * getShape() {
       return this->shape;
    }

    std::vector<T> * getData() {
       return this->data;
    }

    //get position in the array, given an index
    int getPos(std::vector<int> index) {
	int ret = 0;
	int numDimensions = shape->getNumDimensions();
	int factor = shape->getLength()/shape->getDimension(0);
        for(int i = 0; i < numDimensions; i++) {
	    ret += factor*index[i];
	    factor = factor/shape->getDimension(i+1);
	}
	return ret;
    }

    //update the value at the given index
    void update(std::vector<int> index, T newValue) {
        int pos = getPos(index);
	(*data)[pos] = newValue;
    }

    //update the value at the given position
    void update(int pos, T newValue) {
        (*data)[pos] = newValue;
    }

    std::vector<int> getIndex(int pos) {
        std::vector<int> ret;
	int numDimensions = shape->getNumDimensions();
	int factor = shape->getLength()/shape->getDimension(0);
	int curPos = pos;
	for (int i = 0; i < numDimensions; i++) {
	    int curIndex = curPos/factor;
	    ret.push_back(curIndex);
	    curPos = curPos - curIndex*factor;
	    factor = factor/shape->getDimension(i+1);
	}
        return ret;
    }


    //return the value at the given index
    T get(std::vector<int> index) {
        int pos = getPos(index);
	return (*data)[pos];
    }

    //return the value at the given position
    T get(int pos) {
        return (*data)[pos];
    }

    //assign a new data vector
    void assign(std::vector<T>* myData) {
	if (this->data != nullptr) 
             delete this->data;
        this->data = myData;
    }

    


    //get a subtensor at the k-th row of the tensor
    Tensor<T> getSubTensorByRow(int k) {
          Tensor<T> subTensor;
	  std::vector<int> subDimensions;
	  int numDimensions = shape->getNumDimensions();
	  for (int i = 1; i < numDimensions; i++) {
	      subDimensions.push_back(shape->getDimension(1));
	  }
	  subTensor.setShape(subDimensions);
	  std::vector<T> * subData = new std::vector<T>();
	  int factor = shape->getLength()/shape->getDimension(0);
	  for (int i = 0; i < factor; i++) {
	      subData->push_back((*data)[i]);
	  }
	  subTensor.setData(subData);
    }

    //merge delta to this tensor
    void merge(std::unordered_map<std::vector<int>, T>
		    delta) {
        for (std::pair<std::vector<int>, T> pair : delta) {
	     update(pair.first, pair.second);
	}
    }
};
