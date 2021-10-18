# cmake_minimum_required(VERSION 3.0)
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "-Wall")

# project(echo)
# set(BIN echo)

# add_executable(${BIN} main.cpp)
# target_link_libraries(${BIN})














cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

set(BIN echo)
project($(BIN))
set(CMAKE_BUILD_TYPE Release)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${BIN}
    main.cpp
    # polygon.cpp
    # parse_polygonfile.cpp
    # graph.cpp
    # osmparsing.cpp
)
target_link_libraries(${BIN} -pthread)
# target_include_directories(${BIN} PUBLIC ${CMAKE_SOURCE_DIR})






# cmake_minimum_required(VERSION 3.0)

# project(route-engine)

# enable_testing()

# include(CheckIncludeFiles)

# set(PREPROCESS_BIN route-contract-graphs)
# set(ORDERNODES_BIN route-order-nodes)

# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup(TARGETS)


# # This list should grow to allow for maximum compile-time checking
# set(CPP_WARNINGS " \
#     -Wall \
#     -Wextra \
#     -Wshadow \
#     -Wnon-virtual-dtor \
#     -Wold-style-cast \
#     -Wcast-align \
#     -Wunused \
#     -Woverloaded-virtual \
#     -Wpedantic -Wconversion \
#     -Wsign-conversion \
#     -Wlogical-op \
#     -Wuseless-cast \
#     -Wdouble-promotion \
#     -Wformat=2 \
# ")


# # This is a list of compiler warnings that we ignore
# # Adding warnings to this list is not allowed
# # Unsupported warnings should be removed one by one until this list is empty
# set(UNSUPPORTED_WARNINGS " \
#     -Wno-sign-conversion \
#     -Wno-conversion \
#     -Wno-overloaded-virtual \
#     -Wno-unused-parameter \
#     -Wno-useless-cast \
#     -Wno-missing-field-initializers \
#     -Wno-shadow \
#     -Wno-type-limits \
#     -Wno-strict-aliasing \
#     -Wno-terminate \
#     -Wno-format-nonliteral \
# ")

# set(CMAKE_CXX_FLAGS "-std=c++14 -pedantic -Werror ${CPP_WARNINGS} ${UNSUPPORTED_WARNINGS} -fopenmp ${CMAKE_CXX_FLAGS}")

# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage -g3 -fsanitize=address")

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(Threads)
# find_package(PostgreSQL)
# find_package(OpenMP)
# find_package(Boost COMPONENTS program_options log log_setup)
# # Package won't be found on Ubuntu 15.04 / CMake 3.0.2 (dev configuration)
# if (NOT PostgreSQL_FOUND)
#     set(PostgreSQL_INCLUDE_DIRS "/usr/include/postgresql")
#     set(PostgreSQL_LIBRARIES pq)
#     check_include_files(postgresql/libpq-fe.h HAVE_PQ_H)
#     if(NOT HAVE_PQ_H)
#       message(SEND_ERROR "Couldn't find <libpq-fe.h>. libpq-dev doesn't seem to be installed")
#     endif()
# endif()

# check_include_files(ev.h HAVE_EV_H)
# if(NOT HAVE_EV_H)
#   message(SEND_ERROR "Couldn't find <ev.h>. libev-dev doesn't seem to be installed")
# endif()

# # add_definitions("-DBOOST_LOG_DYN_LINK=0")

# set (AMQP_LIBRARIES "-l:libamqpcpp.a")
# set (LIBEV_LIBRARIES "-lev")

# set (EXTERNAL_LIB_PATH ${PROJECT_SOURCE_DIR}/../lib)
# set (ROUTE_SHARED_ROOT_PATH ${EXTERNAL_LIB_PATH}/route-shared/cpp)
# set (ROUTE_SHARED_SRC_PATH ${ROUTE_SHARED_ROOT_PATH}/src)

# include_directories(.)
# include_directories(/usr/include/postgresql/)
# include_directories(/usr/include/postgresql/9.4/server/)
# include_directories(${ROUTE_SHARED_SRC_PATH} ${ROUTE_SHARED_ROOT_PATH})

# set(LIB_SOURCES
#     preprocess.cpp
#     core_base/dev/defines.cpp
#     core_base/common/graph.cpp
#     core_base/common/graph_path.cpp
#     core_base/common/traffic_info.cpp
#     core_base/common/prohibitions.cpp
#     core_base/common/datetime.cpp
#     core_base/common/loggerRE.cpp
#     core_base/common/adjacency_array.cpp
#     core_base/common/raw_prohibitions.cpp
#     core_base/tch/ttf.cpp
#     core_base/tch/ttf_operations.cpp
#     core_base/tch/ttf_operations_internal.cpp
#     core_preproc/common/node_ordering.cpp
#     core_preproc/common/contraction_graph_base.cpp
#     core_preproc/common/turn_contraction.cpp
#     core_preproc/ch/build_ch_graph.cpp
#     core_preproc/ch/node_ordering_ch.cpp
#     core_preproc/ch/contraction_ch.cpp
#     core_preproc/ch/contraction_graph_ch.cpp
#     core_preproc/ch/preprocessing_ch_internal.cpp
#     core_preproc/ch/preprocessing_ch.cpp
#     core_preproc/tch/contraction_graph_tch.cpp
#     core_preproc/tch/contraction_tch.cpp
#     core_preproc/tch/node_ordering_tch.cpp
#     core_preproc/tch/ttp.cpp
#     core_preproc/tch/compression_atch.cpp
#     core_preproc/tch/build_tch_graph.cpp
#     core_preproc/tch/preprocessing_tch.cpp
#     core_preproc/wch/contraction_wch.cpp
#     core_preproc/wch/contraction_graph_wch.cpp
#     core_preproc/wch/node_ordering_wch.cpp
#     core_preproc/wch/preprocessing_wch_internal.cpp
#     core_preproc/wch/preprocessing_wch.cpp
#     core_query/common/query_graph_dump.cpp
#     core_query/common/path.cpp
#     core_query/common/path64.cpp
#     core_query/common/post_process.cpp
#     core_query/common/query_graph_base.cpp
#     core_query/common/section_map.cpp
#     core_query/common/query_graph_metrics.cpp
#     core_query/common/contracted_query_graph_metrics.cpp
#     core_query/common/dump_tools.cpp
#     core_query/dijkstra/dijkstra.cpp
#     core_query/dijkstra/routing_dijkstra.cpp
#     core_query/dijkstra/query_graph_dijkstra.cpp
#     core_query/ch/dijkstra_ch.cpp
#     core_query/ch/routing_ch.cpp
#     core_query/ch/query_graph_ch.cpp
#     core_query/tch/routing_tch.cpp
#     core_query/tch/dijkstra_tch.cpp
#     core_query/tch/dijkstra_atch.cpp
#     core_query/tch/query_graph_tch.cpp
#     core_query/wch/routing_wch.cpp
#     core_query/wch/query_graph_wch.cpp
#     core_query/wtch/csa.cpp
#     core_query/wtch/routing_wtch.cpp
#     core_query/wtch/routing_wtch_csa.cpp
#     core_query/wtch/query_graph_wtch.cpp
#     database/common/load_raw_tables.cpp
#     database/common/prohibitions.cpp
#     database/common/edges.cpp
#     database/common/edges_internal.cpp
#     database/common/defines.cpp
#     database/common/cmn_schema.cpp
#     database/common/graph_flag.cpp
#     database/common/ld_cmn_sections.cpp
#     database/common/ld_cmn_ordering.cpp
#     database/common/st_cmn_ordering.cpp
#     database/common/ld_cmn_prohibitions.cpp
#     database/common/st_cmn_turn_shortcuts.cpp
#     database/dijkstra/ld_dijkstra_query.cpp
#     database/ch/ch_schema.cpp
#     database/ch/ld_ch_edges.cpp
#     database/ch/ld_ch_preprocessing.cpp
#     database/ch/st_ch_contracted.cpp
#     database/ch/ld_ch_query.cpp
#     database/tch/tch_schema.cpp
#     database/tch/ld_tch_ttf.cpp
#     database/tch/ld_tch_preprocessing.cpp
#     database/wch/ld_wch_graph_description.cpp
#     interface_c/dev/err.cpp
#     interface_c/common/cmn_preprocessing.cpp
#     interface_c/common/cmn_query.cpp
#     interface_c/dijkstra/dijkstra_query.cpp
#     interface_c/ch/ch_preprocessing.cpp
#     interface_c/ch/ch_query.cpp
#     interface_c/tch/tch_preprocessing.cpp
#     interface_c/tch/tch_query.cpp
#     interface_c/wch/wch_query.cpp
#     interface_c/wtch/wtch_query.cpp
#     interface_rpc/common/run_worker.cpp
#     interface_rpc/common/request_handler.cpp
#     interface_rpc/common/version_handler.cpp
#     interface_rpc/common/tools_handler.cpp
#     interface_rpc/common/monitor.cpp
#     tools/load_query_graph.cpp
#     )

# set (LIB_SHARED_SRC
#     "${ROUTE_SHARED_SRC_PATH}/utils/assert.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/utils/exceptions.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/utils/misc.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/database/db_connect.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/database/db_result.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/database/db_utils.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/interface_rpc/rpc_worker.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/log/logger.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/log/cleaning.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/data_patching/patch_lyon_periph.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/data_patching/patch_lille_periph.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/data_patching/patch_paris_30.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/data_patching/patch_section_speed.cpp"
#     "${ROUTE_SHARED_SRC_PATH}/system/tmp_file.cpp"
#     )

# set (LINKED_LIBRARIES
#     ${CMAKE_THREAD_LIBS_INIT}
#     ${PostgreSQL_LIBRARIES}
#     ${LIBEV_LIBRARIES}
#     ${AMQP_LIBRARIES}
#     ${OpenMP_LIBRARIES}
#     ${Boost_LIBRARIES})

# add_library(route_engine SHARED ${LIB_SOURCES} ${LIB_SHARED_SRC})
# set_target_properties(route_engine PROPERTIES DEBUG_POSTFIX _d)
# set_target_properties(route_engine PROPERTIES SUFFIX .so)
# target_link_libraries(route_engine ${LINKED_LIBRARIES})

# add_executable(${PREPROCESS_BIN} preprocess_main.cpp)
# set_target_properties(${PREPROCESS_BIN} PROPERTIES DEBUG_POSTFIX _d)
# target_link_libraries(${PREPROCESS_BIN} route_engine ${Boost_LIBRARIES})

# add_executable(${ORDERNODES_BIN} ordernodes_main.cpp)
# set_target_properties(${ORDERNODES_BIN} PROPERTIES DEBUG_POSTFIX _d)
# target_link_libraries(${ORDERNODES_BIN} route_engine ${Boost_LIBRARIES})

# add_executable(dump-prohibitions dump_prohibitions_main.cpp)
# target_link_libraries(dump-prohibitions route_engine)

# add_executable(dump-sections dumpsections_main.cpp)
# target_link_libraries(dump-sections route_engine)

# add_executable(graphcompare graphcompare_main.cpp)
# target_link_libraries(graphcompare route_engine)

# add_executable(graphsanitycheck graphsanitycheck_main.cpp)
# target_link_libraries(graphsanitycheck route_engine)

# set (POC_ONE2MANY_SRC
#     "one2many_poc_main.cpp"
#     "poc_one2many/echo_handler.cpp"
#     "poc_one2many/route_handler.cpp"
#     "poc_one2many/params_parsing.cpp"
#     "poc_one2many/ch_computer.cpp"
# )
# add_executable(one2many ${POC_ONE2MANY_SRC})
# target_include_directories(one2many PUBLIC "${CONAN_INCLUDE_DIRS_CPP-HTTPLIB}")
# target_link_libraries(one2many route_engine -pthread)

# add_executable(loadgraphindb loadgraphindb_main.cpp)
# target_link_libraries(loadgraphindb route_engine)

# add_subdirectory(unit_tests)

