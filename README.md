# CAP
 Correlation Analysis Package

# Prerequisites
- Valid `PYTHIA8` installation
- Valid `root` installation built with `PYTHIA8` support
- Environment variable for `PYTHIA8` location
 ```
 export PYTHIA8=<your_pythia8_directory>
 ```
- `root` environment activated with
 ```
 source <your_root_install_path>/bin/thisroot.sh
 ```
- massive storage path (for massive storage of `root` files)
 ```
 export CAP_LARGE_DATA=<your_massive_storage_path>
 ```

# Build CAP
```
cd <your_cap_directory>
source Setup_CAP.sh
mkdir build
cd build
cmake ../src
make install
```
