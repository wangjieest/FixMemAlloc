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

#include "MemoryPool.h"
#include "gtest.h"

TEST(MemoryPool, EmptyMemoryRegion)
{
    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, NULL, 0, 1024);
    
    void *ptr1 = allocateBlock(&memoryPool);
    void *ptr2 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr1 == NULL);
    EXPECT_TRUE(ptr2 == NULL);
}

TEST(MemoryPool, SingleElement)
{
    uint64_t buffer;

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, &buffer, 1, sizeof(buffer));

    void *ptr1 = allocateBlock(&memoryPool);
    void *ptr2 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr1 == &buffer);
    EXPECT_TRUE(ptr2 == NULL);
}

TEST(MemoryPool, SmallElement)
{
    uint8_t buffer;

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, &buffer, 1, sizeof(buffer));

    void *ptr = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr == NULL);
}

TEST(MemoryPool, FittingElement)
{
    uint8_t buffer[MIN_MEMORY_POOL_BLOCK_SIZE];

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, &buffer, 1, sizeof(buffer));

    void *ptr1 = allocateBlock(&memoryPool);
    void *ptr2 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr1 == &buffer);
    EXPECT_TRUE(ptr2 == NULL);
}

TEST(MemoryPool, SimpleAllocScheme)
{
    const size_t blockSize = 23;
    const size_t numberOfBlocks = 3;
    const size_t memoryRegionSize = numberOfBlocks * blockSize;
    uint8_t memoryRegion[memoryRegionSize];

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, memoryRegion, numberOfBlocks, blockSize);

    void *ptr1 = allocateBlock(&memoryPool);
    void *ptr2 = allocateBlock(&memoryPool);
    void *ptr3 = allocateBlock(&memoryPool);
    void *ptr4 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr1 != NULL);
    EXPECT_TRUE(ptr2 != NULL);
    EXPECT_TRUE(ptr3 != NULL);
    EXPECT_TRUE(ptr4 == NULL);

    releaseBlock(&memoryPool, ptr2);

    void *ptr5 = allocateBlock(&memoryPool);
    void *ptr6 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr5 != NULL);
    EXPECT_TRUE(ptr6 == NULL);
}

TEST(MemoryPool, MultipleRegions)
{
    uint64_t region1;
    uint64_t region2;

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, &region1, 1, sizeof(region1));

    releaseBlock(&memoryPool, &region2);

    void *ptr1 = allocateBlock(&memoryPool);
    void *ptr2 = allocateBlock(&memoryPool);
    void *ptr3 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr1 != NULL);
    EXPECT_TRUE(ptr2 != NULL);
    EXPECT_TRUE(ptr3 == NULL);
}

TEST(MemoryPool, RegionAccumulation)
{
    uint64_t region1;
    uint64_t region2;

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, NULL, 0, sizeof(region1));
    
    releaseBlock(&memoryPool, &region1);
    releaseBlock(&memoryPool, &region2);

    void *ptr1 = allocateBlock(&memoryPool);
    void *ptr2 = allocateBlock(&memoryPool);
    void *ptr3 = allocateBlock(&memoryPool);

    EXPECT_TRUE(ptr1 != NULL);
    EXPECT_TRUE(ptr2 != NULL);
    EXPECT_TRUE(ptr3 == NULL);
}

TEST(MemoryPool, StressTest)
{
    uint64_t buffer[128];
    const size_t blockSize = sizeof(buffer[0]);
    const size_t numberOfBlocks = sizeof(buffer) / blockSize;

    MemoryPool memoryPool;
    initializeMemoryPool(&memoryPool, &buffer, numberOfBlocks, blockSize);
    
    unsigned *mask[numberOfBlocks];
    memset(mask, 0, sizeof(mask));

    srand(0); int x = 0, y = 0;
    for(unsigned iteration = 0; iteration < 16 * 1024; iteration++) {
        unsigned itemIndex = rand() % numberOfBlocks;

        if(!mask[itemIndex]) {
            mask[itemIndex] = (unsigned *) allocateBlock(&memoryPool);
            EXPECT_TRUE(mask[itemIndex] != NULL);
            *mask[itemIndex] = iteration; x++;
        }

        else {
            EXPECT_TRUE(*mask[itemIndex] < iteration);
            releaseBlock(&memoryPool, mask[itemIndex]);
            mask[itemIndex] = NULL; y++;
        }
    }
}