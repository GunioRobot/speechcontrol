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

#include "userinit.hpp"
#include "ui_userinit.h"

#include <QDebug>
#include <QLocale>
#include <QVariantMap>

using SpeechControl::Wizards::Pages::UserInitialization;

UserInitialization::UserInitialization(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::UserInitialization)
{
    ui->setupUi(this);
    registerField("name-first",ui->lineEditNameFirst);
    registerField("name-last",ui->lineEditNameLast);
    registerField("name-middle",ui->lineEditNameFirst);
    registerField("age",ui->comboBoxAge);
    registerField("country",ui->comboBoxCountry);
    registerField("language-spoken",ui->comboBoxLanguageSpoken);
    registerField("language-native",ui->comboBoxLanguageNative);
    registerField("is-gender-male",ui->radioButtonMale);
    registerField("is-gender-female",ui->radioButtonFemale);
}

UserInitialization::~UserInitialization()
{
    delete ui;
}

bool SpeechControl::Wizards::Pages::UserInitialization::validatePage()
{
    if (ui->lineEditNameFirst->text().isEmpty()){
        this->setSubTitle("<b><font color='red'>Please enter your first name.</font></b>");
        return false;
    }

    if (ui->lineEditNameLast->text().isEmpty()){
        this->setSubTitle("<b><font color='red'>Please enter at least the first letter of your last name.</font></b>");
        return false;
    }

    if (!ui->radioButtonFemale->isChecked() && !ui->radioButtonMale->isChecked()){
        this->setSubTitle("<b><font color='red'>Please select your gender.</font></b>");
        return false;
    }

    return true;
}

/// @todo Load all of the countries.
void SpeechControl::Wizards::Pages::UserInitialization::initializePage()
{
    ui->comboBoxCountry->clear();
    QLocale l_sys = QLocale::system();
    QList<QLocale::Country> l_countries = QLocale::countriesForLanguage(l_sys.language());
    Q_FOREACH(const QLocale::Country& l_country, l_countries){
        ui->comboBoxCountry->addItem(QLocale::countryToString(l_country));
    }

    updateLanguages();
}

/// @todo Find a way to obtain languages from the system.
void SpeechControl::Wizards::Pages::UserInitialization::updateLanguages()
{
    QLocale l_locale;
}

void SpeechControl::Wizards::Pages::UserInitialization::clearPage()
{
}
