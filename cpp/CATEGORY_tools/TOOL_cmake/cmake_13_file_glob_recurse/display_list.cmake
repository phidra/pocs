# helper function to display a list :
function(display_list variable_name)
    message(STATUS "")
    message(STATUS "=========================================")
    message(STATUS "The list '${variable_name}' contains :")
    message(STATUS "RAW : ${${variable_name}}")
    foreach(item IN LISTS ${variable_name})
        message(STATUS "     -> ${item}")
    endforeach()
    message(STATUS "=========================================")
    message(STATUS "")
endfunction()

