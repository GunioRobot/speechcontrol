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

#include "books.hpp"
#include "ui_book.h"
#include "session.hpp"
#include "../../session.hpp"
#include "wizards/contents/wizard.hpp"

using SpeechControl::Content;
using SpeechControl::Windows::Managers::BooksManager;
using SpeechControl::Wizards::ContentWizard;

BooksManager::BooksManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookManager)
{
    ui->setupUi(this);
    updateList();
}

BooksManager::~BooksManager()
{
    delete ui;
}

void BooksManager::updateList(){
    ContentList l_lst = Content::allContents();
    if (!l_lst.empty()){
        Q_FOREACH(const Content* l_cnt, l_lst){
            QListWidgetItem* l_item = new QListWidgetItem(l_cnt->title(),ui->lstBooks);
            l_item->setData(0,l_cnt->uuid().toString());
            ui->lstBooks->addItem(l_item);
        }
    }

    ui->btnSelect->setEnabled(l_lst.empty());

    if (l_lst.empty()){
        ui->lblTitle->setText("No Books");
    }
    else {
        ui->lblTitle->setText("No Selection");
    }
}

void SpeechControl::Windows::Managers::BooksManager::on_btnSelect_clicked()
{
    QListWidgetItem* l_item = ui->lstBooks->currentItem();
    if (l_item){
        m_book = Content::obtain(QUuid(l_item->data(0).toString()));
        accept();
    } else {
        m_book = 0;
        reject();
    }
}

Content* SpeechControl::Windows::Managers::BooksManager::doSelectContent()
{
    BooksManager* l_wiz = new BooksManager;

    if (Content::allContents().empty()){
        l_wiz->on_btnAdd_clicked();
        if (Content::allContents().empty())
            return 0;
    } else {
        if (l_wiz->exec() == QDialog::Accepted)
            return l_wiz->m_book;
        else
            return 0;
    }

    return 0;
}

/// @todo Invoke the Book addition wizard here.
void SpeechControl::Windows::Managers::BooksManager::on_btnAdd_clicked()
{
    ContentWizard* l_wiz = new ContentWizard;

    if (l_wiz->exec() == QDialog::Accepted)
        updateList();
}

void SpeechControl::Windows::Managers::BooksManager::on_btnCancel_clicked()
{
    reject();
}

void SpeechControl::Windows::Managers::BooksManager::on_lstBooks_itemSelectionChanged()
{
    const QListWidgetItem* l_item = ui->lstBooks->currentItem();
    if (l_item){
        const QUuid l_uuid(l_item->data(0).toString());
        const Content* l_cnt = Content::obtain(l_uuid);
        ui->lblTitle->setText(l_cnt->title());
        ui->lcdWordCount->display(QString::number(l_cnt->words()));
    } else {
        ui->lblTitle->setText("No Selection");
        ui->lcdWordCount->display(0);
    }
    ui->btnSelect->setEnabled(!(!l_item));
}
