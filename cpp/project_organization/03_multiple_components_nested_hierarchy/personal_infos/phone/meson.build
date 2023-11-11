phone_public_includes = include_directories('public')
phone_impl = [
  'impl/phone.cpp',
]
phone_lib = static_library(
  'phone_lib',
  phone_impl,
  include_directories: [phone_public_includes, 'impl'],
  )
phone_dep = declare_dependency(
  link_with : phone_lib,
  include_directories : phone_public_includes,
)
