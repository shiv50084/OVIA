**ModernPNG:**
=============
* ModernPNG is a project written in modern C, that contains a library libModernPNG, and a utility, *ModernPNG, for reading and writing PNG files.
* ModernPNG is 64 bit native, compatible with C++ (through the C API).

**Features:**
==========
* libModernPNG supports reading regular PNGs with all core chunks, plus the sTER Stereographic 3D chunk, as well as the animation chunks from APNG.
* libModernPNG **DOES NOT SUPPORT** Interlacing, or encoding, though both features will eventually be added.

**How to get ModernPNG:**

* Recursively clone the git repo with `git clone --recursive https://github.com/bumblebritches57/ModernPNG.git` to get the BitIO submodule.
* To compile ModernPNG, use either the Xcode or Visual Studio projects in the Projects folder, or cd to the root of the project and simply type `make`, it will build the library and utility in release form with optimizations for you.
* To install ModernPNG type `make install`, the default installation path is `/usr/local/Packages/ModernPNG`; to change that, edit the makefiles `InstallLocation` variable.

