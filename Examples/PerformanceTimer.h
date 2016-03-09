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

#ifndef PerformanceTimerH
#define PerformanceTimerH

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#elif defined(__linux__)
    #include <time.h>
#else
    #error Unsupported operating system
#endif

class PerformanceTimer
{
    public:
    
        PerformanceTimer()
        {
            #if defined(_WIN32) || defined(_WIN64)
                QueryPerformanceFrequency(&frequency);
            #endif
        }

        inline void start()
        {
            #if defined(_WIN32) || defined(_WIN64)
                QueryPerformanceCounter(&startTime);
            #else
                clock_gettime(CLOCK_MONOTONIC, &startTime);
            #endif
        }

        inline void stop()
        {
            #if defined(_WIN32) || defined(_WIN64)
                QueryPerformanceCounter(&stopTime);
            #else
                clock_gettime(CLOCK_MONOTONIC, &stopTime);
            #endif
        }

        double getTime() const
        {
            #if defined(_WIN32) || defined(_WIN64)
                return (stopTime.QuadPart - startTime.QuadPart) /
                    (double) (frequency.QuadPart);
            #else
                return
                    stopTime.tv_sec + stopTime.tv_nsec * 1e-9 -
                    startTime.tv_sec - startTime.tv_nsec * 1e-9;
            #endif
        }


    private:
    
        #if defined(_WIN32) || defined(_WIN64)
            LARGE_INTEGER frequency;
            LARGE_INTEGER startTime;
            LARGE_INTEGER stopTime;            
        #else
            timespec startTime;
            timespec stopTime;
        #endif
};

#endif
