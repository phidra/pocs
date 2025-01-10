project('mypoc', ['cpp'], default_options : ['warning_level=3', 'cpp_std=c++20', 'werror=true'])

# on utilise notre installation locale de librdkafka :
rdkafka_libs_dir = meson.current_source_dir() / 'NOGIT_install_librdkafka' / 'lib'

bin_consumer = executable(
  'bin_consumer',
  ['src/main_consumer.cpp'],
  include_directories: ['NOGIT_install_librdkafka/include'],
  dependencies: [
    meson.get_compiler('cpp').find_library('rdkafka', dirs: rdkafka_libs_dir),
    meson.get_compiler('cpp').find_library('rdkafka++', dirs: rdkafka_libs_dir),
  ],
)

bin_producer = executable(
  'bin_producer',
  ['src/main_producer.cpp'],
  include_directories: ['NOGIT_install_librdkafka/include'],
  dependencies: [
    meson.get_compiler('cpp').find_library('rdkafka', dirs: rdkafka_libs_dir),
    meson.get_compiler('cpp').find_library('rdkafka++', dirs: rdkafka_libs_dir),
  ],
)

bin_dump_all_messages = executable(
  'bin_dump_all_messages',
  ['src/main_dump_all_messages.cpp'],
  include_directories: ['NOGIT_install_librdkafka/include'],
  dependencies: [
    meson.get_compiler('cpp').find_library('rdkafka', dirs: rdkafka_libs_dir),
    meson.get_compiler('cpp').find_library('rdkafka++', dirs: rdkafka_libs_dir),
  ],
)

bin_showconf = executable(
  'bin_showconf',
  ['src/main_showconf.cpp'],
  include_directories: ['NOGIT_install_librdkafka/include'],
  dependencies: [
    meson.get_compiler('cpp').find_library('rdkafka', dirs: rdkafka_libs_dir),
    meson.get_compiler('cpp').find_library('rdkafka++', dirs: rdkafka_libs_dir),
  ],
)
