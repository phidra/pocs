# STEP1
################################################################################
message(STATUS "=========")
message(STATUS "========= STEP1> showing that we must provide at least the expected arguments, but can provide extra arguments")
message(STATUS "=========")

function(f1 ARG1 ARG2)
    message(STATUS "\t f1 called with ARG1=${ARG1} ARG2=${ARG2}")
endfunction()

message(STATUS "=== calling f1 with exactly the expected arguments :")
f1(pouet coucou)
message(STATUS "=== calling f1 with less than the expected arguments FAILS the exection (it is thus not shown)")
#f1(justpouet)
message(STATUS "=== calling f1 with far more than the expected arguments is ok :")
f1(pouet coucou youpi1 youpi2 youpi3 youpi4 youpi5)
message(STATUS "")


# STEP2
################################################################################
message(STATUS "=========")
message(STATUS "========= STEP2> showing that ARGV contains all the arguments, and ARGN contains only the extra arguments")
message(STATUS "=========")

function(f2 ARG1 ARG2)
    message(STATUS "\t f2 called with ARG1=${ARG1} ARG2=${ARG2}")

    # ARGV = all the arguments :
    list(LENGTH ARGV nb_argv)
    message(STATUS "\t in f2, ARGV = ${ARGV}  (${nb_argv} items) :")
    foreach(arg IN LISTS ARGV)
        message(STATUS "\t\t- ${arg}")
    endforeach()

    # # ARGN = only the extra (unnamed) arguments :
    list(LENGTH ARGN nb_argn)
    message(STATUS "\t in f2, ARGN = ${ARGN}  (${nb_argn} items) :")
    foreach(arg IN LISTS ARGN)
        message(STATUS "\t\t- ${arg}")
    endforeach()
endfunction()

message(STATUS "=== calling f2 with exactly the expected arguments :")
f2(pouet coucou)
message(STATUS "=== calling f2 with far more than the expected arguments is ok :")
f2(pouet coucou youpi1 youpi2 youpi3 youpi4 youpi5)
message(STATUS "=== without using cmake_parse_arguments, 'naming' arguments doesn't work as expected :")
f2(ARG1 pouet ARG2 coucou youpi1 youpi2)
message(STATUS "")


# STEP3
################################################################################
message(STATUS "=========")
message(STATUS "========= STEP3> using cmake_parse_arguments")
message(STATUS "=========")

function(f3)  # as arguments are handled by cmake_parse_arguments, it becomes useless to define them in function declaration
    message(STATUS "\t f3 called, arguments will be handled by cmake_parse_arguments")

    # for the record, we show the args :
    list(LENGTH ARGN nb_argn)
    message(STATUS "\t in f3, ARGN = ${ARGN}  (${nb_argn} items) :")
    foreach(arg IN LISTS ARGN)
        message(STATUS "\t\t- ${arg}")
    endforeach()

    # cmake_parse_arguments is configured with lists :
    # NOTE : the following syntaxs are equivalent, both create the same list :
    #    set(myvar  value1 value2 value3)
    #    set(myvar "value1;value2;value3")

    # options are arguments that can be passed without value : if the argument was passed, the option will be set to TRUE, else to FALSE
    set(option_args OPTION1 OPTION2)

    # one-value args can only store a single value :
    set(onevalue_args ARG1 ARG2)

    # multi-value args can store several values :
    set(multivalue_args MULTIARG1 MULTIARG2)

    # when parsing arguments, cmake_parse_arguments stores the args in variables prefixed by a PREFIX.
    # if args are not passed, the corresponding variable will be undefined (except options, which are set to FALSE)
    cmake_parse_arguments(MY_PREFIX "${option_args}" "${onevalue_args}" "${multivalue_args}" ${ARGN})

    message(STATUS "\tConsequently, these are the variables set by 'cmake_parse_arguments' :")
    message(STATUS "\t\t MY_PREFIX_OPTION1 = ${MY_PREFIX_OPTION1}")
    message(STATUS "\t\t MY_PREFIX_OPTION2 = ${MY_PREFIX_OPTION2}")
    if (DEFINED MY_PREFIX_ARG1)
        message(STATUS "\t\t MY_PREFIX_ARG1 = ${MY_PREFIX_ARG1}")
    else()
        message(STATUS "\t\t MY_PREFIX_ARG1 is not defined !")
    endif()
    if (DEFINED MY_PREFIX_ARG2)
        message(STATUS "\t\t MY_PREFIX_ARG2 = ${MY_PREFIX_ARG2}")
    else()
        message(STATUS "\t\t MY_PREFIX_ARG2 is not defined !")
    endif()
    if (DEFINED MY_PREFIX_MULTIARG1)
        message(STATUS "\t\t MY_PREFIX_MULTIARG1 = ${MY_PREFIX_MULTIARG1}")
    else()
        message(STATUS "\t\t MY_PREFIX_MULTIARG1 is not defined !")
    endif()
    if (DEFINED MY_PREFIX_MULTIARG2)
        message(STATUS "\t\t MY_PREFIX_MULTIARG2 = ${MY_PREFIX_MULTIARG2}")
    else()
        message(STATUS "\t\t MY_PREFIX_MULTIARG2 is not defined !")
    endif()

endfunction()

message(STATUS "=== calling f3 :")
f3(OPTION1 ARG1 pouet ARG2 coucou MULTIARG2 youpi1 youpi2 youpi3 youpi4 youpi5)
message(STATUS "")
