project('mypoc', ['cpp'])

zlib_dep = dependency('zlib', static: true)

exe = executable('bin', ['main.cpp', 'zcompression.cpp'], dependencies: [zlib_dep])
