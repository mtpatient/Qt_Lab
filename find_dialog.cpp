#include "find_dialog.h"
#include "ui_find_dialog.h"
#include <QMessageBox>

Find_Dialog::Find_Dialog(QWidget *parent, QPlainTextEdit *textEdit) :
    QDialog(parent),
    ui(new Ui::Find_Dialog)
{
    // 去掉问号，只保留关闭
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    ui->rbdown->setChecked(true);

    pTextEdit = textEdit;
}

Find_Dialog::~Find_Dialog()
{
    delete ui;
}

//取消按钮
void Find_Dialog::on_btncancel_clicked()
{
    accept();
}

//点击下一个
void Find_Dialog::on_btnnext_clicked()
{
    QString target = ui->textFind->text();

    if(target == "" || pTextEdit == nullptr){
        return;
    }

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();

    int index = -1;

    if(ui->rbdown->isChecked()){
        index = text.indexOf(target, c.position(), ui->distinguish->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        if(index >= 0){
            c.setPosition(index);
            c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }else if(ui->rbup->isChecked()){
        index = text.lastIndexOf(target, c.position() - text.length() - 1, ui->distinguish->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        if(index >= 0){
            c.setPosition(index + target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }

    if(index < 0){
        QMessageBox msg(this);
        msg.setWindowTitle("查询失败");
        msg.setText(QString("查询不到") + target);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}

