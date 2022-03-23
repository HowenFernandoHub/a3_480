# Author: Howen Anthony Fernando, Nathan Schwedock
# RedIDs: 822914112, 822910056
# Class: CS480, Spring 2022
# Assignment #3: Demand Paging with TLB
# Filename: Makefile
 
# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
pagingwithtlb : main.o pageTable.o Map.o level.o tlb.o tracereader.o output_mode_helpers.o
	$(CXX) $(CXXFLAGS) -g -o pagingwithtlb $^

main.o : main.cpp
	$(CXX) $(CXXFLAGS) -g -c $<

pageTable.o : pageTable.cpp pageTable.hpp
	$(CXX) $(CXXFLAGS) -g -c $<

Map.o : Map.cpp Map.hpp
	$(CXX) $(CXXFLAGS) -g -c $<

level.o : level.cpp level.hpp
	$(CXX) $(CXXFLAGS) -g -c $<

tlb.o : tlb.cpp tlb.hpp
	$(CXX) $(CXXFLAGS) -g -c $<

tracereader.o : tracereader.c tracereader.h
	$(CXX) $(CXXFLAGS) -g -c $<

output_mode_helpers.o : output_mode_helpers.c output_mode_helpers.h
	$(CXX) $(CXXFLAGS) -g -c $<

clean :
	rm -f *.o