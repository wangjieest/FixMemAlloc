TARGET := examples

PRJ_DIR := $(realpath .)
TGT_DIR := $(PRJ_DIR)/Builds
OBJ_DIR := $(TGT_DIR)/Intermediate

CC_SRC := \
	$(PRJ_DIR)/Sources/MemoryPool.c

CXX_SRC := \
	$(PRJ_DIR)/Examples/PerformanceTest.cpp \
	$(PRJ_DIR)/Examples/List.cpp \
	$(PRJ_DIR)/Examples/Set.cpp

INCLUDE := \
	$(PRJ_DIR)/Sources \
	$(PRJ_DIR)/Wrappers \
	$(PRJ_DIR)/Examples

CCFLAGS += -std=c89 -Wall -pedantic -O2 -march=native
CXXFLAGS += -std=c++98 -Wall -pedantic -O2 -march=native

CPPFLAGS += $(foreach inc_dir,$(INCLUDE),-I$(inc_dir))
CC_OBJ := $(foreach cc_file,${CC_SRC:.c=.o},$(subst $(PRJ_DIR),$(OBJ_DIR),$(cc_file)))
CXX_OBJ := $(foreach cxx_file,${CXX_SRC:.cpp=.o},$(subst $(PRJ_DIR),$(OBJ_DIR),$(cxx_file)))

.PHONY: all clean distclean

default: all
all: $(TARGET)
$(TARGET): $(TGT_DIR)/$(TARGET)

$(TGT_DIR)/$(TARGET): $(CC_OBJ) $(CXX_OBJ)
	@-echo LD: $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CC_OBJ) $(CXX_OBJ) -o $(TGT_DIR)/$(TARGET)

$(OBJ_DIR)%.o: $(PRJ_DIR)%.c
	@-echo CC: $<
	@mkdir -p $(dir $@)
	@$(CC) $(CPPFLAGS) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR)%.o: $(PRJ_DIR)%.cpp
	@-echo CXX: $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	@-$(RM) -rf $(OBJ_DIR)

distclean:
	@-$(RM) -rf $(TGT_DIR)
