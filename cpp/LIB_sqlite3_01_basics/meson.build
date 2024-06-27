project('mypoc', ['cpp'], default_options : ['warning_level=3', 'cpp_std=c++20', 'werror=true'])

# dépendance gérée avec meson wrap :
#   mkdir subprojects
#   meson wrap install sqlite3
sqlite3_dep = dependency('sqlite3')

exe = executable(
  'bin',
  ['main.cpp'],
  dependencies: [
    sqlite3_dep,
  ],
)
