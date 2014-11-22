default: ../resolution

test: ../run-test
	(cd ..; ./run-test)

SUFFIXES += .d
SRC := $(sort $(wildcard *.cpp) parser.tab.cpp lex.yy.cpp)
MAINS := main.o main-test.o
ALLOBJECTS := $(SRC:%.cpp=%.o)
OBJECTS := $(filter-out $(MAINS),$(ALLOBJECTS))
TEST_LDDEPS := $(LDDEPS) -L../UnitTest -lUnitTest++
CXXFLAGS := -Wall -Wextra -std=c++0x -I../UnitTest/src

UNITTEST := ../UnitTest/libUnitTest++.a
$(UNITTEST):
	make -C ../UnitTest

clean:
	rm -f *.d *.o 
	rm -f *~ 
	rm -f ../resolution ../run-test

cleanparser:
	rm -f parser.tab.cpp lex.yy.cpp

help:
	sed -n -e 's/^#HELP\(.*\)$$/\1/p' < Makefile

#HELP
#HELP pour afficher l'aide du Makefile
#HELP make help
#HELP
#HELP pour compiler le projet
#HELP make 
#HELP
#HELP pour lancer les tests
#HELP make test
#HELP
#HELP pour nettoyer les fichiers compilés et temporaires
#HELP make clean
#HELP

../resolution: $(OBJECTS) main.o
	$(CXX) $(CXXFLAGS) $(OBJECTS) main.o $(LDDEPS) -o ../resolution

../run-test: $(OBJECTS) main-test.o $(UNITTEST)
	$(CXX) $(CXXFLAGS) $(OBJECTS) main-test.o $(TEST_LDDEPS) -o ../run-test

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $*.cpp -MF $*.d

parser.tab.cpp: parser.y
	bison -d parser.y --output-file=parser.tab.cpp

lex.yy.cpp: parser.lex
	flex -o lex.yy.cpp parser.lex 

ifneq ($(subst parser,,$(MAKECMDGOALS)),clean) 
include $(ALLOBJECTS:%.o=%.d)
endif
