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

#include <QUuid>
#include <training.hpp>

#include "session.hpp"
#include "ui_sessionwidget.h"

using namespace SpeechControl;
using SpeechControl::Widgets::SessionWidget;

SessionWidget::SessionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SessionWidget)
{
    ui->setupUi(this);
    ui->listWidget->clear();

    SessionList l_sessions = Session::allSessions();
    if (!l_sessions.isEmpty()){
        Q_FOREACH(const Session* l_session, l_sessions){
            QListWidgetItem* l_itm = new QListWidgetItem(l_session->uuid(),ui->listWidget);
            l_itm->setData(0,l_session->uuid().toString());
            ui->listWidget->addItem(l_itm);
        }
    }
}

SessionWidget::~SessionWidget()
{
    delete ui;
}

Session* SessionWidget::selectedSession() const
{
    QListWidgetItem* l_itm = ui->listWidget->currentItem();
    if (l_itm)
        return Session::obtain(QUuid(l_itm->data(0).toString()));
    return 0;
}

SessionList SessionWidget::selectedSessions() const
{
    QList<QListWidgetItem*> l_lstItem = ui->listWidget->selectedItems();
    SessionList l_lst;
    if (!l_lstItem.isEmpty()){
        Q_FOREACH(const QListWidgetItem* l_itm, l_lstItem){
            l_lst << Session::obtain(QUuid(l_itm->data(0).toString()));
        }
    }

    return l_lst;
}

const QAbstractItemView::SelectionMode SessionWidget::selectionMode() const
{
    return ui->listWidget->selectionMode();
}

void SessionWidget::setSelectionMode(const QAbstractItemView::SelectionMode &l_mode)
{
    ui->listWidget->setSelectionMode(l_mode);
}

/// @todo Find a way to hide elements that "don't" contain this phrase.
void SpeechControl::Widgets::SessionWidget::on_comboBox_editTextChanged(const QString &arg1)
{

}
