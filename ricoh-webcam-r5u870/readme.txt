

sudo apt-get install build-essential libjpeg-dev imagemagick subversion libv4l-dev checkinstall 
svn co svn://svn.code.sf.net/p/mjpg-streamer/code/ mjpg-streamer 
cd mjpg-streamer/mjpg-streamer 
make
sudo make install 

VERSION=$( sed -n '/SOURCE_VERSION/s/^.*"\(.*\)".*$/\1/gp' < mjpg_streamer.h )
REVISION=$( svnversion -cn | sed "s/.*://" )
sudo checkinstall --pkgname=mjpg-streamer --pkgversion="$VERSION+$REVISION~checkinstall" --default 

mjpg_streamer  -i "input_uvc.so -d /dev/video0 -y -r 352x288 -f 10"  -o "output_http.so -p 8080 -w /usr/local/www" 
mjpg_streamer  -i "input_uvc.so -d /dev/video0 -yuv -r 352x288 -f 10"  -o "output_http.so -p 8080 -w /usr/local/www"
https://wiki.ubuntuusers.de/MJPG-Streamer/





You need to install these drivers if you are using a Ricoh R5U870 webcam component and there is no videoX device in /dev
install dependencies: sudo apt-get install libglib2.0-dev libusb-dev build-essential gcc automake mercurial
Download drivers: wget https://bitbucket.org/ahixon/r5u87x/get/default.tar.gz
Unpack: tar -zxvf default.tar.gz and rename folder to something more appropriate: mv ahixon-r5u87x-a9b2171d762b/ r5u87x/
cd r5u87x
make
sudo make install
Run: sudo r5u87x-loader --reload, If you get a successfully uploaded -message, run sudo reboot and reconnect to your raspberry pi. Your camera should now work and there should now be a video0 device in /dev. The r5u87x-loader will now run automatically on boot, so you don't need to do any of these steps again.
I haven't had success with taking any pictures with the R5U870 of higher resolution than 352x288. I would love to hear if someone gets undistorted pictures with full resolution.

https://github.com/DigitalFabricationStudio/Project_0.2/tree/master/valtteri.wikstrom/Project/frame-source
