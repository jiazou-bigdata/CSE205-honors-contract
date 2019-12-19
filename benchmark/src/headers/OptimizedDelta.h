//The OptimizedDelta class describes a set of gradients that we use for updating the model, and this class uses vector instead of hashmap to store the gradients.

#include <vector>


template<typename Type>
class OptimizedDelta {

private:
    //the gradients data is stored in a hashmap
    std::vector<std::pair<int, Type>> * data = nullptr;

public:
    //constructor
    OptimizedDelta() {
        this->data = new std::vector<std::pair<int, Type>>();
    }

    //destructor
    ~OptimizedDelta() {
       this->data->clear();
       delete this->data;
       this->data = nullptr;
    }
    
    //add a new gradient 
    void add(int index, Type newValue) {
       std::pair<int, Type> curPair;
       curPair.first = index;
       curPair.second = newValue;
       data->push_back(curPair);
    }

    //get the gradient data
    std::vector<std::pair<int, Type>>* getData() {
        return data;
    }
};
