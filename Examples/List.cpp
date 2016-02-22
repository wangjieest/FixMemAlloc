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

#include "PerformanceTest.h"
#include "MemoryPoolAllocator.h"
#include <list>

const unsigned numberOfIterations = 128 * 1024 * 1024;

typedef int DataType;
typedef MemoryPoolAllocator<DataType> Allocator;
typedef std::list<DataType> DefaultList;
typedef std::list<DataType, Allocator> MemoryPoolList;

PERFORMANCE_TEST(List, DefaultAllocator)
{
    DefaultList testList;

    for(unsigned iteration = 0; iteration < numberOfIterations; iteration++)
        testList.push_back(iteration);
}

PERFORMANCE_TEST(List, MemoryPoolAllocator)
{
    Allocator allocator(numberOfIterations);
    MemoryPoolList testList(allocator);

    for(unsigned iteration = 0; iteration < numberOfIterations; iteration++)
        testList.push_back(iteration);
}
