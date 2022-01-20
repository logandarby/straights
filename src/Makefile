CXX=g++
CXXFLAGS=-std=c++14 -MMD -Wall -Werror=vla -DDEBUG=0 -g
OBJDIR=obj
OBJECTS=debug.o straights.o view.o deck.o player.o model.o controller.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=straights

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} ${CXXFLAGS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}
