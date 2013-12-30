#script and/or instructions for building Julia Studio on Linux 64.  Developed on Ubuntu 12.04.3 and 13.10.3.  YMMV on other platforms.
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
sudo apt-get --yes --force-yes install vim libgl1-nvidia-glx mesa-common-dev mesa-commonlibglu1-mesa-dev build-essential 



#get specific version of qt
curl http://master.qt-project.org/archive/qt/5.1/5.1.0/qt-linux-opensource-5.1.0-x86_64-offline.run > qt-linux-opensource-5.1.0-x86_64-offline.run
chmod +x qt-linux-opensource-5.1.0-x86_64-offline.run 
./qt-linux-opensource-5.1.0-x86_64-offline.run 

#set up qt
sudo mkdir /etc/xdg/qtchooser
#get path to qt cc_64
cd Qt5.1.0/5.1.0/gcc_64
qtbase=`pwd`

sudo echo "$qtbase/bin" >  /etc/xdg/qtchooser/default.conf
sudo echo "$qtbase/lib" >> /etc/xdg/qtchooser/default.conf


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
