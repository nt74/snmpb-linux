/*
    Copyright (C) 2004-2011 Martin Jolicoeur (snmpb1@gmail.com) 

    This file is part of the SnmpB project 
    (http://sourceforge.net/projects/snmpb)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <memory>
#include <qmessagebox.h> 
#include <qfileinfo.h>
#include <qtextstream.h>

// For DEFAULT_SMIPATH
#ifdef WIN32
#include "../libsmi/win/config.h"
#else
#include "../libsmi/config.h"
#endif

#include "mibmodule.h"
#include "preferences.h"

const char default_mib_config[] = R"(
IF-MIB
RFC1213-MIB
SNMP-FRAMEWORK-MIB
SNMP-NOTIFICATION-MIB
SNMPv2-MIB
SNMPv2-TM
SNMP-VIEW-BASED-ACM-MIB
)";

#define STANDARD_TRAP_PORT       162


Preferences::Preferences(Snmpb *snmpb)
{
    s = snmpb;

    // setup defaults for the default constructor of QSettings
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setOrganizationDomain("snmpb.sourceforge.net");
    QCoreApplication::setApplicationName("SnmpB");

    // read early settings necessary to decide when to drop root
    QSettings settings;
    settings.beginGroup("network");
    enableipv4 = settings.value("enableipv4", true).toBool();
    trapport4 = settings.value("trapport4", 162).toInt();
    enableipv6 = settings.value("enableipv6", true).toBool();
    trapport6 = settings.value("trapport6", 162).toInt();
}

void Preferences::Init(void)
{
    p = new Ui_Preferences();
    pw = new QDialog(s->MainUI()->TabW);
    p->setupUi(pw);

    // Set some properties for the Preferences TreeView
    p->PreferencesTree->header()->hide();
    p->PreferencesTree->setSortingEnabled( false );
    p->PreferencesTree->header()->setSortIndicatorShown( false );
    p->PreferencesTree->setLineWidth( 2 );
    p->PreferencesTree->setAllColumnsShowFocus( false );
    p->PreferencesTree->setFrameShape(QFrame::WinPanel);
    p->PreferencesTree->setFrameShadow(QFrame::Plain);
    p->PreferencesTree->setRootIsDecorated( true );

    transport = new QTreeWidgetItem(p->PreferencesTree);
    transport->setText(0, tr("Transport"));
    mibtree = new QTreeWidgetItem(p->PreferencesTree);
    mibtree->setText(0, tr("MIB Tree"));
    modules = new QTreeWidgetItem(p->PreferencesTree);
    modules->setText(0, tr("Modules"));
    traps = new QTreeWidgetItem(p->PreferencesTree);
    traps->setText(0, tr("Traps"));

    connect( p->PreferencesTree, 
             SIGNAL( currentItemChanged( QTreeWidgetItem *, QTreeWidgetItem * ) ),
             this, SLOT( SelectedPreferences( QTreeWidgetItem *, QTreeWidgetItem * ) ) );
    connect( p->HorizontalSplit, SIGNAL( toggled(bool) ),
             this, SLOT( SetHorizontalSplit(bool) ) );
    connect( p->TrapPort, SIGNAL( valueChanged( int ) ), 
             this, SLOT ( SetTrapPort() ) );
    connect( p->TrapPort6, SIGNAL( valueChanged( int ) ), 
             this, SLOT ( SetTrapPort6() ) );
    connect( p->EnableIPv4, SIGNAL( toggled(bool) ),
             this, SLOT( SetEnableIPv4(bool) ) );
    connect( p->EnableIPv6, SIGNAL( toggled(bool) ),
             this, SLOT( SetEnableIPv6(bool) ) );
    connect( p->ExpandTrapBinding, SIGNAL( toggled(bool) ),
             this, SLOT( SetExpandTrapBinding(bool) ) );
    connect( p->MibLoadingEnable, SIGNAL( toggled(bool) ),
             this, SLOT( SelectAutomaticLoading() ) );
    connect( p->MibLoadingEnablePrompt, SIGNAL( toggled(bool) ),
             this, SLOT( SelectAutomaticLoading() ) );
    connect( p->MibLoadingDisable, SIGNAL( toggled(bool) ),
             this, SLOT( SelectAutomaticLoading() ) );
    connect( p->ShowAgentName, SIGNAL( toggled(bool) ),
             this, SLOT( SetShowAgentName(bool) ) );
    connect( p->ModulePathsReset, 
             SIGNAL( clicked() ), this, SLOT( MibPathReset() ));
    connect( p->ModulePathsAdd, 
             SIGNAL( clicked() ), this, SLOT( MibPathAdd() ));
    connect( p->ModulePathsDelete, 
             SIGNAL( clicked() ), this, SLOT( MibPathDelete() ));

    // Load preferences from the user-scope INI file
    QSettings settings;
    horizontalsplit = settings.value("ui/horizontalsplit", false).toBool();
    p->HorizontalSplit->setChecked(horizontalsplit);

    p->EnableIPv4->setChecked(enableipv4);
    p->EnableIPv6->setChecked(enableipv6);

    expandtrapbinding = settings.value("ui/expandtrapbinding", true).toBool();
    p->ExpandTrapBinding->setChecked(expandtrapbinding);

    showagentname = settings.value("misc/showagentname", false).toBool();
    p->ShowAgentName->setChecked(showagentname);

    automaticloading = settings.value("misc/automaticloading", 2).toInt();
    if (automaticloading == 1) p->MibLoadingEnable->setChecked(true);
    else if (automaticloading == 2) p->MibLoadingEnablePrompt->setChecked(true);
    else if (automaticloading == 3) p->MibLoadingDisable->setChecked(true);

    curprofile = settings.value("ui/selectedprofile", "localhost").toString();
    curproto = settings.value("ui/selectedproto", 0).toInt();

    // reset to default MIB paths if needed
    if (settings.value("mibpaths/size", 0) == 0) {
        MibPathReset();
    }

    MibPathRefresh();

    // reset to default MIB preload list if needed
    if (settings.value("mibpreloads/size", 0) == 0) {
        MibPreloadsReset();
    }

    p->PreferencesTree->setCurrentItem(p->PreferencesTree->topLevelItem(0));
}

void Preferences::Execute (void)
{
    if(pw->exec() != QDialog::Accepted)
    {
        MibPathRefresh();
        return;
    }

    QSettings settings;
    // Warn if trap port or transport changed ...
    if (trapport4 != settings.value("trapport", 162).toInt() ||
        trapport6 != settings.value("trapport6", 162).toInt() ||
        enableipv4 != settings.value("enableipv4", true).toBool() ||
        enableipv6 != settings.value("enableipv6", true).toBool() )
        QMessageBox::information(NULL,
                                 tr("Restart needed"),
                                 tr("SnmpB transport protocol or trap port has changed.\n"
                                    "Please restart SnmpB for the changes to take effect."),
                                 QMessageBox::Ok);

    Save();
}

void Preferences::Save()
{
    QSettings settings;

    // Save preferences
    settings.setValue("network/trapport4", trapport4);
    settings.setValue("network/trapport6", trapport6);
    settings.setValue("network/enableipv4", enableipv4);
    settings.setValue("network/enableipv6", enableipv6);

    settings.setValue("ui/horizontalsplit", horizontalsplit);
    settings.setValue("ui/expandtrapbinding", expandtrapbinding);
    settings.setValue("misc/showagentname", showagentname);
    settings.setValue("misc/automaticloading", automaticloading);

    // Store MIB paths in local list
    QStringList mibpaths;
    QList<QListWidgetItem *> l = p->ModulePaths->findItems("*", Qt::MatchWildcard);
    for (int i = 0; i < l.size(); i++)
        mibpaths << l[i]->text();

    // Then save MIB paths to config
    settings.beginWriteArray("mibpaths");
    for (int i = 0; i < mibpaths.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("dir", mibpaths[i]);
    }
    settings.endArray();

    // Save MIB preload list
    QStringList preloads = s->MibModuleObj()->GetWantedModules();
    settings.beginWriteArray("mibpreloads");
    for (int i = 0; i < preloads.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("mib", preloads[i]);
    }
    settings.endArray();

    // Refresh the MIB lists
    s->MibModuleObj()->Refresh();

    // Refresh UI
    s->TabSelected();
}

void Preferences::SaveWindowGeometry(const QMainWindow& mw)
{
    QSettings settings;
    settings.beginGroup("mainwindow");
    settings.setValue("size", mw.size());
    settings.setValue("pos", mw.pos());
}

void Preferences::RestoreWindowGeometry(QMainWindow & mw)
{
    QSettings settings;
    settings.beginGroup("mainwindow");
    if (settings.contains("size"))
        mw.resize(settings.value("size").toSize());
    if (settings.contains("pos"))
        mw.move(settings.value("pos").toPoint());
}

void Preferences::MibPathAdd()
{
    QListWidgetItem *item = new QListWidgetItem(tr("type new path here"), p->ModulePaths);
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    p->ModulePaths->addItem(item);
    p->ModulePaths->editItem(item);
}

void Preferences::MibPathDelete()
{
    QList<QListWidgetItem *> todel  = p->ModulePaths->selectedItems();
    QList<QListWidgetItem *> total = p->ModulePaths->findItems("*", Qt::MatchWildcard);

    // Protection
    if (todel.size() >= total.size())
    {
        QMessageBox::warning(NULL, tr("SnmpB warning"),
                             tr("Must have at least one defined path. Delete failed."),
                             QMessageBox::Ok);
        return;
    }

    for (int i = 0; i < todel.size(); i++)
        delete p->ModulePaths->takeItem(p->ModulePaths->row(todel[i]));
}

void Preferences::MibPathRefresh()
{
    // discard widget contents and refill it with mib preloads from settings
    p->ModulePaths->clear();
    QSettings settings;
    int size = settings.beginReadArray("mibpaths");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        p->ModulePaths->addItem(settings.value("dir").toString());
    }

    // set all editable
    QList<QListWidgetItem *> l = p->ModulePaths->findItems("*", Qt::MatchWildcard);
    for (int i = 0; i < l.size(); i++)
        l[i]->setFlags(l[i]->flags() | Qt::ItemIsEditable);
}

void Preferences::MibPathReset()
{
    // "Reset to default" for MIB paths
    QStringList defaultpaths = QString(DEFAULT_SMIPATH).split(SMI_PATH_SEPARATOR);

    QSettings settings;
    settings.beginWriteArray("mibpaths");
    for (int i = 0; i < defaultpaths.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("dir", defaultpaths[i]);
    }
    settings.endArray();

    s->MibModuleObj()->RescanPath();
}

void Preferences::MibPreloadsReset()
{
    // "Reset to default" for Wanted MIB list
    QStringList preloaddefaults = QString(default_mib_config).split('\n');
    preloaddefaults.removeAll("");

    QSettings settings;
    settings.beginWriteArray("mibpreloads");
    for (int i = 0; i < preloaddefaults.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("mib", preloaddefaults[i]);
    }
    settings.endArray();

    s->MibModuleObj()->Refresh();
}

void Preferences::SetHorizontalSplit(bool checked)
{
    horizontalsplit = checked;
    s->MainUI()->QuerySplitter->setOrientation(checked ? Qt::Vertical : Qt::Horizontal);
}

void Preferences::SetTrapPort()
{
    trapport4 = p->TrapPort->value();
}

void Preferences::SetTrapPort6()
{
    trapport6 = p->TrapPort6->value();
}

void Preferences::SetEnableIPv4(bool checked)
{
    if (!checked && !enableipv6)
    {
        QMessageBox::critical(NULL, tr("SnmpB error"),
                              tr("Must enable at least one transport protocol."),
                              QMessageBox::Ok);
        p->EnableIPv4->setCheckState(Qt::Checked);
        return;
    }

    enableipv4 = checked;
}

void Preferences::SetEnableIPv6(bool checked)
{
    if (!checked && !enableipv4)
    {
        QMessageBox::critical(NULL, tr("SnmpB error"),
                              tr("Must enable at least one transport protocol."),
                              QMessageBox::Ok);
        p->EnableIPv6->setCheckState(Qt::Checked);
        return;
    }

    enableipv6 = checked;
}

void Preferences::SetExpandTrapBinding(bool checked)
{
    expandtrapbinding = checked;
}

void Preferences::SetShowAgentName(bool checked)
{
    showagentname = checked;
}

void Preferences::SelectAutomaticLoading()
{
    if (p->MibLoadingEnable->isChecked()) automaticloading = 1;
    else if (p->MibLoadingEnablePrompt->isChecked()) automaticloading = 2;
    else if (p->MibLoadingDisable->isChecked()) automaticloading = 3;
}

bool Preferences::GetExpandTrapBinding(void)
{
    return expandtrapbinding;
}

bool Preferences::GetShowAgentName(void)
{
    return showagentname;
}

bool Preferences::GetEnableIPv4()
{
    return enableipv4;
}

bool Preferences::GetEnableIPv6()
{
    return enableipv6;
}

int Preferences::GetAutomaticLoading(void)
{
    return automaticloading;
}

int Preferences::GetTrapPort4(void)
{
    return trapport4;
}

int Preferences::GetTrapPort6(void)
{
    return trapport6;
}

bool Preferences::ShouldListenStdTrapPort4()
{
    return enableipv4 && trapport4 == STANDARD_TRAP_PORT;
}

bool Preferences::ShouldListenStdTrapPort6()
{
    return enableipv6 && trapport6 == STANDARD_TRAP_PORT;
}

void Preferences::SaveCurrentProfile(QString &name, int proto)
{
    curprofile = name;
    curproto = proto;
    QSettings settings;
    settings.setValue("ui/selectedprofile", curprofile);
    settings.setValue("ui/selectedproto", curproto);
}

int Preferences::GetCurrentProfile(QString &name)
{
    name = curprofile;
    return curproto;
}

void Preferences::SelectedPreferences(QTreeWidgetItem * item, QTreeWidgetItem *)
{
    if (item == transport)
    {
        p->PreferencesProps->setCurrentIndex(0);

        p->EnableIPv4->setCheckState(enableipv4==true?Qt::Checked:Qt::Unchecked);
        p->EnableIPv6->setCheckState(enableipv6==true?Qt::Checked:Qt::Unchecked);
    }
    else
    if (item == mibtree)
    {
        p->PreferencesProps->setCurrentIndex(1);

        p->HorizontalSplit->setCheckState(horizontalsplit==true?Qt::Checked:Qt::Unchecked);
        if (automaticloading == 1) p->MibLoadingEnable->setChecked(true);
        else if (automaticloading == 2) p->MibLoadingEnablePrompt->setChecked(true);
        else if (automaticloading == 3) p->MibLoadingDisable->setChecked(true);
    }
    else
    if (item == modules)
    {
        p->PreferencesProps->setCurrentIndex(2);

        MibPathRefresh();
    }
    else
    if (item == traps)
    {
        p->PreferencesProps->setCurrentIndex(3);

        p->TrapPort->setValue(trapport4);
        p->TrapPort6->setValue(trapport6);
        p->ExpandTrapBinding->setCheckState(expandtrapbinding==true?Qt::Checked:Qt::Unchecked);
        p->ShowAgentName->setCheckState(showagentname==true?Qt::Checked:Qt::Unchecked);
    }
}

