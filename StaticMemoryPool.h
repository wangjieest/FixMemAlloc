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

#ifndef StaticMemoryPoolH
#define StaticMemoryPoolH

#include "MemoryPool.h"

template <class DataType>
class StaticMemoryPool : protected MemoryPool
{
    public:

        StaticMemoryPool(DataType *memoryRegion, size_t numberOfElements)
        {
            const size_t memoryRegionSize = sizeof(DataType) * numberOfElements;
            initMemoryPool(this, memoryRegion, memoryRegionSize, sizeof(DataType));
        }

        DataType *allocateBlock()
        {
            void *pointer = ::allocateBlock(this);
            return static_cast<DataType *>(pointer);
        }

        void releaseBlock(DataType *pointer)
        {
            ::releaseBlock(this, pointer);
        }


    private:

        StaticMemoryPool(const StaticMemoryPool &staticMemoryPool);
        StaticMemoryPool & operator =(const StaticMemoryPool &staticMemoryPool);
};

#endif
