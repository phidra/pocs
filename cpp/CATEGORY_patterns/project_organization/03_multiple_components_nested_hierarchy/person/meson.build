person_public_includes = include_directories('public')
person_impl = [
  'impl/person.cpp',
  'impl/age_checker.cpp',
]
person_lib = static_library(
  'person_lib',
  person_impl,
  include_directories: [person_public_includes, 'impl'],
  )
person_dep = declare_dependency(
  link_with : person_lib,
  include_directories : person_public_includes,
)


# test of person implementation details :
person_private_test = executable(
  'test-age-checker',
  'tests/test_private_impl_age_checker.cpp',
  include_directories : 'impl',  # unit-tests have access to private implementation
  dependencies: person_dep,
  )
test('person private implementation', person_private_test)


# CLI to get a person's status :
get_person_status_launcher = executable(
  'get-person-status',
  'mains/get_person_status.cpp',
  dependencies: person_dep,
  )
