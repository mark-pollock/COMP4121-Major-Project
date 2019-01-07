##
## Makefile for COMP4121 Assignment
##

CXX = g++

# compiler flags
CPPFLAGS = -Wall -Werror -std=c++11 -g

default: recommender

recommender: source_files/waxCombination.cpp source_files/wax.cpp source_files/snow.cpp source_files/rating.cpp source_files/dataManager.cpp source_files/recommender.cpp source_files/recommendation.cpp
	$(CXX) $(CPPFLAGS) -o recommender source_files/waxCombination.cpp source_files/wax.cpp source_files/snow.cpp source_files/rating.cpp source_files/dataManager.cpp source_files/recommender.cpp source_files/recommendation.cpp

clean:
	rm recommender

cleanAll:
	-rm recommender
	-rm data_files/*.dat
	-rm *.*~$
