cmake_minimum_required(VERSION 3.0.2)

project(poc)

set(SOURCES src/main.cpp)

add_executable(bin ${SOURCES})

include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)

target_link_libraries(bin PRIVATE glaze::glaze)
