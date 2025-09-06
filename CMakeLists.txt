cmake_minimum_required(VERSION 3.10.0)
project(json-rpc-test VERSION 0.1.0 LANGUAGES C)

add_executable(json-rpc-test test.c json_rpc.c cJSON/cJSON.c)

include(CTest)
enable_testing()

add_test(NAME test COMMAND json-rpc-test)