cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(XZXPIR VERSION 1.0 LANGUAGES CXX)

# set(VCPKG_PATH  D:/Applications/vcpkg)
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake)

set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(third_party ${PROJECT_SOURCE_DIR}/third_party)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_subdirectory(simple)
enable_testing()
add_subdirectory(test)