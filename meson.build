project('ntshell', 'c',
  version : '0.3.1',
  license : 'MIT',
  meson_version : '>=0.54.0',
  default_options : ['c_std=c99', 'warning_level=2'],
)

# ---------------------------------------------------------------------------
# NT-Shell core library
# ---------------------------------------------------------------------------

ntshell_inc = include_directories('lib/core', 'lib/util')

# Configuration overrides for ntconf.h. These flags are also exported
# through the dependency object, so consumers of this subproject are
# compiled with the same settings as the library itself.
ntshell_cargs = [
  '-DNTCONF_EDITOR_MAXLEN=@0@'.format(get_option('editor_maxlen')),
  '-DNTCONF_HISTORY_DEPTH=@0@'.format(get_option('history_depth')),
  '-DNTCONF_CLI_MAX_ARGS=@0@'.format(get_option('cli_max_args')),
]

ntshell_core_sources = files(
  'lib/core/ntshell.c',
  'lib/core/ntcmd.c',
  'lib/core/ntcli.c',
  'lib/core/text_editor.c',
  'lib/core/text_history.c',
  'lib/core/vtrecv.c',
  'lib/core/vtsend.c',
  'lib/core/ntlibc.c',
)

ntshell_util_sources = files(
  'lib/util/ntopt.c',
  'lib/util/ntstdio.c',
)

# Skip installation when built as a subproject; the parent project
# should not end up installing this library.
ntshell_install = not meson.is_subproject()

ntshell_lib = static_library('ntshell',
  ntshell_core_sources,
  ntshell_util_sources,
  include_directories : ntshell_inc,
  c_args : ntshell_cargs,
  install : ntshell_install,
)

# Public dependency object.
#
# Use from a parent project via the subproject fallback:
#
#   ntshell_dep = dependency('ntshell', fallback : ['ntshell', 'ntshell_dep'])
#
# or directly:
#
#   ntshell_sp  = subproject('ntshell')
#   ntshell_dep = ntshell_sp.get_variable('ntshell_dep')
ntshell_dep = declare_dependency(
  link_with : ntshell_lib,
  include_directories : ntshell_inc,
  compile_args : ntshell_cargs,
)

# Allow `dependency('ntshell')` to resolve to this subproject when it is used
# as a fallback.
meson.override_dependency('ntshell', ntshell_dep)

if ntshell_install
  # All headers use quoted includes, so they can share one directory.
  install_headers(
    'lib/core/ntshell.h',
    'lib/core/ntcli.h',
    'lib/core/ntcmd.h',
    'lib/core/ntconf.h',
    'lib/core/ntint.h',
    'lib/core/ntlibc.h',
    'lib/core/text_editor.h',
    'lib/core/text_history.h',
    'lib/core/vtrecv.h',
    'lib/core/vtsend.h',
    'lib/util/ntopt.h',
    'lib/util/ntstdio.h',
    subdir : 'ntshell',
  )

  # ntshell_t embeds buffers sized by the NTCONF_* macros, so the
  # configuration flags are part of the ABI and must be visible to
  # consumers of the installed library as well.
  pkg = import('pkgconfig')
  pkg.generate(ntshell_lib,
    name : 'ntshell',
    description : 'Natural Tiny Shell: VT100 terminal shell for embedded systems',
    subdirs : 'ntshell',
    extra_cflags : ntshell_cargs,
  )
endif

# ---------------------------------------------------------------------------
# Native (host) samples — built only for the top-level project on request
# ---------------------------------------------------------------------------

if get_option('samples')
  subdir('sample/native/ntshell')
  subdir('sample/native/ntcli')
endif
