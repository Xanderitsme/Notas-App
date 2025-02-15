# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(TodoList)

# Configurar el estándar de C++17 (necesario para std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Agregar el directorio de includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Agregar el ejecutable
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# En Linux, linkear con filesystem si es necesario
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
    find_package(Curses REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
    add_compile_options(-fexec-charset=UTF-8)
endif()

# Configuración específica para Windows
if(WIN32)
    add_definitions(-D_WIN32)
endif() 