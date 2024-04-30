## Building SnmpB on Windows ##

SnmpB builds on windows using MSYS2. http://msys2.org

1. Install MSYS2

2. Start the MSYS2 MinGW 64-bit (mingw64_shell.bat)

3. Update MSYS2 and fetch latest package info:
      pacman -Syuu
   Restart MSYS2 when it asks for that, then update again.

4. Install build-deps of SnmpB:
      pacman -S \
        git \
        automake1.16 \
        autoconf \
        bison \
        make \
        mingw-w64-x86_64-jasper \
        mingw-w64-x86_64-qt5-static \
        mingw-w64-x86_64-nsis

5. Obtain SnmpB source:
      mkdir -p src; cd src
      git clone git://git.code.sf.net/p/snmpb/code snmpb
      # or:
      # git clone https://git.code.sf.net/p/snmp/code snmpb
      # git clone http://git.code.sf.net/p/snmp/code snmpb
      # or -- just download snapshot from the web, and unpack
      cd snmpb

6. Start compile:
      make

7. Build installer:
      makensis -V4 installer/win32/snmpb.nsi


### Development dependencies ###

These are not strictly needed for a successfull build, but essentially required
for changing the source should you need that.

    pacman -S mingw-w64-x86_64-qt-creator
