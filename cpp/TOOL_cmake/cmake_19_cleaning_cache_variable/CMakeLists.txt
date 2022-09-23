cmake_minimum_required(VERSION 3.16)
project(display_cached)

# if the proper envvar is set, we set the cached value :
if (DEFINED ENV{SET_CACHED_POUET})
    message(STATUS "The envvar 'SET_CACHED_POUET' is defined -> setting the cached 'POUET'")
    set(POUET youpi CACHE STRING "Just a dummy value to better understand how cmake cache works")
endif()

# if the proper envvar is set, we unset the cached value :
if (DEFINED ENV{UNSET_CACHED_POUET})
    message(STATUS "The envvar 'UNSET_CACHED_POUET' is defined -> unsetting the cached 'POUET'")
    unset(POUET CACHE)
endif()


if (DEFINED CACHE{POUET})
    message(STATUS "")
    message(STATUS "The variable POUET is defined in cache and is '${POUET}'")
    message(STATUS "")
else()
    message(STATUS "")
    message(STATUS "The variable POUET is NOT defined in cache !")
    message(STATUS "")
endif()
