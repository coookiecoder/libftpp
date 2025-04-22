add_executable(test_data_buffer main_data_buffer.cpp)
add_executable(test_pool main_pool.cpp)

target_link_libraries(test_data_buffer PRIVATE libftpp)
target_link_libraries(test_pool PRIVATE libftpp)

add_test(NAME test_data_buffer COMMAND test_data_buffer)
add_test(NAME test_pool COMMAND test_pool)

