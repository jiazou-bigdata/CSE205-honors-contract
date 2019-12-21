//The Delta class describes a set of gradients that we use for updating the model
#pragma once
#include <unordered_map>


template<typename Type>
class Delta {

private:
    //the gradients data is stored in a hashmap
    std::unordered_map<int, Type> * data = nullptr;

public:
    //constructor
    Delta() {
        this->data = new std::unordered_map<int, Type>();
    }

    //destructor
    ~Delta() {
       this->data->clear();
       delete this->data;
       this->data = nullptr;
    }
    
    //add a new gradient 
    void update(int index, Type newValue) {
       (*data)[index] = newValue;
    }

    //get the gradient data
    std::unordered_map<int, Type>* getData() {
        return data;
    }
};
