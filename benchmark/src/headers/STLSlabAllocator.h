/*
 * File: STLSlabAllocator.h
 * Author: Jia
 * May 20, 2016, 10:57
 */

#ifndef STLSLABALLOCATOR_H
#define STLSLABALLOCATOR_H

#include <cstddef>
#include "SlabAllocator.h"
#include <iostream>

/*
 * This class wraps a Memcached SlabAllocator with STL-compatible interfaces.
 */

template <class T>
class STLSlabAllocator {
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
    STLSlabAllocator(size_t size) {
        allocator = make_shared<SlabAllocator>(size, true);
    }

    //constructor that uses a memory pool passed in
    //@param memPool, the pointer to the memory pool passed in
    //@param size, the size of the memory pool passed in
    STLSlabAllocator(void* memPool, size_t size) {
        allocator = make_shared<SlabAllocator>(memPool, size, true);
    }

    //copy constructor
    template <class U>
    STLSlabAllocator(const STLSlabAllocator<U>& other) {
        allocator = other.getAllocator();
    }

    
    template <class U>
    struct rebind {
        typedef STLSlabAllocator<U> other;
    };

    //to allocate a piece of memory of given size
    //@param n, size of memory to allocate
    pointer allocate(size_type n, const void* /*hint*/ = 0) {
        void* retPtr = allocator->do_slabs_alloc(n * sizeof(T));
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
        return allocator->do_slabs_free(p, n * sizeof(T));
    }


    void construct(pointer p, const T& val) {
        new (p) T(val);
    }
    void destroy(pointer p) {
        p->~T();
    }
    SlabAllocatorPtr getAllocator() const {
        return allocator;
    }

private:
    SlabAllocatorPtr allocator;
};

template <class T, class U>
bool operator==(const STLSlabAllocator<T>& left, const STLSlabAllocator<U>& right) {
    if (left.getAllocator() == right.getAllocator()) {
        return true;
    } else {
        return false;
    }
}
template <class T, class U>
bool operator!=(const STLSlabAllocator<T>& left, const STLSlabAllocator<U>& right) {
    if (left.getAllocator() != right.getAllocator()) {
        return true;
    } else {
        return false;
    }
}

#endif
