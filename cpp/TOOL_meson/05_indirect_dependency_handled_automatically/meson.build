project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])


# person :
person_includes = include_directories('person')
person_src = 'person/person.cpp'
person_lib = static_library(
  'person',
  person_src,
  include_directories: person_includes,
)

# en plus de définir la librairie, je crée un objet de type dependency :
person_dep = declare_dependency(
  link_with : person_lib,
  # par cette déclaration, le message est "tout utilisateur de la librairie 'person' va vouloir inclure ce répertoire" :
  include_directories : person_includes,
)



# group :
group_includes = include_directories('group')
group_src = 'group/group.cpp'
group_lib = static_library(
  'group',
  group_src,
  include_directories: [
    group_includes,
  ],
  dependencies: person_dep,
)
group_dep = declare_dependency(
  link_with : group_lib,
  # par cette déclaration, le message est "tout utilisateur de la librairie 'group' va vouloir inclure deux choses :
  #     - les includes de 'group' elle-même
  #     - les includes de 'person'
  # du coup, le message est : 'group' dépend de 'person' PUBLIQUEMENT !
  include_directories : [
    group_includes,
    person_includes,  # NOTE : idéalement, j'aurais préféré ici faire quelque chose comme person_dep.get_includes(), mais je ne trouve pas comment faire...
  ],
)

# launcher :
launcher = executable(
  'launcher',
  'main.cpp',
  # ici, le main n'a plus besoin que de définir sa dépendance à group (et la dépendance à person est transitive) :
  dependencies: group_dep,
)
