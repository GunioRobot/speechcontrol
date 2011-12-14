#/bin/bash
echo "Downloading packages...."
sudo apt-get install libqt4-dev cmake git

echo "Cleaning..."
rm spchcntrl -rvf

echo "Obtaining code..."
git clone git://github.com/spchcntrl/speechcontrol spchcntrl

echo "Building..."
cd spchcntrl
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ..

echo "Compiling..."
make

echo "Running SpeechControl"
./speechcontrol
