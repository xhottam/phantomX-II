
To install

sudo cp -rf ps3joy /etc/init.d/
sudo chmod  755 /etc/init.d/ps3joy
sudo update-rc.d ps3joy defaults 99 99

To uninstall

sudo update-rc.d -f ps3joy remove
