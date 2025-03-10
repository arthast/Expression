CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -O3 -Isrc

SRC = src/expression.cpp src/parser.cpp differentiator.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = differentiator

TEST_SRC = tests/test.cpp
# Тестовый исполняемый файл будет собираться из тестового объекта и объектов из src, необходимых для тестов.
TEST_TARGET = test_app

.PHONY: all test clean

# Цель all: сборка основного приложения, затем удаляются объектные файлы
all: $(TARGET)
	@rm -f $(OBJ)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Правило компиляции для файлов из каталога src
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило компиляции для файлов из каталога tests
tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Цель тестового приложения: собираем тестовый объект и объекты из src, которые требуются для тестов.
$(TEST_TARGET): tests/test.o src/expression.o src/parser.o
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) tests/test.o src/expression.o src/parser.o

# Цель test: сборка тестового приложения, его запуск и последующее удаление объектных файлов
test: $(TEST_TARGET)
	./$(TEST_TARGET)
	@rm -f tests/*.o src/*.o

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET) tests/*.o src/*.o
