#include "history_dialog.h"
#include "ui_history_dialog.h"

history_Dialog::history_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::history_Dialog)
{
    // 去掉问号，只保留关闭
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);
}

history_Dialog::~history_Dialog()
{
    delete ui;
}
