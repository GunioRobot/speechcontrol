#ifndef P_MICSETUP_HPP
#define P_PMICSETUP_HPP

#include <QWizardPage>

namespace Ui {
    class MicrophoneConfiguration;
}

namespace SpeechControl {
namespace Wizards {
namespace Pages {

class MicrophoneConfiguration : public QWizardPage
{
    Q_OBJECT

public:
    explicit MicrophoneConfiguration(QWidget *parent = 0);
    ~MicrophoneConfiguration();

private:
    Ui::MicrophoneConfiguration *ui;
};

}
}
}

#endif // MICSETUP_HPP
