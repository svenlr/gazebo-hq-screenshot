# High-Res Screenshot Plugin for Gazebo
Adds a button to Gazebo client which allows you to take screenshots in higher resolution (4x) and with stronger anti-aliasing than the screenshots saved by the default screenshot button.  
![img.png](hq_button_in_gazebo.png)

## Prerequisites
- Qt, 
- Gazebo, 
- libpng++-dev, libprotobuf-dev:
```bash
sudo apt install libpng++-dev libprotobuf-dev
```

## Installation
- Download, build, environment variables:
```bash
# recursive option is important in order to download the submodule as well
git clone --recursive git@github.com:svenlr/gazebo-hq-screenshot.git
# build process
cd gazebo-hq-screenshot
mkdir -p build
# go into the build directory and add it to the GAZEBO_PLUGIN_PATH
cd build/
export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:$(pwd)
# optionally, save the path in your bashrc
echo "export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:$(pwd)" >> ~/.bashrc
# build the plugin
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j4
```
- Setup Gazebo GUI to include Plugin  
`gedit ~/.gazebo/gui.ini`  
Add the new plugin like this:
```
[geometry]
x=0
y=0

[overlay_plugins]
filenames=libgazebo_hq_screenshot.so
```

- Always make sure you start gzclient from command line so that `GAZEBO_PLUGIN_PATH` is existent and configured properly.