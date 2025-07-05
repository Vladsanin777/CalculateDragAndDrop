cmake_minimum_required(VERSION 3.16)
project(CalculateDragAndDrop VERSION 1.0 LANGUAGES CXX)

# Настройки компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Автоматизация Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Поиск Qt6
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Используем pkg-config для GMP и MPFR
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED gmp)
pkg_check_modules(MPFR REQUIRED mpfr)

# Рекурсивный поиск исходных файлов
file(GLOB_RECURSE SOURCES "srv/*.cpp")
file(GLOB_RECURSE HEADERS "srv/*.h")
file(GLOB_RECURSE FORMS "srv/*.ui")

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS})

# Включение специфических предупреждений в стиле C90 для C++
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -pedantic
        -Wold-style-cast
        -Wtraditional-conversion
        -Wno-pragma-once-outside-header  # Для подавления предупреждений о #pragma once
        # Флаги, специфичные для GCC:
        -Wstrict-prototypes
        -Wmissing-declarations
        -Wmissing-prototypes
        -Wnested-externs
        -Waggregate-return
        -Wpointer-arith
        -Wcast-qual
        -Wconversion
        -Wshadow
        -Wuseless-cast
        -Wzero-as-null-pointer-constant
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -pedantic
        -Wold-style-cast
        -Wc++98-compat
        -Wc++98-compat-pedantic
        -Wconversion
        -Wshadow
        -Wcast-align
        -Wzero-as-null-pointer-constant
    )
endif()

# Настройки для цели
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/srv
    ${GMP_INCLUDE_DIRS}
    ${MPFR_INCLUDE_DIRS}
)

# Линковка библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${GMP_LIBRARIES}
    ${MPFR_LIBRARIES}
)

# Установка
install(TARGETS ${PROJECT_NAME} DESTINATION bin)