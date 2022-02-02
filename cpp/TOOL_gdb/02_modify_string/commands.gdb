break myfunction
run
#
# breaking on the first call = leave the function unchanged :
continue
#
# breaking on the second call = assign another string :
# note : apprenlty, we need to step into the function ?
step
call to_display.assign("====================THIS VALUE IS SET BY GDB, YAY !========================")
continue
#
# breaking on the third call = leave the function unchanged again :
continue

break myfunction
run
#
echo "\n\n\n==== breaking on the first call = leave the function unchanged :\n"
continue
#
echo "\n\n\n==== breaking on the second call = assign another value to the string :\n"
# note : apparenlty, we need to step into the function ?
step
whatis to_display
# As strings are more complex than ints, it's easier to mutate it instead of writing memory locations :
call to_display.assign("====================THIS VALUE IS SET BY GDB, YAY !========================")
continue
#
echo "\n\n\n==== breaking on the third call = leave the function unchanged again :\n"
continue
