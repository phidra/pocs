project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])


# Les variations étudiées par rapport aux POCs précédentes :
#   - 'group' dépend maintenant de DEUX lib : dépendance PUBLIQUE vers 'person' et PRIVÉE vers 'name_modifier'
#   - 'name_modifier' utilise à la fois des includes PRIVÉS et PUBLICS (seuls ces derniers sont incluables par les utilisateurs de la lib)
#   - 'group' a à la fois des includes PRIVÉS et PUBLICS (seuls ces derniers sont incluables par les utilisateurs de la lib)
# On vérifie les impacts de tout ceci sur les commandes de compilation et de link utilisées par ninja.


# name_modifier :
name_modifier_public_includes = include_directories('name_modifier')
name_modifier_private_includes = include_directories('name_modifier_private')
name_modifier_src = ['name_modifier/name_modifier.cpp', 'name_modifier_private/add_exclamation.cpp']
name_modifier_lib = static_library(
  'name_modifier',
  name_modifier_src,
  include_directories: [name_modifier_public_includes, name_modifier_private_includes],
)
name_modifier_dep = declare_dependency(
  link_with : name_modifier_lib,
  include_directories : name_modifier_public_includes,
)


# person :
person_includes = include_directories('person')
person_src = 'person/person.cpp'
person_lib = static_library(
  'person',
  person_src,
  include_directories: person_includes,
)
person_dep = declare_dependency(
  link_with : person_lib,
  include_directories : person_includes,
)

# group :
group_public_includes = include_directories('group')
group_private_includes = include_directories('group_private')
group_src = ['group/group.cpp', 'group_private/announce.cpp']
group_lib = static_library(
  'group',
  group_src,
  include_directories: [
    group_public_includes,
    group_private_includes,
  ],
  # la dépendance de 'group' à 'person' et 'name_modifier' est visible dans la commande de compilation,
  # qui contient bien les deux includes (en plus de ceux de 'group')
  dependencies: [person_dep, name_modifier_dep],
)
group_dep = declare_dependency(
  link_with : group_lib,
  include_directories : [
    group_public_includes,
    # la présence des includes de 'person' dans l'objet dépendance est le signe que 'group' dépend de 'person' PUBLIQUEMENT
    # inversement, l'absence des includes de 'name_modifier' montre que 'name_modifier' est une dépendance de 'group' PRIVÉE
    person_includes,
  ],
)

# launcher :
launcher = executable(
  'launcher',
  'main.cpp',
  # comme c'est le launcher qui utilise l'objet dépendance 'group_dep', c'est le main qui :
  #    - aura accès aux headers de 'person' (car 'group' en dépendait PUBLIQUEMENT)
  #    - n'aura pas accès aux headers de 'name_modifier' (car 'group' en dépendait de façon PRIVÉE)
  # et c'est bien ce qu'on observe dans la commande de compilation
  dependencies: group_dep,
)
