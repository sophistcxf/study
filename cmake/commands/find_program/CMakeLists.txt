project(find_program)

cmake_minimum_required(VERSION 3.20)

find_program(GIT_EXECUTABLE git)
if(GIT_EXECUTABLE)
  message(STATUS "Git found: ${GIT_EXECUTABLE}")
else()
  message(WARNING "Git not found.")
endif()

find_program(NO_CMD no_cmd)
if(NO_CMD)
    message(STATUS "Git found: ${NO_CMD}")
else()
    message(WARNING "no_cmd not found.")
endif()
