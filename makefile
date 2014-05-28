CXX=g++
LIBS=-ltrutle -lSDL2 -lSDL2_image -lGL -lGLU -lGLEW
FLG=-std=gnu++0x -g
OUT=game

SRCDIR = src/

SRC=$(shell ls $(SRCDIR)*.cpp)
OBJ=$(subst .cpp,.o,$(SRC))

all: $(OBJ)
	$(CXX) $(FLG) -o $(OUT) $(OBJ) $(INC) $(LIBS)

%.o: %.cpp
	@echo "COMPILE $<"
	@$(CXX) $(FLG) -o $@ -c $< $(INC) $(LIBS)

clean:
	@echo "CLEANING"
	@rm -f $(OBJ)
