#include "firstrunwizard.hpp"
#include "ui_firstrunwizard.h"

FirstRunWizard::FirstRunWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::FirstRunWizard)
{
    ui->setupUi(this);
}

FirstRunWizard::~FirstRunWizard()
{
    delete ui;
}
