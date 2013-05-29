CXX = g++
PATH_BASE = $(PATH_DGAME_PROJECT)/base
C_FLAGS = -Wall #-Werror 

MODULE=$(PATH_BASE)

SOURCE=$(wildcard *.cpp)
SOURCE+=$(wildcard *.cc)
SRC =$(notdir $(SOURCE))
SRCOBJ=$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRC)))
OBJS=$(SRCOBJ)

#头文件依赖
INC = -I $(BASE_DIR)/
INC += -I/usr/local/log4cxx/include		#log4cxx
#库文件依赖
LIB_FLAGS = -lpthread -lprotoc -llog4cxx

all:$(OBJS)
%.o: %.cpp
	@$(CXX) $(INC) $(C_FLAGS) -c $< -o $@ $(LIB_FLAGS)
	@echo complie	$@
%.o: %.cc
	@$(CXX) $(INC) $(C_FLAGS) -c $< -o $@ $(LIB_FLAGS)
	@echo complie	$@

.PHONY:clean
clean:
	-rm -f $(OBJS)
