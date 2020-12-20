NAME = strange_loops
# DEBUG = -DDEBUG -DDEBUG_STDERR
MACOS_VER = 10.9
CPPFLAGS += -std=c++11 -mmacosx-version-min=$(MACOS_VER) -MD -MP -g $(DEBUG)
LIBS = -lc -lc++ -lncurses -lportmidi
LDFLAGS += $(LIBS) -macosx_version_min $(MACOS_VER)
prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin

SRC = $(wildcard src/*.cpp) $(wildcard src/curses/*.cpp)
OBJS = $(SRC:%.cpp=%.o)
TEST_SRC = $(wildcard test/*.cpp)
TEST_OBJS = $(TEST_SRC:%.cpp=%.o)
TEST_OBJ_FILTERS = src/$(NAME).o

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

-include $(C_SRC:%.c=%.d)
-include $(CPP_SRC:%.cpp=%.d)

.PHONY: test
test: $(NAME)_test
	./$(NAME)_test

$(NAME)_test:	$(OBJS) $(TEST_OBJS)
	$(LD) $(LDFLAGS) -o $@ $(filter-out $(TEST_OBJ_FILTERS),$^)

install:	$(bindir)/$(NAME)

$(bindir)/$(NAME):	$(NAME)
	cp ./$(NAME) $(bindir)
	chmod 755 $(bindir)/$(NAME)

.phony: tags
tags:	TAGS

TAGS:	$(SRC)
	etags $(SRC)

.PHONY: clean
clean:
	rm -f $(NAME) $(NAME)_test src/*.o src/curses/*.o test/*.o

.PHONY: distclean
distclean: clean
	rm -f src/*.d src/curses/*.d test/*.d
