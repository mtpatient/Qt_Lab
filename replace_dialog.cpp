#include "replace_dialog.h"
#include "ui_replace_dialog.h"
#include <QMessageBox>

replace_Dialog::replace_Dialog(QWidget *parent, QPlainTextEdit *textEdit) :
    QDialog(parent),
    ui(new Ui::replace_Dialog)
{
    // 去掉问号，只保留关闭
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
    pTextEdit = textEdit;
    ui->rbdown->setChecked(true);
}

replace_Dialog::~replace_Dialog()
{
    delete ui;
}

void replace_Dialog::on_btnnext_clicked()
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


void replace_Dialog::on_btncancel_clicked()
{
    accept();
}


void replace_Dialog::on_btnReplaceAll_clicked()
{
    QString target = ui->textFind->text();
    QString to = ui->textReplace->text();

    if((pTextEdit != nullptr) && (target != "") && (to != "")){
        QString text = pTextEdit->toPlainText();

        text.replace(target, to, ui->distinguish->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

        pTextEdit->clear();

        pTextEdit->insertPlainText(text);
    }
}


void replace_Dialog::on_btnReplace_clicked()
{
    QString target = ui->textFind->text();
    QString to = ui->textReplace->text();

    if((pTextEdit != nullptr) && (target != "") && (to != "")){
        QString selText = pTextEdit->textCursor().selectedText();

        pTextEdit->insertPlainText(to);

        on_btnnext_clicked();
    }
}

