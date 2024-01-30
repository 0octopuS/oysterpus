# include(${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake)
# set(SEAL_DIR D:\\Applications\\vcpkg\\packages\\seal_x86-windows\\share\\SEAL)
project(simplepir)
set(SEAL_DIR E:\\5.paper\\code\\SEAL\\build\\cmake)
find_package(SEAL 4.1 REQUIRED)


add_library(${PROJECT_NAME} utils.hpp utils.cpp 
                            matrix.hpp matrix.cpp 
                            database.cpp database.hpp 
                            pir.hpp pir.cpp
                            simplePirClient.hpp simplePirClient.cpp
                            simplePirServer.hpp simplePirServer.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE  ${include_dir})
target_link_libraries(${PROJECT_NAME} ${third_party}/fmt.lib SEAL::seal)
