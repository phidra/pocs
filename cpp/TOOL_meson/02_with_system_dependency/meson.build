project('pouet', 'c', version : '0.1', default_options : ['warning_level=3'])
gtkdep = dependency('gtk+-3.0')  # la dépendance doit être présente sur le système
exe = executable('pouet', 'pouet.c', install : true, dependencies : gtkdep)
test('basic', exe)
