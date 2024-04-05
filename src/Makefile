FL = -Wall -Werror -Wextra -std=c++17
TEST_FL = -lstdc++ -lgtest -lm
GCOV_FL = -ftest-coverage -fprofile-arcs --coverage
gcc = gcc $(FL)

# CONTAINERS = array list map multiset queue set stack vector

all: test

test : clean
		$(gcc) ./tests/s21_tests.cc $(TEST_FL) -o ./tests/test; \
		./tests/test; \

clean :
	rm -rf */*.gcda */*.gcno *.info */*.css */*.html gcov */test

clang-check:
	clang-format --style=Google -n *.h */*.h */*.cc

clang-format:
	clang-format --style=Google -i *.h */*.h */*.cc

valgrind: test
		valgrind --leak-check=full --show-reachable=yes --track-origins=yes -s ./tests/test; \




