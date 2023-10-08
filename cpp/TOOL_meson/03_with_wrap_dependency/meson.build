# ajout par rapport à la POC précédente = build des librairies statiques :
project('pouet', 'c', version : '0.1', default_options : ['warning_level=3', 'default_library=static'])
sdl2_dep = dependency('sdl2')  # ici, la dépendance est résolue dans les subprojects
exe = executable('pouet', 'pouet.c', install : true, dependencies : sdl2_dep)
test('basic', exe)
