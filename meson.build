project('riscv-interpreter', 'cpp', default_options : [
  'warning_level=3',
  'cpp_std=c++20'
])

if get_option('buildtype') == 'release'
  add_project_arguments('-Ofast', '-Werror', language : 'cpp')
endif

subdir('lib')
subdir('bin')
