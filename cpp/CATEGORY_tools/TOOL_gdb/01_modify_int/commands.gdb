break myfunction
run
#
echo "\n\n\n==== breaking on the first call = leave the function unchanged :\n"
continue
#
echo "\n\n\n==== breaking on the second call = assign another value to the int :\n"
# note : apparenlty, we need to step into the function ?
step
whatis age
set variable age = 999999
continue
#
echo "\n\n\n==== breaking on the third call = leave the function unchanged again :\n"
continue
