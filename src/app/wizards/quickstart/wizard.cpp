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

#include "core.hpp"
#include "wizard.hpp"
#include "userinit.hpp"
#include "wizards/intro.hpp"
#include "wizards/micsetup/micselect.hpp"
#include "wizards/outro.hpp"

#include <QIcon>
#include <QVariantMap>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

QuickStart::QuickStart(QWidget *parent) :
    WizardBase(parent)
{
    this->setWindowTitle(tr("Quick Start :: SpeechControl"));
    QIcon l_icon = QIcon::fromTheme("preferences-desktop-personal");
    setWindowTitle(tr("Quick Start - SpeechControl"));
    setPixmap(QWizard::LogoPixmap,l_icon.pixmap(32,32,QIcon::Active,QIcon::On));
    setPage(QuickStart::IntroductionPage,
            new Pages::Introduction(tr("This wizard is designed to ease the process of configuring SpeechControl.")));
    setPage(QuickStart::UserCreationPage,
            new Pages::UserInitialization);
    setPage(QuickStart::MicrophoneCreationPage,
            new Pages::MicrophoneSelection);
    //setPage(QuickStart::BookAdditionPage,
    //        new Pages::BookAdditionPage);
    setPage(QuickStart::ConclusionPage,
            new Pages::Conclusion(tr("You've successfully configured SpeechControl to your liking. "
                                     "If you need to re-configure SpeechControl, feel free to run this wizard again.")));
}

/// @todo The user's country could be automatically detected by QLocale.
void QuickStart::accept() {
    Core* l_core = Core::instance();
    QVariantMap l_name;
    QVariantMap l_language;
    QString l_gender;

    l_name["First"] = field("name-first");
    l_name["Middle"] = field("name-middle");
    l_name["Last"] = field("name-last");

    l_language["Spoken"] = field("language-spoken");
    l_language["Native"] = field("language-native");

    if (field("is-gender-male").toBool())
        l_gender = "Male";
    else
        l_gender = "Female";

    l_core->setConfig("User/Name",l_name);
    l_core->setConfig("User/Gender",l_gender);
    l_core->setConfig("User/Language",l_language);

    l_core->setConfig("User/Age",property("age"));
    l_core->setConfig("User/Country",property("country"));
    l_core->setConfig("Microphone/Default",property("mic-uuid"));

    this->QDialog::accept();
}

QuickStart::~QuickStart()
{
}
