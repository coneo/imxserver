CXX = g++
TARGETS = SceneServer
C_FLAGS = -Wall -g #-Werror 

#如果要用到其他模块，则在此处添加
MODULE=$(BASE_DIR)
MODULE+=$(PACKETS_DIR)

SOURCE=$(wildcard *.cpp)
SOURCE+=$(foreach subdir, ${MODULE}, $(wildcard ${subdir}/*.cpp))
SOURCE+=$(foreach subdir, ${MODULE}, $(wildcard ${subdir}/*.cc))
SRCOBJ=$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

#头文件包含
INC = -I $(BASE_DIR)/
INC += -I $(PACKETS_DIR)/
INC += -I/usr/local/log4cxx/include

#库文件依赖
LIB_FLAGS = -lpthread -lprotoc -llog4cxx

all: $(TARGETS)
$(TARGETS): $(SRCOBJ)
	@$(CXX) $^ -o $@ $(LIB_FLAGS)
	@echo -e "-----------Create $@-----------"

%.o: %.cpp
	@$(CXX) $(INC) $(C_FLAGS) -c $< -o $@
	@echo complie	$@
%.o: %.cc
	@$(CXX) $(INC) $(C_FLAGS) -c $< -o $@
	@echo complie	$@

.PHONY:clean
clean:
	-rm -f $(SRCOBJ) $(TARGETS)
