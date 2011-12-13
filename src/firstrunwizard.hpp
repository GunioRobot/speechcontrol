#ifndef FIRSTRUNWIZARD_HPP
#define FIRSTRUNWIZARD_HPP

#include <QWizard>

namespace Ui {
    class FirstRunWizard;
}

class FirstRunWizard : public QWizard
{
    Q_OBJECT

public:
    explicit FirstRunWizard(QWidget *parent = 0);
    ~FirstRunWizard();

private:
    Ui::FirstRunWizard *ui;
};

#endif // FIRSTRUNWIZARD_HPP
