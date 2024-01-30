project(test)

include_directories (${CMAKE_SOURCE_DIR}/simple ${include_dir})
enable_testing()

function(new_test arg)
    message(STATUS "Add test: ${ARGV0}")
    add_executable(${ARGV0} ${ARGV0}.cpp)
    # target_include_directories(${ARGV0} PRIVATE  ${include_dir})
    target_link_libraries(${ARGV0} ${third_party}/fmt.lib simplepir)
    add_test(NAME ${ARGV0} COMMAND ${ARGV0})
endfunction()

new_test(params_test)
new_test(guass_sample_test)
new_test(db_test)
new_test(msg_test)
new_test(server_test)
new_test(pir_test)
new_test(matrix_test)
new_test(speed_test)

