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

#ifndef GrowingMemoryPoolH
#define GrowingMemoryPoolH

#include "MemoryPool.h"

template <class DataType>
class GrowingMemoryPool : protected MemoryPool
{
    public:

        GrowingMemoryPool(std::size_t growByNumberOfBlocks) :
            growByNumberOfBlocks(growByNumberOfBlocks),
            firstMemoryRegion(NULL)
        {
            ::inlinedInitializeMemoryPool(this, NULL, 0, sizeof(DataType));
        }

        ~GrowingMemoryPool()
        {
            while(firstMemoryRegion) {
                MemoryRegion *memoryRegion = firstMemoryRegion;
                firstMemoryRegion = memoryRegion->nextMemoryRegion;

                free(memoryRegion->buffer);
                delete memoryRegion;
            }
        }

        DataType *allocateBlock()
        {
            void *pointer = ::inlinedAllocateBlock(this);

            if(!pointer) {
                allocateNewMemoryRegion();
                pointer = ::inlinedAllocateBlock(this);
            }

            DataType *data = static_cast<DataType *>(pointer);
            new (data) DataType;

            return data;
        }

        void releaseBlock(DataType *pointer)
        {
            pointer->~DataType();
            ::inlinedReleaseBlock(this, pointer);
        }


    private:

        struct MemoryRegion
        {
            MemoryRegion *nextMemoryRegion;
            void *buffer;
        };

        std::size_t growByNumberOfBlocks;
        MemoryRegion *firstMemoryRegion;

        void allocateNewMemoryRegion()
        {
            unsigned blockSize = sizeof(DataType);
            if(blockSize < MIN_MEMORY_POOL_BLOCK_SIZE)
                blockSize = MIN_MEMORY_POOL_BLOCK_SIZE;

            MemoryRegion *memoryRegion = new MemoryRegion;
            memoryRegion->nextMemoryRegion = firstMemoryRegion;
            memoryRegion->buffer = malloc(blockSize * growByNumberOfBlocks);

            ::initializeMemoryPool(this, memoryRegion->buffer, growByNumberOfBlocks, sizeof(DataType));
        }

        GrowingMemoryPool(const GrowingMemoryPool &growingMemoryPool);
        GrowingMemoryPool & operator =(const GrowingMemoryPool &growingMemoryPool);
};

#endif
