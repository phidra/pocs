address_public_includes = include_directories('public')
address_impl = [
  'impl/address.cpp',
]
address_lib = static_library(
  'address_lib',
  address_impl,
  include_directories: [address_public_includes, 'impl'],
  )
address_dep = declare_dependency(
  link_with : address_lib,
  include_directories : address_public_includes,
)
