project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])


# person :
person_includes = include_directories('person')
person_src = 'person/person.cpp'
person_lib = static_library(
  'person',
  person_src,
  include_directories: person_includes,
)


# group :
group_includes = include_directories('group')
group_src = 'group/group.cpp'
group_lib = static_library(
  'group',
  group_src,
  include_directories: [
    group_includes,
    person_includes,  # group doit inclure les includes de person
  ],
  link_with: person_lib,
)

# launcher :
launcher = executable(
  'launcher',
  'main.cpp',
  # LE PROBLÈME À RÉSOUDRE EST ICI : main est obligé utiliser aussi les includes de person
  # (alors qu'on voudrait qu'il ne dépende que des includes de group)
  include_directories: ['group', 'person'],
  link_with: [group_lib],
)
