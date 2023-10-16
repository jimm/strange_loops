NAME = strange_loops
# DEBUG = -DDEBUG -DDEBUG_STDERR

CPPFLAGS += -std=c++14 -MD -MP -g $(DEBUG)

LIBS = -lncurses -lportmidi

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

SRC = $(wildcard src/*.cpp) $(wildcard src/curses/*.cpp)
OBJS = $(SRC:%.cpp=%.o)
TEST_SRC = $(wildcard src/*.cpp) $(wildcard test/*.cpp)
TEST_OBJS = $(TEST_SRC:%.cpp=%.o)
TEST_LIBS = $(LIBS) -lCatch2Main -lCatch2

CATCH_CATEGORY ?= ""

.PHONY: all test tags clean distclean

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(LIBS) -o $@ $^

-include $(SRC:%.cpp=%.d)
-include $(TEST_SRC:%.cpp=%.d)

test: $(NAME)_test
	./$(NAME)_test --colour-mode=none $(CATCH_CATEGORY)

$(NAME)_test:	$(TEST_OBJS)
	$(CXX) $(LDFLAGS) $(TEST_LIBS) -o $@ # $(filter-out src/error.o,$^)

install:	$(bindir)/$(NAME)

$(bindir)/$(NAME):	$(NAME)
	cp ./$(NAME) $(bindir)
	chmod 755 $(bindir)/$(NAME)

tags:	TAGS

TAGS:	$(SRC)
	etags $(SRC)

clean:
	rm -f $(NAME) $(NAME)_test src/*.o src/curses/*.o test/*.o

distclean: clean
	rm -f src/*.d src/curses/*.d test/*.d
