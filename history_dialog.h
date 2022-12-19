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

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

signals:
    void sendSrc(QString src);

private:
    Ui::history_Dialog *ui;
};

#endif // HISTORY_DIALOG_H
