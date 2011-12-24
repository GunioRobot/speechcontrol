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

#include "../../session.hpp"
#include "session.hpp"
#include "windows/managers/books.hpp"
#include "ui_session.h"

#include <training.hpp>

using namespace SpeechControl;
using SpeechControl::Windows::Managers::SessionManager;

SessionManager::SessionManager(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SessionManager)
{
    m_ui->setupUi(this);
    updateList();
}


SessionManager::~SessionManager()
{
    delete m_ui;
}

void SessionManager::updateList() {
    m_ui->listSession->clear();

    SessionList l_lst = Session::allSessions();
    Q_FOREACH(const Session* l_sessionItr, l_lst){
        QListWidgetItem* l_item = new QListWidgetItem(l_sessionItr->content()->title());
        l_item->setData(0,l_sessionItr->uuid().toString());
        m_ui->listSession->addItem(l_item);
    }
}

Session* SessionManager::session() const {
    return m_session;
}

/// @todo Implement a means of selecting @see Session objects from the manager.
Session* SessionManager::doSelectSession()
{
    SessionManager* l_win = new SessionManager;
    if (Session::allSessions().empty()){
        l_win->on_btnCreate_clicked();
    } else {
        if (l_win->exec() != QDialog::Accepted)
            return 0;
    }

    return l_win->session();
}

void SpeechControl::Windows::Managers::SessionManager::on_btnCancel_clicked()
{
    this->reject();
}

void SpeechControl::Windows::Managers::SessionManager::on_btnOk_clicked()
{
    m_session = 0;
    const QListWidgetItem* l_item = m_ui->listSession->currentItem();
    if (l_item){
        m_session = Session::obtain(QUuid(l_item->data(0).toString()));
        this->accept();
    } else
        this->reject();
}

void SpeechControl::Windows::Managers::SessionManager::on_btnCreate_clicked()
{
    m_session = 0;
    Content* l_content = BooksManager::doSelectContent();
    if (l_content)
        m_session = Session::create(l_content);

    updateList();
}
