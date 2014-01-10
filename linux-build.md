#Script and/or instructions for building Julia Studio on Linux 64.  Developed for Ubuntu 12.04.3 and 13.10.3 (so far).  YMMV on other platforms.
#This script is a work in progress.  The Qt installation currently requires some GUI action.

startpath=`pwd`

#update your system like a good penguin
sudo apt-get update
sudo apt-get --yes --force-yes upgrade

#get install julia
sudo add-apt-repository --yes ppa:staticfloat/juliareleases
sudo add-apt-repository --yes ppa:staticfloat/julia-deps
sudo apt-get update
sudo apt-get --yes --force-yes install julia 

#move into master dir
mkdir julia-studio
cd julia-studio

#install build prerequisites
sudo apt-get --yes --force-yes install vim mesa-common-dev build-essential wget
#12.04
sudo apt-get --yes --force-yes install vim mesa-common-dev build-essential wget
sudo apt-get --yes --force-yes install libglu1-mesa-dev

#13.10
sudo apt-get --yes --force-yes install vim libgl1-nvidia-glx mesa-commonlibglu1-mesa-dev 



#get specific version of qt
#64-bit
wget http://download.qt-project.org/archive/qt/5.1/5.1.0/qt-linux-opensource-5.1.0-x86_64-offline.run
#32-bit
#wget http://download.qt-project.org/archive/qt/5.1/5.1.0/qt-linux-opensource-5.1.0-x86-offline.run 
chmod +x qt-linux-opensource-5.1.0-x86_64-offline.run 
./qt-linux-opensource-5.1.0-x86_64-offline.run 

#the qt installer has a GUI...can't seem to get around that...
exit

## Set up Qt

#get path to qt cc_64
cd Qt5.1.0/5.1.0/gcc_64
qtbase=`pwd`

##if multiple qts are present, this let's us choose wiht out changing path
#sudo mkdir /etc/xdg/qtchooser
#echo "$qtbase/bin" >  default.conf
#echo "$qtbase/lib" >> default.conf
#sudo mv default.conf /etc/xdg/qtchooser/

##if not Qt install just append to path
export PATH=$PATH:$qtbase/bin

#get julia studio source
cd $startpath/julia-studio

##for folks who want authenticated access to repo for pushing, forking, etc...
	##if you haven't already, set yourself up for
	#ssh-key -t rsa -C "sthompson@forio.com"
	#ssh-keygen -t rsa -C "sthompson@forio.com"
	#cat .ssh/id_rsa.pub 
	##assign this key to your github account
#git clone git@github.com:forio/julia-studio.git

#anon clone
git clone https://github.com/forio/julia-studio.git

qmake -r
make

#cleanup for packaging
cd bin/
ln -s /usr/bin/julia-basic julia-basic
mv bin/JuliaStudio bin/julia-studio 
