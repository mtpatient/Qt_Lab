#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
namespace Ui {
class Find_Dialog;
}

class Find_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Find_Dialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit=nullptr);
    ~Find_Dialog();

private slots:
    void on_btncancel_clicked();

    void on_btnnext_clicked();

private:
    Ui::Find_Dialog *ui;

    QPlainTextEdit *pTextEdit;
};

#endif // FIND_DIALOG_H
