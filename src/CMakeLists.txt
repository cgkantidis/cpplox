add_executable(cpplox main.cpp lox.cpp scanner.cpp token_type.cpp error_message.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Profile)
  target_link_options(cpplox PRIVATE "-pg")
elseif(CMAKE_BUILD_TYPE STREQUAL HeapProfile)
  target_link_libraries(cpplox PRIVATE tcmalloc)
elseif(CMAKE_BUILD_TYPE STREQUAL TSAN)
  target_link_options(cpplox PRIVATE -fsanitize=thread)
elseif(CMAKE_BUILD_TYPE STREQUAL MSAN)
  target_link_options(cpplox PRIVATE -fsanitize=memory)
endif()

install(TARGETS cpplox)
