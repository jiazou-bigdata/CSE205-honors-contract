//The SparseTensor class describes a sparse matrix with a specified shape

#include "TensorShape.h" 
#include "Tensor.h"

template<typename T>
class SparseTensor {

private:
    //the shape of the sparse tensor
    TensorShape shape;

    //the index of the sparse tensor
    Tensor index;

    //the data of the sparse tensor 
    Tensor val;

public:

    //constructor
    //@param dimensions dimensions of the tensor
    //@param data values of the tensor
    //@param indexDimensions dimensions of the index, the number of rows is N: the number of non-zero elements; the number of columns is numDims: the number of dimensions of the tensor.
    //@param indexData values of the index

    SparseTensor(std::vector<int> &dimension, std::vector<T> *data, std::vector<int> &indexDimensions, std::vector<int> *indexData) {
      shape.setDimension(dimension);
      index.setShape(indexDimension);
      index.setData(indexData);
      val.setShape(dimension);
      val.setData(data);
    }


    //destructor
    ~SparseTensor() {
    }


    TensorShape & getShape() {
        return this.shape;
    
    }

    Tensor<int> & getIndex() {
        return this.index;
    }

    Tensor<T> & getData() {
        return this.val;
    }


    std::shared_ptr<Tensor<T>> convertToDenseTensor() {
       std::shared_ptr<Tensor<T>> dense = std::make_shared<Tensor<T>>(val.getShape()->getDimensions());
       std::shared_ptr<TensorShape> indexShape = index.getShape();
       int numRows = indexShape->getDimension(0);
       std::vector<T> * valData = val.getData();
       for (int i = 0; i < numRows; i++) {
           Tensor curRow = index.getSubTensorByRow(i);
	   std::vector<int> * curIndex = curRow.getData();
           dense->update(*curIndex, (*valData)[i]);	   
       }
       return dense;
    }

    std::shared_ptr<SparseTensor<T>> convertFromDenseTensor(std::shared_ptr<Tensor<T>> denseTensor){
       if(denseTensor == nullptr) 
	   return nullptr;
       std::vector<int> myDimension = denseTensor->getShape()->getDimensions();
       std::vector<T> * myData = new std::vector<T>();
       std::vector<int> * myIndexData = new std::vector<int>();
       int numElements = denseTensor->getLength();
       for (int i = 0; i < numElements; i++) {
           int curValue = denseTensor->get(i);
	   if ((curValue > 0.000001) || (curValue < 0.000001)){
	       myData->push_back(curValue);
	       std::vector<int> curIndexOfValue = denseTensor->getIndex(i);
	       for (int j = 0; j < curIndexOfValue.size(); j++) {
	       myIndexData->push_back(curIndexOfValue[j]);
	   }
       }
       std::vector<int> myIndexDimension;
       myIndexDimension.push_back(myData->size());
       myIndexDimension.push_back(myDimension.size());
       std::shared_ptr<SparseTensor<T>> ret = std::make_shared<SparseTensor<T>>(myDimension, myData, myIndexDimension, myIndexData);
       return ret;
    } 
};
