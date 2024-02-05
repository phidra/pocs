project('mypoc', ['cpp'])



#################################################################################
# STEP 1 = les trucs OTel :
#################################################################################
otel_relative_dir = 'NOGIT_DEP_OpenTelemetry' / 'installed'
otel_include_dir = otel_relative_dir / 'include'
otel_deps = []
otel_macro = ''  # par défaut, pas de #define sur WITH_OPENTELEMETRY


if get_option('compile_with_opentelemetry')
  otel_macro = '-DWITH_OPENTELEMETRY'
  message('OpenTelemetry is enabled')
  otel_absolute_dir = meson.current_source_dir() / otel_relative_dir
  otel_lib_dir = otel_absolute_dir / 'lib'
  message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)
  message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)
  message('PREREQUISITE> OpenTelemetry must be set up here : ' + otel_absolute_dir)
  cpp = meson.get_compiler('cpp')
  foreach otel_dep: [
      'opentelemetry_common',
      'opentelemetry_exporter_ostream_span',
      'opentelemetry_resources',
      'opentelemetry_trace',
    ]
    otel_deps += cpp.find_library(otel_dep, dirs: otel_lib_dir)
  endforeach
else
  message('OpenTelemetry is disabled')
endif


#################################################################################
# STEP 2 = notre app ne dépend que de l'API C++ d'instrumentation (header-only)
#################################################################################
app_lib = static_library(
  'app_lib',
  ['app/app.cpp'],
  include_directories: ['app', otel_include_dir],
  )
app_dep = declare_dependency(
  link_with : app_lib,
  include_directories : ['app'],
)


#################################################################################
# STEP 3 = l'initialiseur de télémétrie dépend du vrai SDK OpenTelemetry
#################################################################################
init_telemetry_lib = static_library(
  'init_telemetry_lib',
  ['init_telemetry/init_telemetry.cpp'],
  include_directories: ['init_telemetry', otel_include_dir],
  dependencies: otel_deps,
  cpp_args: otel_macro,
)
init_telemetry_dep = declare_dependency(
  link_with : init_telemetry_lib,
  include_directories : ['init_telemetry'],
)


#################################################################################
# STEP 4 = le binaire ne dépend pas directement de OpenTelemetry
exe = executable('bin', ['main.cpp'], dependencies: [app_dep, init_telemetry_dep])
