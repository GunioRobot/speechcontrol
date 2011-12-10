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
 
#include "windows_main.hpp"
#include "user.hpp"
#include "microphone.hpp"
#include "ui_main.h"
#include "core.hpp"
#include <QMenu>
#include <QUuid>
#include <QVariant>
#include <QMainWindow>
#include <QMessageBox>

using namespace SpeechControl::Windows;
Main::Main() : m_ui(new Ui::MainWindow) {
   m_ui->setupUi(this);
   m_ui->retranslateUi(this);

   buildMenus();
   toggleTraining(!User::hasAny());
   connect(m_ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
   connect(m_ui->pushButtonBackup,SIGNAL(clicked()),this,SLOT(doMakeBackup()));
   connect(m_ui->pushButtonBooks,SIGNAL(clicked()),this,SLOT(doSwitchBooks()));
   connect(m_ui->pushButtonUsers,SIGNAL(clicked()),this,SLOT(doSwitchUsers()));
}

void Main::on_radioButtonDictation_toggled(bool checked) {
    Core::instance()->setConfiguration("Options/Dictation",checked);
}

void Main::tabChanged(const int& p_index){
    switch (p_index){
        case 0:
            m_ui->checkBoxAutoStart->setChecked(Core::instance()->getConfiguration("Options/AutoStart",QVariant(false)).toBool());
            m_ui->radioButtonDictation->setChecked(Core::instance()->getConfiguration("Options/Dictation",QVariant(false)).toBool());
            m_ui->radioButtonControl->setChecked(Core::instance()->getConfiguration("Options/Control",QVariant(false)).toBool());
        break;
    }
}

Main::~Main() {
    delete m_ui;
}

void Main::on_radioButtonControl_toggled(bool checked)
{
    Core::instance()->setConfiguration("Options/Control",checked);
}

void Main::on_checkBoxAutoStart_toggled(bool checked)
{
    Core::instance()->setConfiguration("Options/AutoStart",checked);
}

void Main::toggleTraining(bool active)
{
    m_ui->tabTraining->setVisible(active);
    m_ui->tabTraining->setEnabled(active);
    m_ui->progressBarVolume->setEnabled(active);
    m_ui->progressBarAccurate->setEnabled(active);
}

void Main::buildMenus()
{
    QMenu* l_menuBooks = new QMenu(this);
    QMenu* l_menuBackup = new QMenu(this);
    QMenu* l_menuUsers = new QMenu(this);
    QMenu* l_menuDictionary = new QMenu(this);

    l_menuBooks->addAction(QIcon::fromTheme("document-open"),"Open..",this,SLOT(doSwitchBooks(void)),QKeySequence::Open);
    l_menuBooks->addAction(QIcon::fromTheme("document-save"),"Save",this,SLOT(doSaveProgress(void)),QKeySequence::Save);
    l_menuBooks->addAction(QIcon::fromTheme("document-save-as"),"Save As..",this,SLOT(doSaveProgressAs(void)),QKeySequence::SaveAs);

    l_menuBackup->addAction(QIcon::fromTheme("archive-extract"),"Restore",this,SLOT(doRestoreBackup(void)));
    l_menuBackup->addAction(QIcon::fromTheme("archive-insert"),"Backup",this,SLOT(doMakeBackup(void)));
    l_menuBackup->addSeparator();
    l_menuBackup->addAction(QIcon::fromTheme("document-preview-archive"),"Manager..",this,SLOT(doShowBackups(void)));

    l_menuUsers->addAction(QIcon::fromTheme("system-switch-user"),"Switch..",this,SLOT(doSwitchUsers(void)));
    l_menuUsers->addSeparator();
    l_menuUsers->addAction(QIcon::fromTheme("user-group-properties"),"Manage Users",this,SLOT(doCreateUserWizard(void)));

    l_menuDictionary->addAction(QIcon::fromTheme("list-add"),"Add Words",this,SLOT(doAddWord(void)));
    l_menuDictionary->addAction(QIcon::fromTheme("list-remove"),"Remove Words",this,SLOT(doRemoveWord(void)));
    l_menuDictionary->addSeparator();
    l_menuDictionary->addAction(QIcon::fromTheme("view-list-details"),"View Dictionaries",this,SLOT(doViewWord(void)));

    m_ui->pushButtonBooks->setMenu(l_menuBooks);
    m_ui->pushButtonBackup->setMenu(l_menuBackup);
    m_ui->pushButtonDicts->setMenu(l_menuDictionary);
    m_ui->pushButtonUsers->setMenu(l_menuUsers);
}

void Main::doSwitchBooks() {

}
void Main::doSaveProgress() {

}
void Main::doSaveProgressAs() {

}
void Main::doRestoreBackup() { }
void Main::doMakeBackup() { }
void Main::doShowBackups() { }
void Main::doSwitchUsers() { }
void Main::doCreateUserWizard() { }
void Main::doAddWord() { }
void Main::doRemoveWord() { }
void Main::doViewWord() { }

void Main::loadUser(const User &p_user)
{
}

void Main::on_pushButtonTestMic_clicked()
{
    const int l_micIndx = m_ui->comboBoxMicrophones->currentIndex();
    const QString l_micUuid = m_ui->comboBoxMicrophones->itemData(l_micIndx).toString();
    Microphone* l_mic = Microphone::getMicrophone(QUuid(l_micUuid));
    l_mic->test();
}
