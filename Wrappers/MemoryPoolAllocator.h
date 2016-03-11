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

        const size_type growByNumberOfBlocks;

        MemoryPoolAllocator(size_type growByNumberOfBlocks = 1024) :
            growByNumberOfBlocks(growByNumberOfBlocks),
            firstMemoryRegion(NULL)
        {
            ::initializeMemoryPool(this, NULL, 0, sizeof(T));
        }

        MemoryPoolAllocator(const MemoryPoolAllocator &allocator) :
            growByNumberOfBlocks(allocator.growByNumberOfBlocks),
            firstMemoryRegion(NULL)
        {
            ::initializeMemoryPool(this, NULL, 0, sizeof(T));
        }

        template <class U>
        MemoryPoolAllocator(const MemoryPoolAllocator<U> &other) :
            growByNumberOfBlocks(other.growByNumberOfBlocks),
            firstMemoryRegion(NULL)
        {
            ::initializeMemoryPool(this, NULL, 0, sizeof(T));
        }

        ~MemoryPoolAllocator()
        {
            while(firstMemoryRegion) {
                MemoryRegion *memoryRegion = firstMemoryRegion;
                firstMemoryRegion = memoryRegion->nextMemoryRegion;

                free(memoryRegion->buffer);
                delete memoryRegion;
            }
        }

        pointer allocate(size_type n, std::allocator<void>::const_pointer hint = 0)
        {
            if(n != 1 || hint)
                throw std::bad_alloc();

            pointer p = allocateBlock();
            if(!p)
                throw std::bad_alloc();

            return p;
        }

        void deallocate(pointer p, size_type n)
        {
            ::inlinedReleaseBlock(this, p);
        }

        void construct(pointer p, const_reference val)
        {
            new (p) T(val);
        }

        void destroy(pointer p)
        {
            p->~T();
        }


    private:

        struct MemoryRegion
        {
            MemoryRegion *nextMemoryRegion;
            void *buffer;
        };

        MemoryRegion *firstMemoryRegion;

        void allocateNewMemoryRegion()
        {
            unsigned blockSize = sizeof(T);
            if(blockSize < MIN_MEMORY_POOL_BLOCK_SIZE)
                blockSize = MIN_MEMORY_POOL_BLOCK_SIZE;

            MemoryRegion *memoryRegion = new MemoryRegion;
            memoryRegion->nextMemoryRegion = firstMemoryRegion;
            memoryRegion->buffer = malloc(blockSize * growByNumberOfBlocks);

            ::initializeMemoryPool(this, memoryRegion->buffer, growByNumberOfBlocks, sizeof(T));
        }

        pointer allocateBlock()
        {
            void *data = ::inlinedAllocateBlock(this);

            if(!data) {
                allocateNewMemoryRegion();
                data = ::inlinedAllocateBlock(this);
            }

            return static_cast<pointer>(data);
        }
};

#endif
