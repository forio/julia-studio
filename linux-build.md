# Building Julia Studio on Ubuntu Linux

Script and/or instructions for building Julia Studio on Linux 64. Developed
for Ubuntu 12.04.3. YMMV on other platforms. The Qt 5.1.1 installation
currently requires some GUI action so cannot be fully automated.

First, update your system.

```bash
# Update your system like a good penguin
sudo apt-get update
sudo apt-get --yes --force-yes upgrade
```

Next, install Julia--the most recent because we are bleeding edge that way

```bash
# Get install julia
sudo add-apt-repository --yes ppa:staticfloat/juliareleases
sudo add-apt-repository --yes ppa:staticfloat/julia-deps
sudo apt-get update
sudo apt-get --yes --force-yes install julia
```

We need some other requiements to build Julia Studio. These may be platform dependant.

```bash
# Install build prerequisites
sudo apt-get --yes --force-yes install vim mesa-common-dev build-essential wget libglu1-mesa-dev
```

Julia Studio is currently based on Qt 5.1.1 There has been some weirdness
getting the PPAs, so let's just download directly and install:

```bash
# Get specific version of qt

# 64-bit
wget http://download.qt-project.org/archive/qt/5.1/5.1.1/qt-linux-opensource-5.1.1-x86_64-offline.run

# 32-bit
wget http://download.qt-project.org/archive/qt/5.1/5.1.1/qt-linux-opensource-5.1.1-x86-offline.run

chmod +x qt-linux-opensource-5.1.1-x86_64-offline.run
./qt-linux-opensource-5.1.1-x86_64-offline.run
```
...run through the installation with default settings, although there is no reason to run Qt Creator.

If you do have Qt installed, this will force Qt to use. Don't do this if you
don't need/want to. It will impact other Qt-based environments.

```bash
# 64-bit
cd Qt5.1.1/5.1.1/gcc_64

# 32-bit
cd Qt5.1.1/5.1.1/gcc

sudo mkdir /etc/xdg/qtchooser
sudo sh -c 'echo `pwd`/bin > /etc/xdg/qtchooser/default.conf'
sudo sh -c 'echo `pwd`/lib >> /etc/xdg/qtchooser/default.conf'

cd -
```

Time for Julia Studio. The fastest way to get Julia Studio is to download the
compressed master branch without the git history.

```bash
wget https://github.com/forio/julia-studio/archive/master.zip
unzip master.zip
cd julia-studio-master
mkdir build
qmake -r CONFIG+=debug_and_release ./julia-studio.pro

# Build!
make release
make install INSTALL_ROOT=`pwd`/julia-studio-linux-x86_64

# If debug symbols are needed:
make debug
```

Julia Studio looks for a link to the Julia binary in its bin directory, along
side the Julia Studio binary. The below will create a symbolic link to the PPA
installed Julia binary. Feel free to point this at another version of Julia if
desired.

```bash
cd bin/
ln -s /usr/bin/julia-basic julia-basic
```

# Presto!  You can start Julia Studio now.

This document is a work in progress and subject to change as we develope Julia
Studio.  Please create an issue if you run into a problem. We'll get back to
you pronto.

# Package for distribution

Continuing from after the `make install` command above, we'll use the
`deploy.sh` script to create a self-contained Julia Studio directory that
includes the Qt 5.1 libraries.

```bash
./scripts/deploy.sh build/julia-studio-linux-x86_64
```

# To Do
 * move Qt install to PPA once Qt is updated to newer version that is accessible from a PPA
 * move creation of julia-basic symbolic link to make file
 * determine with authority what mesa packages are required for what version
