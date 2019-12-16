#include <vector>


//this class defines the TensorShape class
class TensorShape {

    protected:
       //dimensions of the shape
       std::vector<int> dimension;

    public:
       TensorShape(std::vector<int> dimension){
           this->dimension = dimension;
       }
       //to get the i-th dimension
       int getDimension(int i) {
	   if (i < dimension.size())
               return dimension[i];
	   else if (i == dimension.size()) 
	       return 1;
	   else 
	       return 0;
       }

       //to get the total size corresponding to the Shape
       int getLength() {
	   int ret = 1;
           for (int i : dimension) {
	       ret *= i;
	   }
	   return ret;
       }


       //to get the total number of dimensions
       int getNumDimensions() {
           return dimension.size();
       }

};
