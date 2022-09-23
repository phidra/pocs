cmake_minimum_required(VERSION 3.0.2)
project(poc CXX)

# helper function to iterate over list :
function(iterate_list list_name)
    message(STATUS "")
    message(STATUS "----------------------------------------")
    message(STATUS "ELEMENTS OF LIST : ${list_name}")
    foreach(fruit ${${list_name}})
        message(STATUS "    ${fruit}")
    endforeach()
    message(STATUS "----------------------------------------")
    message(STATUS "")
endfunction()

# A list is a semi-colon separated string.
# Thus, the following lists are identical :
set(list1 pomme poire abricot)
set(list2 "pomme;poire;abricot")  # this string is actually considered a list !

if(list1 STREQUAL list2)
    message(STATUS "=================> lists are EQUAL !")
    message(STATUS ${list1})
    message(STATUS ${list2})
else()
    message(STATUS "=================> lists are DIFFERENT !")
    message(STATUS ${list1})
    message(STATUS ${list2})
endif()

iterate_list("list1")
iterate_list("list2")

add_executable(bin src/main.cpp)
