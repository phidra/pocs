cmake_minimum_required(VERSION 3.0.2)

project(poc-of-external-project CXX)

include(ExternalProject)

set(SOURCES src/main.cpp)

add_executable(bin ${SOURCES})


# An archive will be downloaded from an URL and extracted into a destination directory :
set(DOWNLOAD_URL "https://github.com/Tencent/rapidjson/archive/v1.1.0.tar.gz")
set(ARCHIVE_MD5 "badd12c511e081fec6c89c43a7027bce")
set(DESTINATION_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib_GITIGNORE/rapidjson")

# NOTE : it seems that if the target directory exists, the external project is not downloaded again

ExternalProject_Add(
    my_super_dependency
    PREFIX rapidjson
    URL "${DOWNLOAD_URL}"
    URL_HASH MD5=${ARCHIVE_MD5}
    SOURCE_DIR "${DESTINATION_DIRECTORY}"
    # we only want to download the archive, thus we disable building :
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)


add_dependencies(bin my_super_dependency)  # note : this really is necessary !
target_include_directories(bin PUBLIC "${DESTINATION_DIRECTORY}/include")
