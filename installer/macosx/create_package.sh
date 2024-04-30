#!/bin/bash -eu
#
# Script used to build an SnmpB MAC installer.
# SnmpB needs to be compiled first, this script assumes
# that the application is already built.
#

VERSION=1.0
APPNAME=SnmpB
APFILESPATH=${APPNAME}.app/Contents/MacOS

# the script takes no arguments (which could be relative paths)
# so we can canonicalize PWD to "here" where the script lives
cd "$(dirname "$0")"

log() { echo "$*"; }

# Cleanup
rm -rf ${APPNAME}.app

# Copy application binary in local folder
cp -R ../../app/snmpb.app ./${APPNAME}.app
mv -f ${APFILESPATH}/snmpb ${APFILESPATH}/${APPNAME}

# Piece together Info.plist
{
    cat ../../app/snmpb.app/Contents/Info.plist \
        | sed -e "s:<string>snmpb</string>:<string>$APPNAME</string>:g" \
        | grep -v "/dict" | grep -v "/plist"
    cat fileassoc.txt
    echo "</dict>"
    echo "</plist>"
} > ${APFILESPATH}/Info.plist

# Copy preinstalled mib files
mkdir -p ${APFILESPATH}/{mibs,pibs}
cp -R ../../libsmi/mibs/{iana,ietf,tubs}/* ${APFILESPATH}/mibs/
cp -R ../../libsmi/pibs/{ietf,tubs}/* ${APFILESPATH}/pibs/
rm -f ${APFILESPATH}/{mibs,pibs}/Makefile* ${APFILESPATH}/{mibs,pibs}/*-orig
for i in ${APFILESPATH}/mibs/*; do mv $i $i.mib; done
for i in ${APFILESPATH}/pibs/*; do mv $i $i.pib; done

# Copy license file in local resource folder
mkdir -p ${APPNAME}.app/Contents/Resources/en.lproj
cp ../../license.txt ${APPNAME}.app/Contents/Resources/en.lproj/License

# Create a file-system image (.dmg) with the installer
macdeployqt ${APPNAME}.app
./create-dmg --window-size 500 300 --icon-size 96 --background background.png --volname "SnmpB" --app-drop-link 380 170 --volicon ${APPNAME}.app/Contents/Resources/snmpb.icns --icon "SnmpB" 110 170 ${APPNAME}-${VERSION}.dmg ${APPNAME}.app
