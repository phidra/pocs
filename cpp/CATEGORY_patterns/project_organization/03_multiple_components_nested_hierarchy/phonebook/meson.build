phonebook_public_includes = include_directories('public')
phonebook_impl = [
  'impl/database.cpp',
  'impl/filter.cpp',
]
phonebook_lib = static_library(
  'phonebook_lib',
  phonebook_impl,
  include_directories: [phonebook_public_includes, 'impl'],
  dependencies: [person_dep, address_dep, phone_dep],
  )
phonebook_dep = declare_dependency(
  link_with : phonebook_lib,
  include_directories : [
    phonebook_public_includes,
    person_public_includes,  # the dependency of phonebook to person is PUBLIC (i.e. person is used in public API)
    address_public_includes,  # same here
    phone_public_includes,  # same here
  ],
)


# test of phonebook implementation details :
phonebook_private_test = executable(
  'test-filter',
  'tests/test_private_impl_filter.cpp',
  include_directories : 'impl',  # unit-tests have access to private implementation
  dependencies: phonebook_dep,
  )
test('phonebook private implementation', phonebook_private_test)


# CLI to use project = a phonebook of persons :
phonebook_launcher = executable(
  'cli-phonebook',
  'mains/cli.cpp',
  dependencies: phonebook_dep,
  )
