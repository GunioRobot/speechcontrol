#include "wizards/intro.hpp"
#include "ui_intro.h"

using namespace SpeechControl::Wizards::Pages;

Introduction::Introduction(const QString& summary) :
                           QWizardPage(),
    ui(new Ui::Introduction)
{
    ui->setupUi(this);
    ui->labelSummary->setText(summary);
}

Introduction::~Introduction()
{
    delete ui;
}
