#ifndef SIMPSONGUI_H
#define SIMPSONGUI_H

#include <QDialog>

namespace Ui {
    class SimpsonGui;
}

class SimpsonGui : public QDialog {
    Q_OBJECT
public:
    SimpsonGui(QWidget *parent = 0);
    ~SimpsonGui();

    QString getSimpsonString() const;
    static QString SimpsonDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SimpsonGui *ui;
};

#endif // SIMPSONGUI_H
