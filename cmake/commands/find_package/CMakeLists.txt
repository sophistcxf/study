find_package(Boost REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(myapp main.cpp)
target_link_libraries(myapp ${Boost_LIBRARIES})
