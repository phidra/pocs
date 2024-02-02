project('mypoc', ['cpp'])


# NDM : c'est un peu pourri... meson refuse les répertoires d'includes absolus s'ils font partie du projet.
# MAIS dans le même temps, on doit passer un répertoire absolu à find_library...
# Du coup, je dois définir deux fois le chemin vers la dépendance OTel : en relatif et en absolu.
# Pas critique pour cette POC
otel_relative_dir = 'NOGIT_DEP_OpenTelemetry' / 'installed'
otel_absolute_dir = meson.current_source_dir() / otel_relative_dir
otel_include_dir = otel_relative_dir / 'include'
otel_lib_dir = otel_absolute_dir / 'lib'

message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)
message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)
message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)
message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)

cpp = meson.get_compiler('cpp')

# NOTE : opentelemetry se présente sous la forme de PLUSIEURS librairies.
# Celles nécessaires à la présente POC sont listées ci-dessous.
# Les autres, qui sont disponibles mais non-nécessaires à cette POC, sont :
#     opentelemetry_exporter_in_memory
#     opentelemetry_exporter_ostream_logs
#     opentelemetry_exporter_ostream_metrics
#     opentelemetry_logs
#     opentelemetry_metrics
#     opentelemetry_version
# Du point de vue du build meson, l'enjeu de cette POC est double :
#     ENJEU 1 = utiliser une librairie qui a été installée dans un chemin custom
#     ENJEU 2 = utiliser de façon simple plusieurs librairies
# J'ai testé plusieurs façons de faire.


# # FAÇON 1 = définir manuellement les options de compilation :
# otel_dep = declare_dependency(
#   link_args : [
#     '-L' + otel_lib_dir,
#     '-lopentelemetry_common',
#     '-lopentelemetry_exporter_ostream_span',
#     '-lopentelemetry_resources',
#     '-lopentelemetry_trace',
#   ],
#   include_directories : otel_include_dir,
# )
# exe = executable('bin', ['main.cpp'], dependencies : [otel_dep])

# # FAÇON 2 = utiliser find_library pour chaque librairie nécessaire :
# otel_dep1 = cpp.find_library('opentelemetry_common', dirs: otel_lib_dir)
# otel_dep2 = cpp.find_library('opentelemetry_exporter_ostream_span', dirs: otel_lib_dir)
# otel_dep3 = cpp.find_library('opentelemetry_resources', dirs: otel_lib_dir)
# otel_dep4 = cpp.find_library('opentelemetry_trace', dirs: otel_lib_dir)
# exe = executable('bin', ['main.cpp'], dependencies : [otel_dep1, otel_dep2, otel_dep3, otel_dep4],
#   include_directories : otel_include_dir,
# )

# FAÇON 3 = idem, mais avec foreach + array (la plus propre) :
otel_deps = []
foreach otel_dep: [
  'opentelemetry_common',
  'opentelemetry_exporter_ostream_span',
  'opentelemetry_resources',
  'opentelemetry_trace',
  ]
  otel_deps += cpp.find_library(otel_dep, dirs: otel_lib_dir)
  endforeach
exe = executable('bin', ['main.cpp'], dependencies : otel_deps,
  include_directories : otel_include_dir,
)
