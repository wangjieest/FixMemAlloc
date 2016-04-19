# FixMemAlloc
Fixed-size blocks allocation for C and C++


## Abstract
This source provides with raw C implementation of fixed-size blocks allocation
for use in embedded and microcontroller areas, where limited resources plays key
role. Additional C++ wrappers extends usability for other applications where
fast memory allocation/deallocation is required e.g. lists, queues etc.


## License
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
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


## Build and execution
To build examples and unit tests, on Linux enter the project directory from
console and type `make`. On MS Windows use MS Visual Studio and rebuild whole
solution. In both cases an output directory `Builds` is created. After
successful build, the directory contains two programs called `examples` and
`tests` and all intermediate compilation files. Both programs can be executed
in console mode.

For cleanup type `make clean` to remove intermediate compilation files. To
remove all generated files including both programs and `Build` directory type
`make distclean`.


## Project directory organization
The below table contains description of project directory structure:

| Directory      | Description            |
|----------------|------------------------|
| .\             | Project root directory |
| .\Sources      | C implementation       |
| .\Wrappers     | C++ wrappers           |
| .\UnitTests    | Unit tests             |
| .\Externals    | Third-party components |
| .\Examples     | Usage examples         |
| .\Builds       | Output directory       |


## Memory pool
Memory pool is a set of fixed-size memory blocks used for dynamic memory
allocation. The memory pool allocation algorithms offers O(1) complexity for
block allocation and deallocation as well as for memory pool initialization.
Additionally the memory fragmentation problem is not occurring in this kind of
algorithms. This features leads to wide use of fixed-size memory allocation in
embedded and real-time systems.

To use memory pool implementation in a project, whole content of `Sources`
directory must be added to the project. The `MemoryPool.h` file must be included
in every source file, where memory pool management functions are going to be
used.


### Initialization
The variable of `MemoryPool` type represents single memory pool. In order to use
allocation / deallocation mechanisms within a specified memory pool it must be
initialized first.

Function `initializeMemoryPool` performs initialization of specified memory pool
and have the following declaration:

```
void initializeMemoryPool(
    struct MemoryPool *memoryPool,
    void *memoryRegion,
    size_t numberOfBlocks,
    size_t blockSize
);
```

**Parameter memoryPool**  
Pointer to variable of `MemoryPool` type. After successful initialization the
variable is indicating the memory pool used for further block allocations and
deallocations.

**Parameter memoryRegion**  
Pointer to continuous memory area. This area will be used by memory allocation
mechanisms and must be of size `numberOfBlocks` times `blockSize`.

**Parameter numberOfBlocks**  
Total number of blocks available for memory allocation in specified memory area.

**Parameter blockSize**  
Size of single block in bytes. This value must be above or equal to the value of
`MIN_MEMORY_POOL_BLOCK_SIZE`. Otherwise memory pool is initialized with no free
memory blocks.


### Block allocation
Once the memory pool is initialized, it can be used by `allocateBlock` function
for memory block allocation. The function have following declaration:

```
void *allocateBlock(
    struct MemoryPool *memoryPool
);
```

**Parameter memoryPool**  
Pointer to variable of `MemoryPool` type indicating the memory pool to be used
for block allocation.

**Returned value**  
Function returns pointer to allocated memory block or `NULL` on failure.
Function can fail only when there are no available blocks for allocation.


### Block release
When memory block is no longer used, it should be released by `releaseBlock`
function. The function have following declaration:

```
void releaseBlock(
    struct MemoryPool *memoryPool,
    void *pointer
);
```

**Parameter memoryPool**  
Pointer to variable of `MemoryPool` type indicating the memory pool previously
used for block allocation.

**Parameter pointer**  
Pointer to the block previously returned by `allocateBlock` function.


### Multiple memory regions
A memory pool represented by single variable of `MemoryPool` type can be used
to perform allocations in multiple non continuous memory regions. I this case,
the memory pool must be initialized with first of available memory regions. Once
the memory pool is empty and no more allocations can be performed, an auxiliary
memory pool is initialized with second memory region. It is used then for next
allocations, however all blocks are released into primary memory pool. Once no
more available blocks are in the second memory pool, it is reinitialized with
third memory region and so on.

Above algorithm is correct because the memory pool is in fact a list of free
blocks, where each not used block contains pointer to the next free block.
Therefore the limitation in block size is required. Memory pool cannot be
successfully initialized with blocks smaller than `MIN_MEMORY_POOL_BLOCK_SIZE`
bytes.


### Inlined functions
For special cases, when fast allocation or deallocation is required, the inlined
version of functions can be used. This however leads to increase in size of the
executable code. Each inlined function begins with `inlined` prefix followed by
original function name. For example function `allocateBlock` have
`inlinedAllocateBlock` equivalent.


## C++ Wrappers
Wrappers provides with object oriented code wrapping C implementation.
Depending on usage, four different wrappers are offered and described in
following chapters. All wrappers are located in `Wrappers` directory.

Each wrapper is a template class which derives from `MemoryPool` structure. The
template parameter is a type of block, used for allocations. When size of single
block is smaller than `MIN_MEMORY_POOL_BLOCK_SIZE` it is rounded up to this
value. Once new block is allocated, its constructor is called. Object destructor
is called when previously allocated block is released. When wrapper object is
destroyed, all not released blocks will not be destructed.

Implementation of each wrapper is provided in a separate header file. This
allows compiler to decide to inline functions or not in order to optimize code
for size or speed depending on configuration.


### Static and Dynamic Memory Pools
Once an object of `DynamicMemoryPool` type is created, it allocates sufficient
amount of memory for blocks allocation. After object destruction, the memory is
released.

On the other hand, object of `StaticMemoryPool` type neither allocates either
releases memory for blocks allocation. Instead, a pointer to the memory region
sufficient to hold all blocks must be specified on object construction through
`memoryRegion` parameter. It is important to remember to allocate enough memory
for blocks of size smaller than `MIN_MEMORY_POOL_BLOCK_SIZE` where block size is
rounded up to that value.

Both Static and Dynamic Memory Pools allows to allocate limited amount of blocks
at the same time. This limitation is specified through constructor as
`numberOfBlocks` parameter.


### Growing Memory Pool
The `GrowingMemoryPool` is very similar to the `DynamicMemoryPool`. However it
is only limited by system resources. The Growing Memory Pool allocates space for
N blocks, where N is equal to the `growByNumberOfBlocks` parameter specified at
object construction. When there is no free blocks available to allocate new one,
the Growing Memory Pool allocates another region with next N free blocks. All of
allocated memory regions are released at object destruction.


### Memory Pool Allocator
The `MemoryPoolAllocator` wrapper is dedicated for use with STL containers.


## Examples
Presented set of examples shows how to use Memory Pool Allocator for STL
containers. Output from `examples` program shows usually 2-5x speed up ratio
in comparison with standard STL allocator when used with list, set or map.