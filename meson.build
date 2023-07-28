project('riscv-interpreter', 'cpp', default_options : [
  'warning_level=3',
  'cpp_std=c++20'
])

sources = []

subdir('src')

if get_option('buildtype') == 'release'
  add_project_arguments('-Ofast', '-Werror',  language : 'cpp')
endif

exe = executable('riscv-interpreter', sources)
