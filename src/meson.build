inc = include_directories('.', '../..', '../')

e = dependency('enlightenment')

src = [
  'counter.h',
  'main.c',
  'config.c'
]

executable('counter',
  src,
  dependencies: e,
  install_dir: install_dir,
  install: true,
  include_directories: inc
)
