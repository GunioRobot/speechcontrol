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

#include "wizard.hpp"
#include "addselect.hpp"
#include "fileselect.hpp"
#include "wikipage.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"
#include "sessions/session.hpp"


#include <QIcon>
#include <QVariant>

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

ContentWizard::ContentWizard(QWidget *parent) :
    WizardBase(parent)
{
    QIcon l_icon = QIcon::fromTheme("text-plain");
    setPixmap(QWizard::LogoPixmap,l_icon.pixmap(32,32,QIcon::Active,QIcon::On));
    setWindowTitle(tr("Book Addition Wizard - SpeechControl"));
    setPage(ContentWizard::IntroductionPage,
            new Pages::IntroductionPage(tr("This wizard allows you to add more reading sources for transcription "
                                       "for SpeechControl.")));
    setPage(ContentWizard::AdditionSelectionPage,
            new Pages::AdditionSelectionPage);
    setPage(ContentWizard::FileSelectionPage,
            new Pages::FileSelectionPage);
    setPage(ContentWizard::WikiSourcePage,
            new Pages::WikiSourcePage);
    setPage(ContentWizard::ConclusionPage,
            new Pages::Conclusion(tr("You've successfully added a book to your local transcription library.")));
}

int ContentWizard::nextId() const {
    switch (currentId()){
        case IntroductionPage:
            return AdditionSelectionPage;
        break;

        case AdditionSelectionPage:
            if (field("selection.wiki").toBool())
                return WikiSourcePage;
            else
                return FileSelectionPage;
        break;

        case WikiSourcePage:
            return ConclusionPage;
        break;

        case FileSelectionPage:
            Content::create(field("file.author").toString(),
                            field("file.title").toString(),
                            field("file.content").toString());
            return ConclusionPage;
        break;

        case ConclusionPage:
            return -1;
        break;
    }

    return QWizard::nextId();
}

ContentWizard::~ContentWizard()
{
}
