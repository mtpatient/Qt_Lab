#ifndef HISTORY_DIALOG_H
#define HISTORY_DIALOG_H

#include <QDialog>

namespace Ui {
class history_Dialog;
}

class history_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit history_Dialog(QWidget *parent = nullptr);
    ~history_Dialog();

private:
    Ui::history_Dialog *ui;
};

#endif // HISTORY_DIALOG_H
