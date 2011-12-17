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

#ifndef SESSION_HPP
#define SESSION_HPP

#include <QWidget>
#include <QAbstractItemView>
#include "training.hpp"

namespace Ui {
    class SessionWidget;
}

namespace SpeechControl {
namespace Widgets {

class SessionWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Session* SelectedSession READ selectedSession)
    Q_PROPERTY(SessionList SelectedSession READ selectedSessions)
    Q_PROPERTY(const QAbstractItemView::SelectionMode SelectionMode READ selectionMode WRITE setSelectionMode)

public:
    explicit SessionWidget(QWidget *parent = 0);
    ~SessionWidget();
    Session* selectedSession() const;
    SessionList selectedSessions() const;
    const QAbstractItemView::SelectionMode selectionMode() const;
    void setSelectionMode(const QAbstractItemView::SelectionMode& );

private slots:
    void on_comboBox_editTextChanged(const QString &arg1);

private:
    Ui::SessionWidget *ui;    
};

}}

#endif // SESSION_HPP
