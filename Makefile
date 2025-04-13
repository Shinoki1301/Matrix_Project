# ==============================================================================
#  Конфигурация проекта
# ==============================================================================

# --------------------------------
#  Компилятор и флаги
# --------------------------------
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -g -D_POSIX_C_SOURCE=200809L
INCLUDES = -Iinclude -Isrc -Isrc/matrix -Isrc/output
TEST_LDFLAGS = -lcunit

# --------------------------------
#  Директории проекта
# --------------------------------
SRC_DIR   = src
TEST_DIR  = tests
BUILD_DIR = build
DATA_DIR  = data

# ==============================================================================
#  Файлы проекта
# ==============================================================================

# --------------------------------
#  Основные исходные файлы
# --------------------------------
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(SRC_DIR)/matrix/*.c) \
       $(wildcard $(SRC_DIR)/output/*.c)

OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))

# --------------------------------
#  Тестовые файлы
# --------------------------------
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS = $(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/$(TEST_DIR)/%, $(TEST_SRCS:.c=.o))

# --------------------------------
#  Цели сборки
# --------------------------------
TARGET      = $(BUILD_DIR)/matrix_app
TEST_TARGET = $(BUILD_DIR)/matrix_tests

# ==============================================================================
#  Основные цели
# ==============================================================================
.PHONY: all clean run test init_data help format docs docs-open docs-clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@
	@echo "Основное приложение собрано: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ==============================================================================
#  Тестирование
# ==============================================================================

test: $(TEST_TARGET)
	@echo "\n🔍 === ЗАПУСК ТЕСТОВ ==="
	@./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS) $(filter-out $(BUILD_DIR)/main.o, $(OBJS))
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(TEST_LDFLAGS)
	@echo "Тестовый модуль собран: $@"

$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ==============================================================================
#  Вспомогательные цели
# ==============================================================================

# --------------------------------
#  Форматирование кода
# --------------------------------
format:
	@echo "=== Форматирование кода ==="
	@find $(SRC_DIR) $(TEST_DIR) include -name '*.c' -o -name '*.h' | \
		xargs clang-format --style=file -i -Werror
	@echo "Готово"

# --------------------------------
#  Управление тестовыми данными
# --------------------------------
init_data:
	@mkdir -p $(DATA_DIR)/input $(DATA_DIR)/output
	@echo "2 2\n1 2\n3 4" > $(DATA_DIR)/input/A.txt
	@echo "2 2\n5 6\n7 8" > $(DATA_DIR)/input/B.txt
	@echo "2 2\n1 1\n1 1" > $(DATA_DIR)/input/C.txt
	@echo "2 2\n1 2\n4 5" > $(DATA_DIR)/input/D.txt
	@echo "Тестовые данные созданы в $(DATA_DIR)/input/"

# --------------------------------
#  Очистка проекта
# --------------------------------
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Директория $(BUILD_DIR) очищена"

# --------------------------------
#  Запуск приложения
# --------------------------------
run: $(TARGET) init_data
	@echo "\n=== ЗАПУСК ПРИЛОЖЕНИЯ ==="
	@./$(TARGET)

# ==============================================================================
#  Документация
# ==============================================================================
DOXYFILE      = Doxyfile
DOCS_HTML_DIR = docs/html

docs:
	@echo "Генерация документации Doxygen..."
	@doxygen $(DOXYFILE)
	@echo "Документация сгенерирована в $(DOCS_HTML_DIR)/"

docs-open: docs
	@echo "\nОткрытие документации в браузере..."
	@if [ -f "$(DOCS_HTML_DIR)/index.html" ]; then \
		if command -v xdg-open > /dev/null; then \
			xdg-open "$(DOCS_HTML_DIR)/index.html"; \
		elif command -v open > /dev/null; then \
			open "$(DOCS_HTML_DIR)/index.html"; \
		elif command -v start > /dev/null; then \
			start "$(DOCS_HTML_DIR)/index.html"; \
		else \
			echo "Не удалось определить команду для открытия браузера"; \
		fi \
	else \
		echo "Ошибка: Файл документации не найден: $(DOCS_HTML_DIR)/index.html"; \
		echo "Проверьте вывод команды 'make docs' на наличие ошибок"; \
		exit 1; \
	fi

docs-clean:
	@echo "Удаление сгенерированных файлов документации..."
	@rm -rf $(DOCS_HTML_DIR) docs/latex/ docs/xml/ docs/man/ docs/rtf/
	@echo "Документация очищена"

# ==============================================================================
#  Справка
# ==============================================================================
help:
	@echo "  Доступные команды:"
	@echo ""
	@echo "  Основные команды:"
	@echo "    make all        - Собрать основное приложение (по умолчанию)"
	@echo "    make test       - Собрать и запустить все тесты"
	@echo "    make run        - Собрать и запустить приложение с тестовыми данными"
	@echo ""
	@echo "  Вспомогательные команды:"
	@echo "    make init_data  - Создать тестовые данные"
	@echo "    make clean      - Очистить проект"
	@echo "    make format     - Форматирование кода программы"
	@echo ""
	@echo "  Документация:"
	@echo "    make docs       - Генерирование документации"
	@echo "    make docs-open  - Сгенерировать и сразу открыть документацию"
	@echo "    make docs-clean - Удалить сгенерированную документацию"
	@echo ""
	@echo "    make help       - Показать эту справку"
