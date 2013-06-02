##
#  Version: 0.0.1
#  Date:  2013-3-23
##

#此处PATH_DGAME_PROJECT是一个环境变量，指向项目根目录
export BASE_DIR	= $(PATH_DGAME_PROJECT)/base
export PACKETS_DIR = $(PATH_DGAME_PROJECT)/Packets
export GATEWAY_DIR	= $(PATH_DGAME_PROJECT)/GatewayServer
export SCENESERVER_DIR = $(PATH_DGAME_PROJECT)/SceneServer
export LOGINSERVER_DIR = $(PATH_DGAME_PROJECT)/LoginServer

all:
	cd $(BASE_DIR);make
	cd $(PACKETS_DIR);make
	cd $(GATEWAY_DIR);make
	cd $(SCENESERVER_DIR);make
	cd $(LOGINSERVER_DIR);make

clean:
	cd $(BASE_DIR);make clean
	cd $(PACKETS_DIR);make clean
	cd $(GATEWAY_DIR);make clean
	cd $(SCENESERVER_DIR);make clean
	cd $(LOGINSERVER_DIR);make clean

