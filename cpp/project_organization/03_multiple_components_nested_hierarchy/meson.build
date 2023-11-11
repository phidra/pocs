project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])


subdir('person')
subdir('personal_infos')
subdir('phonebook')  # phonebook CAN use all things that were previously defined in person's sub-meson.build


# test of person's public API :
person_public_api_test = executable(
  'test-person',
  'tests/test_person_public_api.cpp',
  # note : this test has NO access to person's private implementation
  dependencies: person_dep,
  )
test('person public API', person_public_api_test)


# test of phonebook's public API :
phonebook_public_api_test = executable(
  'test-phonebook',
  'tests/test_phonebook_public_api.cpp',
  # note : this test has NO access to phonebook's private implementation
  dependencies: phonebook_dep,
  )
test('phonebook public API', phonebook_public_api_test)



# test of personal_infos/address's public API :
address_public_api_test = executable(
  'test-address',
  'tests/test_address_public_api.cpp',
  dependencies: address_dep,
  )
test('address public API', address_public_api_test)
