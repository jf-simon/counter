project('news', 'c',
        version        : '0.1',
        license        : 'BSD 2 clause',
        default_options: [ 'c_std=gnu99' ],
        meson_version  : '>= 0.40.0')
base_url = 'https://github.com/jf-simon/news'

e = dependency('enlightenment')

##### convenience variables for later
proj     = meson.project_name()
ver      = meson.project_version()
cfg      = configuration_data()

dir_module_e = e.get_pkgconfig_variable('libdir')
release = e.get_pkgconfig_variable('release')

host_os = host_machine.system()
cc = meson.get_compiler('c')

if host_os == 'linux'
   if cc.has_header_symbol('features.h', '__UCLIBC__')
      host_os = 'linux-uclibc'
   elif cc.has_header_symbol('features.h', '__dietlibc__')
      host_os = 'linux-dietlibc'
   else
      host_os = 'linux-gnu'
   endif
endif


module_arch = '@0@-@1@-@2@'.format(host_os, host_machine.cpu_family(), release)

install_dir = join_paths(dir_module_e, 'enlightenment/gadgets', module_arch, meson.project_name())

#config_h = configuration_data()
#config_h.set_quoted('MODULE_DIR', install_dir)

subdir('src')
subdir('data')

##### dir locations
dir_prefix = get_option('prefix')
dir_bin    = join_paths(dir_prefix, get_option('bindir'))
dir_lib    = join_paths(dir_prefix, get_option('libdir'))
dir_data   = join_paths(dir_prefix, get_option('datadir'))
dir_locale = join_paths(dir_prefix, get_option('localedir'))

##### config.h
cfg.set_quoted('PACKAGE'                 , proj)
cfg.set_quoted('PACKAGE_NAME'            , proj)
cfg.set_quoted('PACKAGE_VERSION'         , ver)
cfg.set_quoted('PACKAGE_STRING'          , proj + ' ' + ver)
cfg.set_quoted('PACKAGE_URL'             , base_url + proj)
cfg.set_quoted('PACKAGE_BIN_DIR'         , dir_bin)
cfg.set_quoted('PACKAGE_LIB_DIR'         , dir_lib)
cfg.set_quoted('PACKAGE_DATA_DIR'        , join_paths(dir_data, proj))
cfg.set_quoted('LOCALEDIR'               , dir_locale)
cfg.set       ('_GNU_SOURCE'             , 1)
cfg.set       ('__EXTENSIONS__'          , 1)
cfg.set       ('_POSIX_PTHREAD_SEMANTICS', 1)
cfg.set       ('_ALL_SOURCE'             , 1)
cfg.set       ('_POSIX_SOURCE'           , 1)
cfg.set       ('_POSIX_1_SOURCE'         , 1)

configure_file(output: 'config.h', configuration: cfg)

desktop_data = configuration_data()
        desktop_data.set('GADGET_DIR', install_dir)
        desktop_data.set('VERSION', meson.project_version())

configure_file(input: 'news.desktop.in',
        output: 'news.desktop',
        install: true,
        install_dir: install_dir,
        configuration: desktop_data)
