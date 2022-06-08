file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-mul_2core_p
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-mul_2core_p PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-acc_mul2core_p
        main.cpp)

target_link_libraries(riscv-vp-acc_mul2core_p rv32 platform-mul_2core_p platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
