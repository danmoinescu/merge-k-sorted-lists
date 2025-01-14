app_sources= \
            merge-sorted-lists.cpp \
            main.cpp
test_sources = $(wildcard test/*.cpp)
headers = \
            merge-sorted-lists.hpp

app_objects = ${app_sources:.cpp=.o}
test_objects = ${test_sources:.cpp=.o}

CXXFLAGS = --std=c++20 -g -Werror -Wall -I.

binary=merge-sorted-lists

.PHONY: clean all test debug test-debug
all: $(binary)

debug: CXXFLAGS += -DDEBUG -O0
debug: all

test-debug: CXXFLAGS += -DDEBUG -O0
test-debug: test


$(binary): $(app_objects)
	$(CXX) -o $@ $(app_objects)


%.o: %.cpp $(headers)
	$(CXX) $(CXXFLAGS) -c $< -o $@


export PATH := ./test-runner/src/:$(PATH)
app_args := $(addprefix  -a , $(app_objects))
test_args := $(addprefix  -t , $(test_objects))

test: $(app_objects) $(test_objects)
	CC=$(CXX) run-tests.sh $(app_args) $(test_args)


clean:
	rm -f $(binary) $(app_objects) $(test_objects)
