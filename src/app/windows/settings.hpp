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


#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QDialog>

namespace Ui {
    class Settings;
}

namespace SpeechControl {
namespace Windows {


class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_tabWidgetWizards_currentChanged(int index);
    void on_buttonBox_accepted();
    void on_pushButtonWizardConfig_clicked();
    void on_pushButtonWizardSessions_clicked();
    void on_pushButtonWizardVoxforge_clicked();
    void on_pushButtonWizardMic_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonTrain_clicked();
    void on_pushButtonDelete_clicked();

    void on_listWidgetBooks_itemSelectionChanged();

private:
    Ui::Settings *ui;
};

}}

#endif // SETTINGS_HPP
