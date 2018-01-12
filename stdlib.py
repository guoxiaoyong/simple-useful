# You may need to use setuptools.distutils depending on Python distribution.
import distutils
import glob
import os
import pkgutil
import sys

def get_python_library():
  # Get list of the loaded source modules on sys.path.
  modules = {
      module
      for _, module, package in list(pkgutil.iter_modules())
      if package is False
  }

  # Glob all the 'top_level.txt' files installed under site-packages.
  site_packages = glob.iglob(os.path.join(os.path.dirname(os.__file__)
                  + '/site-packages', '*-info', 'top_level.txt'))

  # Read the files for the import names and remove them from the modules list.
  modules -= {open(txt).read().strip() for txt in site_packages}

  # Get the system packages.
  system_modules = set(sys.builtin_module_names)

  # Get the just the top-level packages from the python install.
  python_root = distutils.sysconfig.get_python_lib(standard_lib=True)
  _, top_level_libs, _ = list(os.walk(python_root))[0]

  return sorted(top_level_libs + list(modules | system_modules))


get_python_library()
#for n in get_python_library(): print n
