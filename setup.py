from distutils.core import setup, Extension

setup(name = "atmctrl",
    version = "1.0",
    description = "atm controller",
    author = "kyoungje",
    author_email = "kyoungje@gmail.com",
    url = "",
    ext_modules = [Extension("atmctrl", ["atmctrlmodule.c"])]
    )
