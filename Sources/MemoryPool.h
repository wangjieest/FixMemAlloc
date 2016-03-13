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

#ifndef MemoryPoolH
#define MemoryPoolH

#include <stddef.h>
#include <stdint.h>

#include "Inline.h"

#define MIN_MEMORY_POOL_BLOCK_SIZE sizeof(void *)

struct MemoryPool
{
    size_t blockSize;
    size_t numberOfNotYetUsedBlocks;
    void *notYetUsedBlocks;
    void *firstFreeBlock;
};

INLINE void inlinedInitializeMemoryPool(struct MemoryPool *memoryPool,
    void *memoryRegion, size_t numberOfBlocks, size_t blockSize)
{
    if(blockSize < MIN_MEMORY_POOL_BLOCK_SIZE)
        numberOfBlocks = 0;

    memoryPool->blockSize = blockSize;
    memoryPool->numberOfNotYetUsedBlocks = numberOfBlocks;
    memoryPool->notYetUsedBlocks = memoryRegion;
    memoryPool->firstFreeBlock = NULL;
}

INLINE void *inlinedAllocateBlock(struct MemoryPool *memoryPool)
{
    void *pointer;

    pointer = memoryPool->firstFreeBlock;
    if(pointer) {
        memoryPool->firstFreeBlock = *(void **) pointer;
        return pointer;
    }

    if(memoryPool->numberOfNotYetUsedBlocks) {
        pointer = memoryPool->notYetUsedBlocks;
        memoryPool->notYetUsedBlocks = ((uint8_t *) pointer) + memoryPool->blockSize;
        memoryPool->numberOfNotYetUsedBlocks--;
    }

    return pointer;
}

INLINE void inlinedReleaseBlock(struct MemoryPool *memoryPool, void *pointer)
{
    *(void **) pointer = memoryPool->firstFreeBlock;
    memoryPool->firstFreeBlock = pointer;
}

#ifdef __cplusplus
    extern "C" {
#endif

    void initializeMemoryPool(struct MemoryPool *memoryPool,
        void *memoryRegion, size_t numberOfBlocks, size_t blockSize);

    void *allocateBlock(struct MemoryPool *memoryPool);
    void releaseBlock(struct MemoryPool *memoryPool, void *pointer);

#ifdef __cplusplus
    }
#endif

#endif
