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

#ifndef MS_WIZARD_HPP
#define MS_WIZARD_HPP

#include "wizards/base.hpp"

namespace SpeechControl {
namespace Wizards {

class MicrophoneSetup : public WizardBase
{
    Q_OBJECT
    enum {
        IntroductionPage = 0,
        SelectionPage,
        ConfigurationPage,
        ConclusionPage
    };

public:
    explicit MicrophoneSetup(QWidget *parent = 0);
    ~MicrophoneSetup();

};

}}
#endif // WIZARD_HPP
