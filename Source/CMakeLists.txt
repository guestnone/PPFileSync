include(CMakeSources.cmake)

add_executable(PPFileSync ${PPFS_SRC})
target_link_libraries(PPFileSync OpenSSL::SSL)

add_executable(Test ${PPFS_INC_NOFILTER} ${PPFS_SRC_NOFILTER} PPFSTestMain.c)
target_link_libraries(Test  OpenSSL::SSL)