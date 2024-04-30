Name: snmpb
Summary: Graphical SNMP MIB browser written in QT.
Version: 0.9
Release:  2pre
License: GPL2
Group: Applications/Internet

Requires(post): shared-mime-info
Requires(post): desktop-file-utils

%description
SnmpB is an SNMP (Simple Network Management Protocol) MIB
browser written in QT. It supports SNMPv1, SNMPv2c & SNMPv3.

SnmpB can browse/edit/load/add MIB files and can query SNMP
agents. It also supports agent discovery, trap events
and graph plotting.

%prep

%build
make INSTALL_PREFIX=%prefix

%install
make INSTALL_PREFIX=%buildroot%prefix install

%clean
rm -rf %buildroot

%post
update-mime-database %prefix/share/mime
update-desktop-database %prefix/share/applications

%files
%defattr(-,root,root)

%prefix/bin/snmpb
%prefix/share/applications/snmpb.desktop
%prefix/share/mime/packages/snmpb.xml
%prefix/share/icons/hicolor/scalable/apps/snmpb.svg
%prefix/share/icons/hicolor/128x128/apps/snmpb.png
%prefix/share/pixmaps/snmpb.png
%prefix/share/apps/snmpb/mibs/*
%prefix/share/apps/snmpb/pibs/*

%doc license.txt readme.txt todo.txt changelog.txt

%changelog
* Wed Nov 7 2018 Max <ulidtko@gmail.com> 0.9-2pre
- Correct postinstall script
- Abbreviate MIB/PIB lists
- Remove force-redefines for _topdir, Prefix and Packager
    (rpmbuild --define "_topdir $PWD" -bb snmpb.spec)

* Mon Jun 1 2015 Martin Jolicoeur
- Release 1.0 (TBD) 

* Sun Jun 21 2009 Martin Jolicoeur 
- Release 0.8 

* Sun Apr 13 2008 Martin Jolicoeur 
- Initial release 

