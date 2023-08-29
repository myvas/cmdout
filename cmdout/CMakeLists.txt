find_package(Threads)

add_library(cmdout)
target_include_directories(cmdout PUBLIC include)
target_sources(cmdout PRIVATE "src/cmdout.cpp" "src/system.cpp")
target_link_libraries(cmdout PRIVATE ${CMAKE_THREAD_LIBS_INIT})
#set_target_properties(cmdout PROPERTIES COMPILE_FLAGS "-pthread" LINK_FLAGS "-pthread")

if(INSTALL_CMDOUT)
	install(TARGETS cmdout)
endif()