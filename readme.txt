SnmpB is a desktop SNMP browser and MIB editor written with Qt.

Supported features:
 + custom MIB loading & editing
 + trap reception
 + agent discovery
 + SNMPv3 USM (authentication & encryption)
 + IPv6


### Compiling ###

# make (linux, macosx, unix)
# gmake (*BSD)
> make (Windows/MSYS2)

Make variables to tune:
 - QTBIN sets the Qt bin path (used to locate qmake)
 - INSTALL_PREFIX sets the installation directory (/usr/local by default)
 - BIN_PREFIX is "bin" by default, sets binary install path suffix
 - NO_ROOT enables non-root install (off by default)

Tested & compiles on MSYS2/Windows, Linux, MacOS Leonard & Sierra, and NetBSD


### Dependencies ###

 - C++ compiler of course. g++ or clang++, both are good.
 - GNU toolchain:
    * bison & flex
    * autoconf and automake
    * GNU make
    * GNU install
    * The `patch` tool
 - Qt5: `apt install qt5-base-dev`
 - Qwt library: `apt install libqwt-qt5-dev`

Optional:

 - libtomcrypt-dev
 - libsmi2-dev


### Windows build ###

See separate guide in installer/win32/building-on-windows.md


---------------------
# External packages #

## Snmp++ http://www.agentpp.com ##
Version: 3.3.9

Modified file(s):
config_snmp_pp.h libsnmp.h config.h (new) "Hard-coded values instead of using package's autoconf"
notifyqueue.cpp "Added missing ipv6 include required for compilation on WIN32"
address.cpp "Replaced custom get_printable for IPv6 addresses by inet_ntop()"
notifyqueue.h uxsnmp.h notifyqueue.cpp uxsnmp.cpp 
"Modified snmp++ to allow binding trap port on both ipv4 and ipv6 for all interfaces"
uxsnmp.h notifyqueue.cpp "Added set/get_notify_callback_fd() to access fd when replying to INFORMS"
IPv6Utility.cpp IPv6Utility.h "Re-enabled local inet_pton unavailable on MSYS2/MINGW"
uxsnmp.h "Redefined INVALID_SOCKET to get rid of compiler warnings"


## Libtomcrypt http://libtom.org ##
Version: 1.17

Modified file(s):
makefile
src/headers/tomcrypt_pk.h "Added defined(BSD) around wchar_t definition to fix broken build on *BSD"
src/headers/tomcrypt_custom.h "Defined LTC_NO_ROLC for MacOSX using LLVM"
src/headers/tomcrypt_macros.h "Disabled ROL64/ROR64 assembly routines on Windows 64 bits"


## Libsmi http://www.ibr.cs.tu-bs.de/projects/libsmi ##
Version: 0.5.0

Modified file(s):
win/Makefile.mingw
win/config.h
lib/Makefile.am
Makefile.am     "Removed compilation of all subdirs except lib"
lib/Makefile.am "Fixed file generation rules: broke parallel build"
lib/util.c      "smiIsPath(): removed condition for "." in filename"
lib/snprintf.c  "Upgraded snprintf.c from source to support 64 bits printing on Windows"
lib/smi.c       "Fixed a bug on paths with spaces in smiReadConfig()"
lib/smi.c       "Fixed stack overwrite in smiRenderValue due to uint64 format"
lib/smi.c       "Fixed display for non-printable octet-strings -tracker 3096221"
lib/smi.c       "Allows OCTET STRING to be printable with \v, \t , \r, \n, \f"
lib/parser-smi.y "Fixed crash when defval mismatches with object type"
lib/parser-smi.y "Fixed crash when row name is NULL (JETDIRECT3-MIB)"
lib/parser-smi.y "Fixed crash on compliance statement group"

Deleted mibs/ietf/IANA-ITU-ALARM-TC-MIB: redundant with one in iana folder
(prevented proper copy in win32 installer)
Ran 'autoreconf -i' to generate Makefiles for 1.15 libtools

## QWT http://qwt.sourceforge.net ##
Version: 6.1.2

Modified file(s):
qwtconfig.pri
qwtbuild.pri "Removed silent flag, prevented build on win32"
qwt.pro

--------------------------------

Original author: Martin Jolicoeur <martsjoli@gmail.com>

Current maintainer: Max ulidtko <ulidtko@gmail.com>
