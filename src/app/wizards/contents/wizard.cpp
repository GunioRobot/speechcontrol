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

#include "wizard.hpp"
#include "addselect.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"

#include <QIcon>

using namespace SpeechControl::Wizards;

ContentWizard::ContentWizard(QWidget *parent) :
    WizardBase(parent)
{
    QIcon l_icon = QIcon::fromTheme("text-plain");
    setPixmap(QWizard::LogoPixmap,l_icon.pixmap(32,32,QIcon::Active,QIcon::On));
    setWindowTitle(tr("Book Addition Wizard - SpeechControl"));
    setPage(ContentWizard::IntroductionPage,
            new Pages::Introduction(tr("This wizard allows you to add more reading sources for transcription "
                                       "for SpeechControl.")));
    setPage(ContentWizard::AdditionSelectionPage,
            new Pages::AdditionSelectionPage);
    setPage(ContentWizard::ConclusionPage,
            new Pages::Conclusion(tr("You've successfully added a book to your local transcription library.")));
}

ContentWizard::~ContentWizard()
{
}
