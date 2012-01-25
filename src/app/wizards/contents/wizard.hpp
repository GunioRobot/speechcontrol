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

#ifndef CONTENTS_WIZARD_HPP
#define CONTENTS_WIZARD_HPP

#include "wizards/base.hpp"

namespace SpeechControl {
namespace Wizards {

class ContentWizard : public WizardBase
{
    Q_OBJECT
public:
    enum Pages {
        IntroductionPage = 0,
        AdditionSelectionPage,

        // from Wikipedia (not yet implemented)
        WikiSourcePage,
        PageSelectionPage,

        // from a local file...
        /// @todo Provide support for RTF.
        /// @todo Provide support for ODT.
        /// @todo Provide support for PDF.
        /// @todo Provide support for ePub.
        FileSelectionPage,

        ConclusionPage
    };

    explicit ContentWizard(QWidget *parent = 0);
    virtual ~ContentWizard();

public slots:
    virtual int nextId() const;

};

}}
#endif // WIZARD_HPP
