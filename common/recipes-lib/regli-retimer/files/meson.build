project('libregli-retimer', 'c',
    version: '0.3.0',
    license: 'Apache-2.0',
    default_options: ['werror=true', 'warning_level=3'],
    meson_version: '>=0.40')

headers = [
    'kb900x_addresses.h',
    'kb900x_bic_comm.h',
    'kb900x_eeprom.h',
    'kb900x_i2c_comm.h',
    'kb900x_i2c_master.h',
    'kb900x_log.h',
    'kb900x_reg_dump.h',
    'kb900x_smbus_comm.h',
    'kb900x_utils.h',
    'kb900x.h',
]

sources = [
    'kb900x.c',
    'kb900x_i2c_comm.c',
    'kb900x_smbus_comm.c',
    'kb900x_i2c_master.c',
    'kb900x_eeprom.c',
    'kb900x_utils.c',
    'kb900x_log.c',
]

rt_deps = []
compile_args = []

bic_comm = get_option('bic_communication')

if bic_comm
    message('KB900x communication: BIC (IPMB)')
    sources += 'kb900x_bic_comm.c'
    rt_deps += dependency('libbic')
    compile_args += '-DBIC_COMMUNICATION'
else
    message('KB900x communication: SMBus')
endif

install_headers(
    headers,
)

srcs = files(
  sources,
)

# Retimer library.
rt_lib = shared_library(
    'regli-retimer', srcs,
    dependencies: rt_deps,
    version: meson.project_version(),
    c_args: compile_args,
    install: true
)

# pkgconfig for Retimer library.
pkg = import('pkgconfig')
pkg.generate(libraries: [rt_lib],
    name: meson.project_name(),
    version: meson.project_version(),
    description: 'Regli retimer Library')
