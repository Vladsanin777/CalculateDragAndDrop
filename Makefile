# Makefile для сборки проекта SelecterGradient

# Имя исполняемого файла
TARGET = SelecterGradient

# Компилятор C++
CXX = g++

# Установка стандарта C++23
CXXSTD = -std=c++23

# Каталог для исходников
SRCDIR = srv/UI

# Список файлов исходного кода (.cpp)
SRCS = \
	$(SRCDIR)/Theme/GradientPoint/GradientPoint.cpp \
	$(SRCDIR)/Theme/Gradient/Gradient.cpp \
	$(SRCDIR)/SelecterColor/AlphaSlider/AlphaSlider.cpp \
	$(SRCDIR)/SelecterColor/ColorArea2D/ColorArea2D.cpp \
	$(SRCDIR)/SelecterColor/ColorPicker/ColorPicker.cpp \
	$(SRCDIR)/SelecterColor/HueSlider/HueSlider.cpp \
	$(SRCDIR)/SelecterGradient/GradientEditor/GradientEditor.cpp \
	$(SRCDIR)/SelecterGradient/GradientStrip/GradientStrip.cpp \
	$(SRCDIR)/SelecterGradient/main.cpp

# Список заголовочных файлов (.hpp)
HDRS = \
	$(SRCDIR)/Theme/GradientPoint/GradientPoint.hpp \
	$(SRCDIR)/Theme/Gradient/Gradient.hpp \
	$(SRCDIR)/SelecterColor/AlphaSlider/AlphaSlider.hpp \
	$(SRCDIR)/SelecterColor/ColorArea2D/ColorArea2D.hpp \
	$(SRCDIR)/SelecterColor/ColorPicker/ColorPicker.hpp \
	$(SRCDIR)/SelecterColor/HueSlider/HueSlider.hpp \
	$(SRCDIR)/SelecterGradient/GradientStrip/GradientStrip.hpp \
	$(SRCDIR)/SelecterGradient/GradientEditor/GradientEditor.hpp

# Каталоги для объектных файлов и сгенерированных MOC-файлов
BUILD_DIR = build
MOC_DIR = $(BUILD_DIR)/moc

# Получение флагов компиляции и линковки для Qt6 с помощью pkg-config
QT_CFLAGS = $(shell pkg-config --cflags Qt6Core Qt6Gui Qt6Widgets)
QT_LDFLAGS = $(shell pkg-config --libs Qt6Core Qt6Gui Qt6Widgets)

# Флаги компилятора, включая путь поиска для srv/
# Это позволяет использовать #include "UI/..." в исходниках
CXXFLAGS = -g -Wall $(CXXSTD) -I. -I./srv $(QT_CFLAGS) -fPIC

# Флаги линковщика
LDFLAGS = $(QT_LDFLAGS)

# Список всех заголовочных файлов, которые нужно обработать moc
MOC_HDRS = $(filter %.hpp, $(HDRS))
# Список сгенерированных moc-файлов
MOC_SRCS = $(patsubst $(SRCDIR)/%.hpp,$(MOC_DIR)/moc_%.cpp,$(MOC_HDRS))

# Список объектных файлов для основных исходников
MAIN_OBJS = $(patsubst srv/UI/%.cpp,$(BUILD_DIR)/srv/UI/%.o,$(SRCS))
# Список объектных файлов для сгенерированных moc-файлов
MOC_OBJS = $(patsubst $(MOC_DIR)/%.cpp,$(MOC_DIR)/%.o,$(MOC_SRCS))

# Объединение всех объектных файлов в один список
OBJS = $(MAIN_OBJS) $(MOC_OBJS)

# Каталоги для объектных файлов
OBJ_DIRS = $(sort $(dir $(OBJS)))

.PHONY: all $(TARGET)
all: $(TARGET)

# Основная цель: сборка исполняемого файла
$(TARGET): $(OBJS)
	@echo "Соединение $(TARGET)..."
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

# Цель для создания каталогов, если они не существуют
$(OBJ_DIRS):
	@mkdir -p $@

# Правило для компиляции основных .cpp файлов
# Оно зависит от существования соответствующего каталога
$(BUILD_DIR)/srv/UI/%.o: srv/UI/%.cpp | $(OBJ_DIRS)
	@echo "Компиляция $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для создания moc-файлов из заголовочных файлов
$(MOC_DIR)/moc_%.cpp: $(SRCDIR)/%.hpp | $(MOC_DIR)
	@echo "Генерация moc для $<..."
	@moc $< -o $@

# Правило для компиляции сгенерированных moc-файлов
# Оно компилирует файлы из moc-каталога в объектные файлы в том же каталоге
$(MOC_DIR)/%.o: $(MOC_DIR)/%.cpp | $(MOC_DIR)
	@echo "Компиляция сгенерированного moc-файла $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Дополнительные цели
.PHONY: clean
clean:
	@echo "Очистка..."
	@rm -rf $(BUILD_DIR) $(TARGET)

