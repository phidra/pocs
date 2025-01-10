project('mypoc', ['cpp'], default_options : ['warning_level=3', 'cpp_std=c++20', 'werror=true'])


#################################################################################
# la librairie du repo :
#################################################################################
repo_lib = static_library(
  'repo',
  ['repo/repo.cpp'],
  include_directories: ['repo'],
)
repo_dep = declare_dependency(
  link_with : repo_lib,
  include_directories : ['repo'],
)

#################################################################################
# la librairie de mon app SANS PIMPL:
#################################################################################
app_without_pimpl_lib = static_library(
  'myapp_without_pimpl',
  ['WITHOUT_PIMPL/myapp.cpp'],
  dependencies : [repo_dep],
  include_directories: [
    'WITHOUT_PIMPL',
    'repo',
    ],
)
app_without_pimpl_dep = declare_dependency(
  link_with : [app_without_pimpl_lib],
  include_directories : [
    'WITHOUT_PIMPL',
    # IMPORTANT : sans PIMPL, tout utilisateur de l'app devra inclure le repo
    # dit autrement : l'app dépend PUBLIQUEMENT du repo
    'repo',
  ],
)

#################################################################################
# la librairie de mon app AVEC PIMPL:
#################################################################################
app_with_pimpl_lib = static_library(
  'myapp_with_pimpl',
  ['WITH_PIMPL/myapp.cpp'],
  dependencies : [repo_dep],
  include_directories: [
    'WITH_PIMPL',
    'repo',
    ],
)
app_with_pimpl_dep = declare_dependency(
  link_with : app_with_pimpl_lib,
  include_directories : [
    'WITH_PIMPL',
    # IMPORTANT : avec PIMPL, un utilisateur de l'app n'a PAS connaissance du repo
    # dit autrement : l'app dépend du repo DE FAÇON PRIVÉE
    # ici, ça se traduit par l'absence d'include de 'repo'
  ],
)

#################################################################################
## l'exécutable qui utilise l'app sans PIMPL :
#################################################################################
exe_WITHOUT_PIMPL = executable(
  'bin_WITHOUT_PIMPL',
  'main.cpp',
  dependencies: [
    # comme l'app dépend publiquement du repo, le main DÉPEND du repo !
    # (même si ça ne se voit pas ici)
    app_without_pimpl_dep,
  ],
)

#################################################################################
## l'exécutable qui utilise l'app avec PIMPL :
#################################################################################
exe_WITH_PIMPL = executable(
  'bin_WITH_PIMPL',
  'main.cpp',
  dependencies: [
    # comme l'app dépend du repo de façon privée, le main NE DÉPEND PAS du repo !
    # (même si ça ne se voit pas ici)
    app_with_pimpl_dep,
  ],
)
