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
#include "StaticMemoryPool.h"
#include "DynamicMemoryPool.h"

#include "gtest.h"

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

TEST(StaticMemoryPool, SimpleAllocScheme)
{
    typedef size_t TestDataType;
    const size_t numberOfBlocks = 3;
    TestDataType memoryRegion[numberOfBlocks];

    StaticMemoryPool<TestDataType> staticMemoryPool(memoryRegion, numberOfBlocks);

    TestDataType *ptr1 = staticMemoryPool.allocateBlock();
    TestDataType *ptr2 = staticMemoryPool.allocateBlock();
    TestDataType *ptr3 = staticMemoryPool.allocateBlock();
    TestDataType *ptr4 = staticMemoryPool.allocateBlock();

    EXPECT_TRUE(ptr1 != NULL);
    EXPECT_TRUE(ptr2 != NULL);
    EXPECT_TRUE(ptr3 != NULL);
    EXPECT_TRUE(ptr4 == NULL);

    staticMemoryPool.releaseBlock(ptr2);

    TestDataType *ptr5 = staticMemoryPool.allocateBlock();
    TestDataType *ptr6 = staticMemoryPool.allocateBlock();

    EXPECT_TRUE(ptr5 != NULL);
    EXPECT_TRUE(ptr6 == NULL);
}

TEST(DynamicMemoryPool, SimpleAllocScheme)
{
    typedef size_t TestDataType;
    const size_t numberOfBlocks = 3;

    DynamicMemoryPool<TestDataType> dynamicMemoryPool(numberOfBlocks);

    TestDataType *ptr1 = dynamicMemoryPool.allocateBlock();
    TestDataType *ptr2 = dynamicMemoryPool.allocateBlock();
    TestDataType *ptr3 = dynamicMemoryPool.allocateBlock();
    TestDataType *ptr4 = dynamicMemoryPool.allocateBlock();

    EXPECT_TRUE(ptr1 != NULL);
    EXPECT_TRUE(ptr2 != NULL);
    EXPECT_TRUE(ptr3 != NULL);
    EXPECT_TRUE(ptr4 == NULL);

    dynamicMemoryPool.releaseBlock(ptr2);

    TestDataType *ptr5 = dynamicMemoryPool.allocateBlock();
    TestDataType *ptr6 = dynamicMemoryPool.allocateBlock();

    EXPECT_TRUE(ptr5 != NULL);
    EXPECT_TRUE(ptr6 == NULL);
}
