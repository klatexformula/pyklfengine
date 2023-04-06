import sys
import os.path
from glob import glob
from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension


this_dir = os.path.dirname(__file__)
klfengine_src_dir = os.path.join(this_dir, 'klfengine-include')

if not os.path.exists(klfengine_src_dir):
    raise ValueError("Please download or link the klfengine sources in this folder as "
                     "‘./klfengine-include’ such that we can include "
                     "‘<./klfengine-include/klfengine/klfengine>’")


include_dirs = [ klfengine_src_dir ]

deps_install_include_dir = os.path.join(this_dir, 'deps_install', 'include')
if os.path.exists(deps_install_include_dir):
    include_dirs.append(deps_install_include_dir)

ext_modules = [
    Pybind11Extension(
        "pyklfengine",
        sorted(glob("cxx/*.cxx")),  # Sort source files for reproducibility
        include_dirs=include_dirs,
        extra_compile_args=[
            '-std=gnu++17',
            '-isysroot',
            '/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk',
        ],
    ),
]


sys.path.insert(0, this_dir)
import pyklfengine_

setup(
    name="pyklfengine",
    version=pyklfengine_.__version__,
    author="Philippe Faist",

    #packages=find_packages(),

    ext_modules=ext_modules,
)
