/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 Jacky Alcine <jacky.alcine@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
 
#include <QFile>
#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <QMessageBox>

#include "settings.hpp"
#include "user.hpp"
#include "microphone.hpp"
#include "ui_main.h"
#include "core.hpp"
#include "windows/main.hpp"
#include "windows/bookmanager.hpp"
#include "windows/usermanager.hpp"
#include "windows/sessionmanager.hpp"
#include "wizards/micsetup/wizard.hpp"
#include "wizards/quickstart/wizard.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows;

Main::Main() : m_ui(new Ui::MainWindow) {
   m_ui->setupUi(this);
   m_ui->retranslateUi(this);

   connect(m_ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
   this->restoreGeometry(Core::instance()->getConfig("MainWindow/Geometry").toByteArray());
   this->restoreState(Core::instance()->getConfig("MainWindow/State").toByteArray());
}

void Main::tabChanged(const int& p_index){
}

Main::~Main() {
    delete m_ui;
}

void SpeechControl::Windows::Main::on_actionOptions_triggered()
{
    Settings* l_settings = new Settings(this);
    l_settings->exec();
}

void SpeechControl::Windows::Main::on_actionWizardQuickStart_triggered()
{
    Wizards::QuickStart* l_wiz = new Wizards::QuickStart(this);
    l_wiz->exec();
}

void SpeechControl::Windows::Main::on_actionWizardMic_triggered()
{
    Wizards::MicrophoneSetup* l_wiz = new Wizards::MicrophoneSetup(this);
    l_wiz->exec();
}

void SpeechControl::Windows::Main::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

/// @todo Implement About dialog using QMessageBox.
void SpeechControl::Windows::Main::on_actionAbout_SpeechControl_triggered()
{    
}

void SpeechControl::Windows::Main::on_actionSessionManage_triggered()
{
    SessionManager* l_sessWin = new SessionManager(this);
    l_sessWin->exec();

}

void SpeechControl::Windows::Main::on_actionUserManage_triggered()
{
    UserManager* l_usrWin = new UserManager(this);
    l_usrWin->exec();
}

void SpeechControl::Windows::Main::on_actionBookManage_triggered()
{
    BookManager* l_bkWin = new BookManager(this);
    l_bkWin->exec();
}
