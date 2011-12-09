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
 
#include "windows_main.hpp"
#include "ui_main.h"
#include <QMainWindow>

namespace SpeechControl {
    namespace Windows {
        Main::Main() : m_ui(new Ui::MainWindow) {
           m_ui->setupUi(this);
           m_ui->retranslateUi(this);

           connect(m_ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
           m_ui->tabWidget->setCurrentIndex(0);
        }

        void Main::tabChanged(const int& p_index){
            switch (p_index){
                case 0:
                break;
            }
        }

        Main::~Main() {
            delete m_ui;
        }
    }
}
