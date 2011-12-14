#/bin/bash
echo "Downloading packages...."
sudo apt-get install libqt4-dev cmake

echo "Cleaning..."
rm spchcntrl -rvf

echo "Obtaining code..."
wget -c https://github.com/spchcntrl/speechcontrol/tarball/master
tar -xvf master

echo "Building..."
cd spchcntrl*
mkdir build -p
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ..

echo "Compiling..."
make

echo "Running SpeechControl"
./speechcontrol
