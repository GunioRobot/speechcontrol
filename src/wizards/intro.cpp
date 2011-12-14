#include "wizards/intro.hpp"
#include "ui_intro.h"

using namespace SpeechControl::Wizards::Pages;

Introduction::Introduction(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::Introduction)
{
    ui->setupUi(this);
}

Introduction::~Introduction()
{
    delete ui;
}
