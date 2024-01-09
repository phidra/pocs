project('mypoc', 'cpp', version : '0.1', default_options : ['warning_level=3', 'cpp_std=c++20'])

gbenchmark_proj = subproject('google-benchmark')
gbenchmark_dep = gbenchmark_proj.get_variable('google_benchmark_dep')

my_test_exe = executable(
  'my_super_test',
  [files('my_test.cpp')],
  dependencies : [
    gbenchmark_dep,
    ]
  )
test('youpiya tsoin tsoin', my_test_exe)
