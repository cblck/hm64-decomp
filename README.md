# WIP Harvest Moon 64 Decomp

## Progress

Currently, 67% of the 1467 game functions have been decompiled, and all library functions have been identified.

## Asset extraction

Sprites are now able to be extracted and converted to png files using the `extract.py` script in the `tools` directory (`make extract` or `cd tools && python3 extract.py`). Extracted assets will be put in `./assets/sprites` folder. A handful of asset locations are already listed in the `sprite_addresses.csv` file in the tools directory (see next paragraph for formatting).

There are two sprite asset formats used. Both types have a spritesheet at the beginning and an asset lookup table that follows it. Type 1 (used with character and map object sprites) includes a separate spritesheet index at the end of the asset, while type 2 (for smaller sprites, such as overlay icons) has the spritesheet index at the beginning of the actual spritesheet. Thus, the format for `sprite_addresses.csv` is spritesheet start, asset lookup table start, spritesheet index start, asset end, and asset name for type 1, and spritesheet start, asset lookup table start, asset end, and asset name for type 2.


## Setting up
1. Install [WSL2](https://learn.microsoft.com/en-us/windows/wsl/install)
1. WSL: `sudo apt-get update && sudo apt install -y python3 binutils-mips-linux-gnu gcc-mips-linux-gnu build-essential`

### WSL Workflow
1. WSL: `cd ${HOME}/projects`
1. `git clone --recursive https://github.com/cblck/hm64-decomp.git`
1. `cd hm64-decomp`
1. `sudo chmod +x tools/setup.sh`
1. `sudo tools/setup.sh`. This will fetch Splat, GCC 2.7.2, and the necessary GCC binutils.
1. Copy `baserom.us.z64` (must be in big endian/z64 format) to the project's root directory.
1. Run `make setup && make VERBOSE=1`

### Visual Studio 2022 Workflow
1. Install [VS Workload Linux Development with C++](https://learn.microsoft.com/en-us/cpp/linux/download-install-and-setup-the-linux-development-workload?view=msvc-170)
1. **Run WSL Workflow steps 1-6**
1. VS2022: `Clone a repository`: `https://github.com/cblck/hm64-decomp.git`
1. Build the hm64-decomp project which:
    1. Copies VS project files from Windows to the WSL project directory.
    1. Runs `tools/remote_setup.sh` as the remote pre-build step: `make setup`
    1. Builds the dummy vs-wrapper application.
    1. Runs `tools/remote_build.sh` as the remote post-build step: `make VERBOSE=1`