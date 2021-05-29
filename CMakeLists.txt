cmake_minimum_required(VERSION 3.1)
project(guard VERSION 0.1 DESCRIPTION "A cross-platform C++ instance guard library")

file(GLOB src
    "guard/src/*.cpp"
    "guard/src/*/*.cpp"
    "guard/src/*/*/*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${src})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wextra -pedantic)

if (UNIX)
    target_link_libraries(${PROJECT_NAME} INTERFACE pthread rt ${CMAKE_DL_LIBS})
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} INTERFACE Kernel32)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "guard/include")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME}   PROPERTIES 
                                        CXX_STANDARD 17
                                        CXX_EXTENSIONS OFF
                                        CXX_STANDARD_REQUIRED ON)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PROJECT_NAME ${PROJECT_NAME})
