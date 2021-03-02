CU_SET_PATH("CMAKE_MALL_TELEPORT_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mod-mall-teleport.cpp")

AC_ADD_SCRIPT_LOADER("MallTeleport"      "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod-mall-teleport.conf.dist")

message("-- Mall Teleport CONFIGURED --")
