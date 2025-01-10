project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])

gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')

app_src = files('my_code.cpp')

tests_src = files('my_test.cpp')

my_test_exe = executable(
  'my_super_test',
  [app_src, tests_src],
  dependencies : [
    gtest_dep,
    gmock_dep,  # pas utilisé ici, mais peut servir pour de vrais tests
    ]
  )
test('youpiya tsoin tsoin', my_test_exe)
