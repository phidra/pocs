project('mypoc', ['cpp'])

aws_sdk_libs_dir = '/aws_sdk_installed/lib'
aws_sdk_include_dir = '/aws_sdk_installed/include'

cpp = meson.get_compiler('cpp')
exe = executable(
  'bin',
  [
    'main.cpp',
    's3.cpp',
  ],
  dependencies: [
    cpp.find_library('libaws-cpp-sdk-core', dirs: aws_sdk_libs_dir),
    cpp.find_library('libaws-cpp-sdk-s3', dirs: aws_sdk_libs_dir),
  ],
  include_directories: [aws_sdk_include_dir],
)
