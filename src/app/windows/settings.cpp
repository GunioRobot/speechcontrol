/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#include <QMessageBox>

#include "core.hpp"
#include "settings.hpp"
#include "training.hpp"
#include "sessions/session.hpp"
#include "wizards/quickstart/wizard.hpp"
#include "wizards/micsetup/wizard.hpp"
#include "wizards/contents/wizard.hpp"
#include "wizards/session-create/wizard.hpp"
#include "ui_settings.h"

using namespace SpeechControl;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows;

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    for (int i = 0; i < ui->tabWidgetWizards->count(); ++i)
        on_tabWidgetWizards_currentChanged(i);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_tabWidgetWizards_currentChanged(int p_index)
{
    switch (p_index){
    case 0: // configuration
    {
        ui->checkBoxSysStart->setChecked(Core::instance()->getConfig("Options/AutoStart",QVariant(false)).toBool());
        ui->checkBoxDictate->setChecked(Core::instance()->getConfig("Options/Dictation",QVariant(false)).toBool());
        ui->checkBoxDesktopControl->setChecked(Core::instance()->getConfig("Options/Control",QVariant(false)).toBool());
        ui->checkBoxVoxForge->setChecked(Core::instance()->getConfig("VoxForge/EnableUploading",QVariant(false)).toBool());
    }
        break;
    case 1: // plug-ins
    {
    }
        break;
    case 2: // books
    {
        ContentList l_books = Content::allContents();
        ui->textEditPreview->setEnabled(false);
        ui->listWidgetBooks->clear();
        Q_FOREACH(const Content* l_cnt, l_books){
            QListWidgetItem* l_item = new QListWidgetItem;
            l_item->setText(l_cnt->title());
            l_item->setData(Qt::UserRole,l_cnt->uuid().toString());
            ui->listWidgetBooks->addItem(l_item);
        }

        if (!l_books.empty())
            ui->listWidgetBooks->setCurrentRow(0);
    }
        break;
    case 3: // wizards
    {
    }
        break;
    }

}

void Settings::on_buttonBox_accepted()
{
    Core::instance()->setConfig("Options/AutoStart",ui->checkBoxSysStart->checkState());
    Core::instance()->setConfig("Options/Dictation",ui->checkBoxDictate->checkState());
    Core::instance()->setConfig("Options/Control",ui->checkBoxDesktopControl->checkState());
    Core::instance()->setConfig("VoxForge/EnableUploading",ui->checkBoxVoxForge->checkState());
}

void Settings::on_pushButtonWizardConfig_clicked()
{
    QuickStart* l_wiz = new QuickStart(this);
    l_wiz->open();
}

void Settings::on_pushButtonWizardSessions_clicked()
{
    SessionCreate* l_wiz = new SessionCreate(this);
    l_wiz->open();
}

void Settings::on_pushButtonWizardVoxforge_clicked()
{
    QMessageBox::information(this,tr("Not Yet Implemented"),tr("This functionality doesn't yet exist."));
}

void Settings::on_pushButtonWizardMic_clicked()
{
    MicrophoneSetup* l_wiz = new MicrophoneSetup(this);
    l_wiz->open();
}

void Settings::on_pushButtonAdd_clicked()
{
    ContentWizard* l_wiz = new ContentWizard;
    l_wiz->open();

    if (l_wiz->result() == QDialog::Accepted)
        on_tabWidgetWizards_currentChanged(2);
}

void Settings::on_pushButtonTrain_clicked()
{
    QListWidgetItem* l_item = ui->listWidgetBooks->currentItem();
    Content* l_cnt = Content::obtain(l_item->data(Qt::UserRole).toString());
    Session* l_sess = Session::create(l_cnt);
    Training::startTraining(l_sess);
}

/// @todo Delete the selected book.
void Settings::on_pushButtonDelete_clicked()
{
    QListWidgetItem* l_item = ui->listWidgetBooks->currentItem();
    Content* l_cnt = Content::obtain(l_item->data(Qt::UserRole).toString());
    l_cnt->deleteLater();
}

void Settings::on_listWidgetBooks_itemSelectionChanged()
{
    QListWidgetItem* l_item = ui->listWidgetBooks->currentItem();
    if (l_item){
        Content* l_cnt = Content::obtain(l_item->data(Qt::UserRole).toString());
        ui->pushButtonTrain->setEnabled(true);
        ui->pushButtonDelete->setEnabled(true);
        ui->textEditPreview->setEnabled(true);
        ui->textEditPreview->setPlainText(l_cnt->pageAt(0));
    } else {
        ui->textEditPreview->clear();
        ui->textEditPreview->setEnabled(false);
    }
}
