# $Id: gfx-config.in 343 2008-09-13 18:34:59Z garland $

LDLIBS = -lm
OPT=-Os
CFLAGS = $(OPT) $(INCLUDES)
CC = gcc

# Added the -w flag to get rid of stupid warnings, add it back again if needed

CXX = g++
CXXFLAGS = -g -O2 -w -Wall -Wno-sign-compare -Iinclude -DHAVE_CONFIG_H 
OBJS = core.o grid.o agent.o headController.o controller.o oorl_learner.o passenger.o  relation.o wall.o



oorl_taxi: $(OBJS)
	$(CXX) -o $@ $^ -lglut  -lGLU -lGL
clean:
	rm  -f $(OBJS) oorl_taxi
