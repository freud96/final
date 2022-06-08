file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-mul_4PE
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-mul_4PE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-acc_4PE
        main.cpp)

target_link_libraries(riscv-vp-acc_4PE rv32 platform-mul_4PE platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
