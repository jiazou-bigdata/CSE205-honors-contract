//by Jia

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctime>
#include <chrono>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <unordered_map>
#include "STLSlabAllocator.h"
#include "STLTlsfAllocator.h"
#include <iostream>
using namespace std;

//a function to transform integer to strings
char * new_string_from_integer(int num) {
   int ndigits = num == 0 ? 1 : (int)log10(num) + 1;
   char * str = (char *) malloc (ndigits + 1);
   sprintf(str, "%d", num);
   return str;
}

int main(int argc, char ** argv) {

   if(argc <= 3) {
       std::cout << "[Usage] numMillionsOfKeys 0/1/2 randomstring/sequentialstring" << std::endl;
       std::cout << "@param numMillionsOfKeys: the number of millions of keys to insert to the map. For example if numMillionsOfKeys=8, 8 millions of keys will be inserted into the map" << std::endl;
       std::cout << "@param 0/1/2: which mode to test. 0 represents to use the STL default allocator; 1 represents to use the STLTlsfAllocator; 2 represents to use the STLSlabAllocator" << std::endl;
       std::cout << "@param randomstring/sequentialstring: whether to insert sequentially incremented strings, or to insert randomly generated strings" << std::endl;
       return 1;
   }
   int numKeysx1000000 = atoi(argv[1]);
   int i,j, value = 0;

   auto begin = std::chrono::high_resolution_clock::now();
   //to use the STL default allocator
   if(atoi(argv[2])==0) {
       cout << "unordered_map using std::Allocator\n";
       std::unordered_map<const char *, int64_t>* str_hash=new std::unordered_map<const char *, int64_t>();
       if(!strcmp(argv[3], "sequentialstring")) {
           for(i = 0; i< numKeysx1000000; i++) {
               for (j = 0; j < 1000000; j++ ) {
                   str_hash->insert(std::unordered_map<const char *, int64_t>::value_type(new_string_from_integer(i*1000000+j), value));
               }
           }
       }
       else if(!strcmp(argv[3], "randomstring")){
           srandom(1);
           for(i = 0; i< numKeysx1000000; i++) {
               for (j = 0; j < 1000000; j++ ) {
                   str_hash->insert(std::unordered_map<const char *, int64_t>::value_type(new_string_from_integer((int)random()), value));
               }
           }
       }
   }
   //to use the STLTlsfAllocator
   else if(atoi(argv[2])==1){

       cout << "unordered_map using STLTlsfAllocator\n";
       char * data = (char *) malloc (64*1024*1024*sizeof(char));
       STLTlsfAllocator<std::pair<const char *, int64_t>> allocator = STLTlsfAllocator<std::pair<const char *, int64_t>>((void *)data, 64*1024*1024);
       hash<const char*> hasher = hash<const char*>();
       equal_to<const char*> key_equal=equal_to<const char*>();
       std::unordered_map<const char*, int64_t, hash<const char*>, equal_to<const char*>,STLTlsfAllocator<std::pair<const char *, int64_t>>>* str_hash = new std::unordered_map<const char*, int64_t, hash<const char*>, equal_to<const char*>,STLTlsfAllocator<std::pair<const char *, int64_t>>> (10, hasher, key_equal, allocator);
       if(!strcmp(argv[3], "sequentialstring")) {
           for(i = 0; i< numKeysx1000000; i++) {
               for (j = 0; j < 10; j++ ) {
                  try{
                   str_hash->insert(std::pair<const char*, int64_t>(new_string_from_integer(i*10+j), value));
                  }
                  catch(std::bad_alloc &e) {
                       cout <<"Caught the exception! Should add a new page, but simply exit now...\n";
                       exit(-1);
                  }
               }
           }
       }
       else if(!strcmp(argv[3], "randomstring")){
           srandom(1);
           for(i = 0; i< numKeysx1000000; i++) {
               for (j = 0; j < 10; j++ ) {
                   str_hash->insert(std::pair<const char*, int64_t>(new_string_from_integer((int)random()), value));
               }
           }
       }

   } else {
       cout << "unordered_map using SlabAllocator\n";
       STLSlabAllocator<std::pair<const char *, int64_t>> allocator = STLSlabAllocator<std::pair<const char *, int64_t>>(64*1024*1024);
       hash<const char*> hasher = hash<const char*>();
       equal_to<const char*> key_equal=equal_to<const char*>();
       std::unordered_map<const char*, int64_t, hash<const char*>, equal_to<const char*>,STLSlabAllocator<std::pair<const char *, int64_t>>>* str_hash = new std::unordered_map<const char*, int64_t, hash<const char*>, equal_to<const char*>,STLSlabAllocator<std::pair<const char *, int64_t>>> (10, hasher, key_equal, allocator);
       if(!strcmp(argv[3], "sequentialstring")) {
           for(i = 0; i< numKeysx1000000; i++) {
               for (j = 0; j < 10; j++ ) {
                  try{
                   str_hash->insert(std::pair<const char*, int64_t>(new_string_from_integer(i*10+j), value));
                  }
                  catch(std::bad_alloc &e) {
                       cout <<"Caught the exception! Iterate the hashmap and write items to disk.\n";
                       //iterate items and write items to disk
                       //create a buffer
                       exit(-1);
                       
                  }
               }
           }
       }
       else if(!strcmp(argv[3], "randomstring")){
           srandom(1);
           for(i = 0; i< numKeysx1000000; i++) {
               for (j = 0; j < 10; j++ ) {
                   str_hash->insert(std::pair<const char*, int64_t>(new_string_from_integer((int)random()), value));
               }
           }
       }

   }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "End-to-End Time Duration: "
              << std::chrono::duration_cast<std::chrono::duration<float>>(end - begin).count()
              << " secs." << std::endl;

}
