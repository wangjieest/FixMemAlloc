/*
FixMemAlloc - Fixed-size blocks allocation for C and C++

Copyright (c) 2016, Mariusz Moczala
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

* Neither the name of FixMemAlloc nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef MemoryPoolAllocatorH
#define MemoryPoolAllocatorH

#include <memory>
#include "MemoryPool.h"

template <class T>
class MemoryPoolAllocator : protected MemoryPool
{
    public:

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef T *pointer;
        typedef const T *const_pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef T value_type;

        template <class U>
        struct rebind
        {
            typedef MemoryPoolAllocator<U> other;
        };

        MemoryPoolAllocator(size_type numberOfElements) :
            numberOfElements(numberOfElements),
            memoryRegion(NULL)
        {
        }

        MemoryPoolAllocator(const MemoryPoolAllocator &allocator) :
            numberOfElements(allocator.getNumberOfElements()),
            memoryRegion(NULL)
        {
        }

        template <class U>
        MemoryPoolAllocator(const MemoryPoolAllocator<U> &other) :
            numberOfElements(other.getNumberOfElements()),
            memoryRegion(NULL)
        {
        }

        ~MemoryPoolAllocator()
        {
            delete[] memoryRegion;
            memoryRegion = NULL;
        }

        pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0)
        {
            if(!memoryRegion)
                initialize();

            void *p = allocateBlock(this);
            return static_cast<pointer>(p);
        }

        void deallocate(pointer p, size_type n)
        {
            releaseBlock(this, p);
        }

        void construct(pointer p, const_reference val)
        {
            new (p) T(val);
        }

        void destroy(pointer p)
        {
            p->~T();
        }

        size_type getNumberOfElements() const
        {
            return numberOfElements;
        }


    private:

        const size_type numberOfElements;
        pointer memoryRegion;

        void initialize()
        {
            memoryRegion = new T[numberOfElements];
            const std::size_t memoryRegionSize = sizeof(T) * numberOfElements;
            initializeMemoryPool(this, memoryRegion, memoryRegionSize, sizeof(T));
        }
};

#endif
