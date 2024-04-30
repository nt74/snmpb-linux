SOURCES = \
	../snmp++/src/address.cpp \
	../snmp++/src/asn1.cpp \
	../snmp++/src/auth_priv.cpp \
	../snmp++/src/counter.cpp \
	../snmp++/src/ctr64.cpp \
	../snmp++/src/eventlist.cpp \
	../snmp++/src/eventlistholder.cpp \
	../snmp++/src/gauge.cpp \
	../snmp++/src/idea.cpp \
	../snmp++/src/integer.cpp \
	../snmp++/src/log.cpp \
	../snmp++/src/md5c.cpp \
	../snmp++/src/mp_v3.cpp \
	../snmp++/src/msec.cpp \
	../snmp++/src/msgqueue.cpp \
	../snmp++/src/notifyqueue.cpp \
	../snmp++/src/octet.cpp \
	../snmp++/src/oid.cpp \
	../snmp++/src/pdu.cpp \
	../snmp++/src/reentrant.cpp \
	../snmp++/src/sha.cpp \
	../snmp++/src/snmpmsg.cpp \
	../snmp++/src/target.cpp \
	../snmp++/src/timetick.cpp \
	../snmp++/src/usm_v3.cpp \
	../snmp++/src/uxsnmp.cpp \
	../snmp++/src/v3.cpp \
	../snmp++/src/vb.cpp \
	../snmp++/src/IPv6Utility.cpp \
	agent.cpp \
	agentprofile.cpp \
	comboboxes.cpp \
	discovery.cpp \
	graph.cpp \
	logsnmpb.cpp \
	markerwidget.cpp \
	mibeditor.cpp \
	mibhighlighter.cpp \
	mibmodule.cpp \
	mibnode.cpp \
	mibselection.cpp \
	mibtextedit.cpp \
	mibview.cpp \
	preferences.cpp \
	snmpb.cpp \
	trap.cpp \
	usmprofile.cpp \
	main.cpp

HEADERS = \
	agent.h \
	agentprofile.h \
	comboboxes.h \
	discovery.h \
	graph.h \
	logsnmpb.h \
	markerwidget.h \
	mibeditor.h \
	mibhighlighter.h \
	mibmodule.h \
	mibnode.h \
	mibselection.h \
	mibtextedit.h \
	mibview.h \
	preferences.h \
	snmpb.h \
	snmpbapp.h \
	trap.h \
	usmprofile.h

FORMS += \
	agentprofile.ui \
	find.ui \
	gotoline.ui \
	mainw.ui \
	plot.ui \
	preferences.ui \
	replace.ui \
	usmprofile.ui \
	varbinds.ui

INCLUDEPATH += \
	../snmp++/include \
	../snmp++/ \
	../libtomcrypt/src/headers \
	../libsmi/lib \
	../qwt/src
LIBS	+= \
	-L../libtomcrypt \
	-L../libsmi/lib/.libs \
	-L../qwt/lib \
	-lsmi -ltomcrypt -lqwt

RESOURCES	= snmpb.qrc

TEMPLATE	= app
CONFIG	+= qt warn_on debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
win32 {
  CONFIG += release
  RC_FILE = snmpb.rc
  LIBS	+= -lws2_32 -L../libsmi/win
  QTPLUGIN += qsvg
}
macx:ICON = images/snmpb.icns

#-- i18n --#

TRANSLATIONS = \
     l10n/snmpb.uk_UA.ts

QMAKE_LRELEASE_FLAGS = -removeidentical -compress

#-- this works in Qt 5.12+
#CONFIG += lrelease embed_translations

#-- below is a "roll your own" workaround
isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt5 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}
updateqm.input = TRANSLATIONS
updateqm.output = .qm/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} $$QMAKE_LRELEASE_FLAGS -qm ${QMAKE_FILE_OUT}
updateqm.CONFIG += no_link target_predeps
QMAKE_EXTRA_COMPILERS += updateqm

for (translation, TRANSLATIONS) {
    translation = $$basename(translation)
    QM_FILES += $$OUT_PWD/.qm/$$replace(translation, \\.ts$, .qm)
}
embedqm.files = $$QM_FILES
embedqm.base = $$OUT_PWD/.qm
embedqm.prefix = i18n
embedqm.depends = updateqm
RESOURCES += embedqm
