function(function_no_parent_scope)
    set(TEST1 "I am defined in the function !")
endfunction()

function(function_parent_scope)
    set(TEST2 "I am defined in the function !" PARENT_SCOPE)
endfunction()

macro(macro_no_need_of_a_parent_scope)
    set(TEST3 "I am defined in the macro !")
endmacro()

function_no_parent_scope()
function_parent_scope()
macro_no_need_of_a_parent_scope()

message(STATUS "TEST_FUNCTION_NO_PARENT_SCOPE = '${TEST1}'")
message(STATUS "TEST_FUNCTION_PARENT_SCOPE    = '${TEST2}'")
message(STATUS "TEST_MACRO                    = '${TEST3}'")
message(STATUS "")
message(STATUS "This POC shows that :")
message(STATUS "   a variable set in a MACRO is visible in its caller's scope")
message(STATUS "   a variable set in a FUNCTION is not visible in its caller's scope...unless it is set with PARENT_SCOPE")
