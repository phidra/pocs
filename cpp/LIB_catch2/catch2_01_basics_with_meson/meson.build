project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])

catch2_proj = subproject('catch2')
catch2_dep = catch2_proj.get_variable('catch2_with_main_dep')

app_src = files('my_code.cpp')

tests_src = files('my_test.cpp')

my_test_exe = executable(
  'my_super_test',
  [app_src, tests_src],
  dependencies : [
    catch2_dep,
    ]
  )
test('youpiya tsoin tsoin', my_test_exe)
