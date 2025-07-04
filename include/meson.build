conf = configuration_data()
conf.set('VSFLTK_VERSION_MAJOR', major_version)
conf.set('VSFLTK_VERSION_MINOR', minor_version)
conf.set('VSFLTK_VERSION_REV', rev_version)
conf.set_quoted('VSFLTK_VERSION', major_version + '.' + minor_version + '.' + rev_version)

#TODO: Add all the other configuration flags here in place of injecting them at compile-time


cfgfile = configure_file(output : 'config.hpp',
  configuration : conf,
)

#install_headers(cfgfile, subdir:'vs-xml')
#install_subdir('.', install_dir : 'include/vs-xml', strip_directory: false, follow_symlinks: true, exclude_files: ['meson.build','config.hpp.in'] )
