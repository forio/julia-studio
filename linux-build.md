#Building Julia Studio on Ubuntu Linux

Script and/or instructions for building Julia Studio on Linux 64.  Developed for Ubuntu 12.04.3.  YMMV on other platforms. The Qt 5.1.0 installation currently requires some GUI action so cannot be fully automated.  


First, update your system.
```bash
#update your system like a good penguin
sudo apt-get update
sudo apt-get --yes --force-yes upgrade
```

Next, install Julia--the most recent because we are bleeding edge that way
```bash
#get install julia
sudo add-apt-repository --yes ppa:staticfloat/juliareleases
sudo add-apt-repository --yes ppa:staticfloat/julia-deps
sudo apt-get update
sudo apt-get --yes --force-yes install julia 
```

We need some other requiements to build Julia Studio.  These may be platform dependant. 
```bash
#install build prerequisites
sudo apt-get --yes --force-yes install vim mesa-common-dev build-essential wget
#12.04
sudo apt-get --yes --force-yes install libglu1-mesa-dev

#13.10
#sudo apt-get --yes --force-yes install vim libgl1-nvidia-glx mesa-commonlibglu1-mesa-dev 
```

Julia Studio is currently based on Qt 5.1.  There has been some weirdness getting the PPAs, so let's just download directly and install:
```bash
#get specific version of qt
#64-bit
wget http://download.qt-project.org/archive/qt/5.1/5.1.0/qt-linux-opensource-5.1.0-x86_64-offline.run
#32-bit
#wget http://download.qt-project.org/archive/qt/5.1/5.1.0/qt-linux-opensource-5.1.0-x86-offline.run 
chmod +x qt-linux-opensource-5.1.0-x86_64-offline.run 
./qt-linux-opensource-5.1.0-x86_64-offline.run 
```
...run through the installation with default settings, although there is no reason to run Qt Creator.

If you do have Qt installed, this will force Qt to use.  Don't do this if you don't need/want to.  It will impact other Qt-based environments.
```bash
##if multiple qts are present, this let's us choose without changing path
sudo mkdir /etc/xdg/qtchooser
echo "$qtbase/bin" >  default.conf
echo "$qtbase/lib" >> default.conf
sudo mv default.conf /etc/xdg/qtchooser/default.conf /etc/xdg/qtchooser/default.conf.bu 
sudo mv default.conf /etc/xdg/qtchooser/
/etc/xdg/qtchooser/default.conf

```

Now we need to get our build environment to use this version of Qt.  (Note: only tested on Qt-less systems so far.)
```bash
cd Qt5.1.0/5.1.0/gcc_64
qtbase=`pwd`
export PATH=$PATH:$qtbase/bin
cd ../../../
```

Time for Julia Studio. Clone from github and then build.  
```bash
#anon clone
git clone https://github.com/forio/julia-studio.git
cd julia-studio
qmake -r
make
```

Julia Studio looks for a link to the Julia binary in its bin directory, along side the Julia Studio binary.  The below will create a symbolic link to the PPA installed Julia binary.  Feel free to point this at another version of Julia if desired.  

```bash
cd bin/
ln -s /usr/bin/julia-basic julia-basic
```

#Presto!  You can start Julia Studio now.

This document is a work in progress and subject to change as we develope Julia Studio.  Please create an issue if you run into a problem.  We'll get back to you pronto.

# To Do
 * move Qt install to PPA once Qt is updated to newer version that is accessible from a PPA
 * move creation of julia-basic symbolic link to make file
 * determine with authority what mesa packages are required for what version
