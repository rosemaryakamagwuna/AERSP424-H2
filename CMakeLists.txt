cmake_minimum_required(VERSION 3.14.0)
project(SDLExample VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Dependenciesc
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.5
)
set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(SDL2)

# Our Project
set(SOURCE_FILES
    new.cpp HW2_Visualizer.cpp HW2_Visualizer.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS ON)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main)

# Copy the resource image to the same folder as the executable file.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD    # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different       # which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/map.bmp" # <--this is also in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)             # <--this is out-file path

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
