add_executable(demo)
target_sources(demo PRIVATE src/main.cpp)
target_link_libraries(demo PRIVATE cmdout)