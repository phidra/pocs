There is 2 versions of the POC.

In one of the, cmake is configured to generate compile_commands.json.
In the other, cmake is configured NOT to generate compile_commands.json.

To see the impact on vim's ALE, you can open src/main.cpp :

WITHOUT compile_commands.json :
As an included header "lib.h" is in a "hidden" location (a folder apart from "src" directory),
vim's ALE has no way to find to find this header, and displays a warning.

WITH compile_commands.json :
The file compile_commands.json allows ALE to find the header, thus no warning is displayed.

WARNING : it seems that CMAKE_EXPORT_COMPILE_COMMANDS must be defined AFTER the "project" directive.
