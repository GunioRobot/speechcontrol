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
#include "ui_main.h"
#include "core.hpp"
#include <QVariant>
#include <QMainWindow>
#include <QMessageBox>

using namespace SpeechControl::Windows;
Main::Main() : m_ui(new Ui::MainWindow) {
   m_ui->setupUi(this);
   m_ui->retranslateUi(this);

   buildMenus();
   connect(m_ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
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
    m_ui->progressBarVolume->setEnabled(active);
    m_ui->progressBarAccurate->setEnabled(active);
}

void Main::buildMenus()
{
}

void Main::loadUser(const User &p_user)
{
}
