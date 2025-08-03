cmake_minimum_required(VERSION 3.10)
project(MyProject)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Устанавливаем выходной путь для исполняемого файла
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SelecterGradient)

# 1. Поиск необходимых компонентов Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 2. Включение автоматической работы MOC, UIC и RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 3. Нахождение всех исходников (включая .h для MOC)
file(GLOB SelecterGradient_SOURCES
	"srv/UI/SelecterColor/*/*.cpp"
    "srv/UI/SelecterGradient/*/*.cpp"
    "srv/UI/Theme/*/*.cpp"
)

# 4. Создание цели с Qt6
add_executable(SelecterGradient
    ${SelecterGradient_SOURCES}
)

# 5. Подключение путей для заголовков (относительно ./srv)
target_include_directories(SelecterGradient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/srv
)

# 6. Линковка Qt6 библиотек
target_link_libraries(SelecterGradient
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# 7. Настройка стандарта C++23
target_compile_features(SelecterGradient PRIVATE cxx_std_23)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(FORCE_TARGET_SELECTION "Require target selection" ON)
