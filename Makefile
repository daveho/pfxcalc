C_SRCS = token.c main.c util.c node.c
C_OBJS = $(C_SRCS:%.c=%.o)

CXX_SRCS = lexer.cpp
CXX_OBJS = $(CXX_SRCS:%.cpp=%.o)

CC = gcc
CFLAGS = -g -Wall

CXX = g++
CXXFLAGS = $(CFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all : pfxcalc

pfxcalc : $(C_OBJS) $(CXX_OBJS)
	$(CXX) -o $@ $(C_OBJS) $(CXX_OBJS)

clean :
	rm -f *.o pfxcalc

depend :
	$(CC) $(CFLAFGS) -M $(C_SRCS) > depend.mak
	$(CXX) $(CXXFLAGS) -M $(CXX_SRCS) >> depend.mak

depend.mak :
	touch $@

include depend.mak
