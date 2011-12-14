#ifndef INTRO_HPP
#define INTRO_HPP

#include <QWizardPage>

namespace Ui {
    class Introduction;
}

namespace SpeechControl {
namespace Wizards {
namespace Pages {

class Introduction : public QWizardPage
{
    Q_OBJECT

public:
    explicit Introduction(QWidget *parent = 0);
    ~Introduction();

private:
    Ui::Introduction *ui;
};

}}}

#endif // INTRO_HPP
