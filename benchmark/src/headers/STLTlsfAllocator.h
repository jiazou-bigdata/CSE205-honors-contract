/*
 * File: STLTlsfAllocator.h
 * Author: Jia
 * Nov 23, 2019, 10:57
 */

#ifndef STLTlsfALLOCATOR_H
#define STLTlsfALLOCATOR_H

#include <cstddef>
#include "tlsf.h"
#include <iostream>

/*
 * This class wraps a TLSF Allocator with STL-compatible interfaces.
 */

template <class T>
class STLTlsfAllocator {
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;



    //constructor that allocates a memory pool of the given size from the heap
    //@param size, the size of the memory pool
    STLTlsfAllocator(size_t size) {
	this->memPool = (void *)malloc(size * sizeof(char));
	this->memPoolSize = size;
        this->my_tlsf = this->allocator.tlsf_create_with_pool(this->memPool, this->memPoolSize);
    }

    //constructor that uses a memory pool passed in
    //@param memPool, the pointer to the memory pool passed in
    //@param size, the size of the memory pool passed in
    STLTlsfAllocator(void* memPool, size_t size) {
	this->memPool = memPool;
	this->memPoolSize = size;
        this->my_tlsf = this->allocator.tlsf_create_with_pool(this->memPool, this->memPoolSize);
    }

    //copy constructor
    template <class U>
    STLTlsfAllocator(const STLTlsfAllocator<U>& other) {
        this.my_tlsf = other.my_tlsf;
	this.memPool = other.memPool;
	this.memPoolSize = other.memPoolSize;
    }

    
    template <class U>
    struct rebind {
        typedef STLTlsfAllocator<U> other;
    };

    //to allocate a piece of memory of given size
    //@param n, size of memory to allocate
    pointer allocate(size_type n, const void* /*hint*/ = 0) {
        void* retPtr = allocator.tlsf_malloc(this->my_tlsf, n * sizeof(T));
        if (retPtr == nullptr) {
            std::bad_alloc exception;
            cout << "Can't allocate more from the page! TODO: add a new page!\n";
            throw exception;
        }
        return (pointer)retPtr;
    }

    //to deallocate a piece of memory of given size
    //@param p, pointer to the memory to deallocate
    //@param n, size of memory to deallocate
    void deallocate(pointer p, size_type n) {
        return allocator.tlsf_free(this->my_tlsf, p);
    }


    void construct(pointer p, const T& val) {
        new (p) T(val);
    }
    void destroy(pointer p) {
        p->~T();
    }
    tlsfAllocator getAllocator() const {
        return allocator;
    }

public:
    tlsfAllocator allocator;
    void* my_tlsf;
    void* memPool;
    size_t memPoolSize;
};

template <class T, class U>
bool operator==(const STLTlsfAllocator<T>& left, const STLTlsfAllocator<U>& right) {
    if ((left.my_tlsf == right.my_tlsf)&&(left.memPool == right.memPool)&&(left.memPoolSize == right.memPoolSize)) {
        return true;
    } else {
        return false;
    }
}
template <class T, class U>
bool operator!=(const STLTlsfAllocator<T>& left, const STLTlsfAllocator<U>& right) {
    if ((left.my_tlsf != right.my_tlsf)||(left.memPool != right.memPool)||(left.memPoolSize != right.memPoolSize)) {
        return true;
    } else {
        return false;
    }
}

#endif
