#!/bin/bash
rm -rf build/
# Создаем папку сборки (если её нет)
mkdir -p build && cd build || exit

# Генерируем Makefile (указываем путь к Qt6, если CMake его не находит)
cmake .. -DCMAKE_PREFIX_PATH="/usr/include/qt6/"

# Собираем проект (только цель SelecterGradient)
cmake --build . --target SelecterGradient -- -j24

# Запускаем
./SelecterGradient
cd ../
