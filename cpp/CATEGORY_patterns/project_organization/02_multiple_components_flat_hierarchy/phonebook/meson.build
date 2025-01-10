phonebook_public_includes = include_directories('public')
phonebook_impl = [
  'impl/database.cpp',
  'impl/filter.cpp',
]
phonebook_lib = static_library(
  'phonebook_lib',
  phonebook_impl,
  include_directories: [phonebook_public_includes, 'impl'],
  dependencies: person_dep,  # phonebook depends on person
  )
phonebook_dep = declare_dependency(
  link_with : phonebook_lib,
  include_directories : [
    phonebook_public_includes,
    person_public_includes,  # the dependency of phonebook to person is PUBLIC (i.e. person is used in public API)
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


# CLI to list persons alphabetically :
sorted_list_launcher = executable(
  'list-alphabetically',
  'mains/list_by_alphabetical_order.cpp',
  dependencies: phonebook_dep,
  )
