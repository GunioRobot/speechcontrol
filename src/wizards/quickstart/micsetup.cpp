#include "wizards/quickstart/micsetup.hpp"
#include "ui_micasksetup.h"

using SpeechControl::Wizards::Pages;

MicrophoneConfiguration::MicrophoneConfiguration(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MicrophoneConfiguration)
{
    ui->setupUi(this);
}

MicrophoneConfiguration::~MicrophoneConfiguration()
{
    delete ui;
}
