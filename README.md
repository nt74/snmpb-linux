# snmpb-linux
SnmpB is an Open Source (GPL-2.0) desktop SNMP browser and MIB editor written in Qt5.

For more information please visit [Sourceforce SnmpB](https://sourceforge.net/projects/snmpb/)

## Building and installing for RHEL/ROCKY 9 distros

### Prerequisites
`sudo dnf install qt5-qtbase-devel qt5-qttools-devel qt5-qtsvg-devel bison flex`

`sudo ln -s /usr/lib64/qt5/bin/qmake /usr/bin/qmake`

## Installation
`git clone https://github.com/nt74/snmpb-linux.git`

`cd snmpb-linux/libsmi`

`autoreconf -i`

### Create RPM package
`cd ../installer/linux/rpm`

`sudo rpmbuild SPECS/snmpb.spec --bb --define "_topdir $(pwd)" --define "prefix /usr"`

### Install libqwt (optional)
`cd ../../../qwt`

`sudo make install`

`echo /opt/qwt-6.2.0/lib | sudo tee /etc/ld.so.conf.d/qwt.conf`

`sudo ldconfig -v`

### Install RPM package
`cd ../installer/linux/rpm/RPMS/x86_64`

`sudo rpm --nodeps -Uvh snmpb-0.9-2pre.x86_64.rpm`


## Building and installing for DEBIAN/UBUNTU distros

### Prerequisites
`sudo apt install bison flex autoconf automake libtool pbuilder qtbase5-dev qttools5-dev-tools qt5-qmake`

`sudo ln -s /usr/lib/qt5/bin/qmake /usr/bin/qmake`

### Optional packages
`sudo apt install libtomcrypt-dev libsmi2-dev`

## Installation
`git clone https://github.com/nt74/snmpb-linux.git`

`cd snmpb-linux/libsmi`

`autoreconf -i`

### Create DEB package
`cd ../installer/linux/deb/BUILD`

`debuild --no-sign`

### Install RPM package
`cd ..`

`sudo dpkg -i snmpb_1.0_amd64.deb`

### Install libqwt (mandatory)
`cd ../../../qwt`

`sudo make install`

`echo /opt/qwt-6.2.0/lib | sudo tee /etc/ld.so.conf.d/qwt.conf`

`sudo ldconfig -v`
