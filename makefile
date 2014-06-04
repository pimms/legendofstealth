CXX=g++
LIBS=-ltrutle -lSDL2 -lSDL2_image -lSDL2_net -lGL -lGLU -lGLEW
FLG=-std=gnu++0x -g
GAME_OUT=game
SERV_OUT=serv

SRCDIR = src/

GAME_SRC=$(shell ls $(SRCDIR)*.cpp)
GAME_SRC+=$(shell ls $(SRCDIR)net/*.cpp)
GAME_OBJ=$(subst .cpp,.o,$(GAME_SRC))

SERV_SRC=$(shell ls $(SRCDIR)server/*.cpp)
SERV_SRC+=$(shell ls $(SRCDIR)net/*.cpp)
SERV_OBJ=$(subst .cpp,.o,$(SERV_SRC))

all: game server

game: $(GAME_OBJ) 
	$(CXX) $(FLG) -o $(GAME_OUT) $(GAME_OBJ) $(INC) $(LIBS)

server: $(SERV_OBJ)
	$(CXX) $(FLG) -o $(SERV_OUT) $(SERV_OBJ) $(INC) $(LIBS)

%.o: %.cpp
	@echo "COMPILE $<"
	@$(CXX) $(FLG) -o $@ -c $< $(INC) $(LIBS)

clean:
	@echo "CLEANING"
	@rm -f $(GAME_OBJ)
	@rm -f $(SERV_OBJ)
