add_executable(test
               test.cpp
               ${CMAKE_SOURCE_DIR}/src/lox.cpp
               ${CMAKE_SOURCE_DIR}/src/scanner.cpp
               ${CMAKE_SOURCE_DIR}/src/token_type.cpp
               ${CMAKE_SOURCE_DIR}/src/error_message.cpp)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_add_warnings(test)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain fmt::fmt)

include(CTest)
include(Catch)
catch_discover_tests(test WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
