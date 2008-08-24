#!/usr/bin/python

from distutils.core import setup, Extension
import os

__version__ = '0.1'

macros = [('MODULE_VERSION', '"%s"' % __version__)]

lru_extension = Extension(
	name='lru',
	sources=['lru.c'],
	libraries=['glib-2.0'],
	include_dirs=["/usr/include/glib-2.0", "/usr/lib/glib-2.0/include"],
	define_macros=macros
)

setup(
	name			= 'lru',
	version			= __version__,
	author			= 'Evan Klitzke',
	author_email	= 'evan@eklitzke.org',
	description		= 'LRU lib',
	platforms		= ['Platform Independent'], # FIXME: not quite true
	ext_modules		= [lru_extension],
    #library_dirs=['/usr/local/lib'],
)
