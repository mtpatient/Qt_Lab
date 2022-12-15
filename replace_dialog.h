#ifndef REPLACE_DIALOG_H
#define REPLACE_DIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class replace_Dialog;
}

class replace_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit replace_Dialog(QWidget *parent = nullptr, QPlainTextEdit *textEdit = nullptr);
    ~replace_Dialog();

private slots:
    void on_btnnext_clicked();

    void on_btncancel_clicked();

    void on_btnReplaceAll_clicked();

    void on_btnReplace_clicked();

private:
    Ui::replace_Dialog *ui;

    QPlainTextEdit *pTextEdit;
};

#endif // REPLACE_DIALOG_H
