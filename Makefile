# FixMemAlloc - Fixed-size blocks allocation for C and C++
#
# Copyright (c) 2016, Mariusz Moczala
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of FixMemAlloc nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


.PHONY: default all clean distclean examples tests

HOME_DIR := $(realpath .)
BUILD_DIR := $(HOME_DIR)/Builds

default: all

all: examples tests

clean:
	@-$(RM) -rf $(PROJECT_DIR) $(TEST_DIR)

distclean:
	@-$(RM) -rf $(BUILD_DIR)



PROJECT_TARGET := examples
PROJECT_DIR := $(BUILD_DIR)/FixMemAlloc

PROJECT_SOURCES := \
	$(HOME_DIR)/Sources/MemoryPool.c \
	$(HOME_DIR)/Examples/PerformanceTest.cpp \
	$(HOME_DIR)/Examples/List.cpp \
	$(HOME_DIR)/Examples/Set.cpp \
	$(HOME_DIR)/Examples/Map.cpp

PROJECT_INCLUDES := \
	$(HOME_DIR)/Sources \
	$(HOME_DIR)/Wrappers \
	$(HOME_DIR)/Examples

PROJECT_FLAGS_LD := $(LDFLAGS) $(LDLIBS)
PROJECT_FLAGS_CC := $(CFLAGS) -std=c89 -Wall -pedantic -O2 -march=native
PROJECT_FLAGS_CXX := $(CXXFLAGS) -std=c++98 -Wall -pedantic -O2 -march=native
PROJECT_FLAGS_CPP := $(CPPFLAGS) $(addprefix -I, $(PROJECT_INCLUDES))
PROJECT_OBJ := $(subst $(HOME_DIR), $(PROJECT_DIR), $(addsuffix .o, $(basename $(PROJECT_SOURCES))))

$(PROJECT_TARGET): $(BUILD_DIR)/$(PROJECT_TARGET)

$(BUILD_DIR)/$(PROJECT_TARGET): $(PROJECT_OBJ)
	@-echo LD: $@
	@mkdir -p $(dir $@)
	@$(CXX) $(PROJECT_FLAGS_LD) $(PROJECT_OBJ) -o $(BUILD_DIR)/$(PROJECT_TARGET)

$(PROJECT_DIR)%.o: $(HOME_DIR)%.c
	@-echo CC: $<
	@mkdir -p $(dir $@)
	@$(CC) $(PROJECT_FLAGS_CPP) $(PROJECT_FLAGS_CC) -c $< -o $@

$(PROJECT_DIR)%.o: $(HOME_DIR)%.cpp
	@-echo CXX: $<
	@mkdir -p $(dir $@)
	@$(CXX) $(PROJECT_FLAGS_CPP) $(PROJECT_FLAGS_CXX) -c $< -o $@



TEST_TARGET := tests
TEST_DIR := $(BUILD_DIR)/UnitTests

TEST_SOURCES := \
	$(HOME_DIR)/Sources/MemoryPool.c \
	$(HOME_DIR)/Externals/gtest-all.cc \
	$(HOME_DIR)/Externals/gtest_main.cc \
	$(HOME_DIR)/UnitTests/UTMemoryPool.cpp

TEST_INCLUDES := \
	$(HOME_DIR)/Sources \
	$(HOME_DIR)/Externals \
	$(HOME_DIR)/UnitTests

TEST_FLAGS_LD := $(LDFLAGS) $(LDLIBS) -lpthread
TEST_FLAGS_CC := $(CFLAGS) -std=c89 -Wall -pedantic -O2 -march=native
TEST_FLAGS_CXX := $(CXXFLAGS) -std=c++11 -Wall -pedantic -O2 -march=native
TEST_FLAGS_CPP := $(CPPFLAGS) $(addprefix -I, $(TEST_INCLUDES))
TEST_OBJ := $(subst $(HOME_DIR), $(TEST_DIR), $(addsuffix .o, $(basename $(TEST_SOURCES))))

$(TEST_TARGET): $(BUILD_DIR)/$(TEST_TARGET)

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJ)
	@-echo LD: $@
	@mkdir -p $(dir $@)
	@$(CXX) $(TEST_FLAGS_LD) $(TEST_OBJ) -o $(BUILD_DIR)/$(TEST_TARGET)

$(TEST_DIR)%.o: $(HOME_DIR)%.c
	@-echo CC: $<
	@mkdir -p $(dir $@)
	@$(CC) $(TEST_FLAGS_CPP) $(TEST_FLAGS_CC) -c $< -o $@

$(TEST_DIR)%.o: $(HOME_DIR)%.cc
	@-echo CXX: $<
	@mkdir -p $(dir $@)
	@$(CXX) $(TEST_FLAGS_CPP) $(TEST_FLAGS_CXX) -c $< -o $@

$(TEST_DIR)%.o: $(HOME_DIR)%.cpp
	@-echo CXX: $<
	@mkdir -p $(dir $@)
	@$(CXX) $(TEST_FLAGS_CPP) $(TEST_FLAGS_CXX) -c $< -o $@
