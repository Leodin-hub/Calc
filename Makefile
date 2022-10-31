WWW = -Wall -Werror -Wextra
LINUX=-lsubunit -lrt -lpthread -lm
LIBS=-lcheck
LFLAGS=-fprofile-arcs -ftest-coverage
OS=$(shell uname -s)

all: clean
	@cd smart-calc/ && qmake && make && make clean && rm -rf Makefile
	@open smart-calc/smart-calc.app

test: clean logic.a
ifeq ($(OS), Darwin)
	@gcc $(WWW) unit-tests.c logic.a -o unit_test $(LIBS)
else
	@gcc $(WWW) unit-tests.c logic.a -o unit_test $(LIBS) $(LINUX)
endif
	@./unit_test
	@make clean

gcov_report: clean
ifeq ($(OS), Darwin)
	@gcc $(WWW) -fprofile-arcs -ftest-coverage unit-tests.c smart-calc/logic.c -o unit_test $(LIBS)
else
	@gcc $(WWW) -fprofile-arcs -ftest-coverage unit-tests.c smart-calc/logic.c -o unit_test $(LIBS) $(LINUX)
endif
	@./unit_test
	@lcov -t "test" -o test.info -c -d .
	@genhtml -o report test.info

valgf: clean logic.a
ifeq ($(OS), Darwin)
	@gcc $(WWW) unit-tests.c logic.a -o unit_test $(LIBS)
	@CK_FORK=no valgrind --leak-check=full -s ./unit_test
else
	@gcc $(WWW) unit-test.c logic.a -o unit_test $(LIBS) $(LINUX)
	@CK_FORK=no valgrind --leak-check=full -s ./unit_test
endif

check:
	@cp ../materials/linters/.clang-format .
	@echo *.c files
	@clang-format -n smart-calc/*.c
	@echo *.cpp files
	@clang-format -n smart-calc/*.cpp
	@echo *.h files
	@clang-format -n smart-calc/*.h
	@rm -rf .clang-format

logic.a:
	@gcc -c smart-calc/logic.c
	@ar rcs logic.a logic.o
	@ranlib logic.a

clean_obj:
	@rm -rf *.o

clean_lib: 
	@rm -rf *.a

clean_test:
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.info
	@rm -rf test
	@rm -rf report

clean_app:
	@rm -rf smart-calc/*.app

clean: clean_lib clean_lib clean_test clean_obj clean_app
	@rm -rf unit_test
	@rm -rf a.out
