#include "history_dialog.h"
#include "ui_history_dialog.h"
#include "idatabase.h"

history_Dialog::history_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::history_Dialog)
{
    // 去掉问号，只保留关闭
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDataBase = IDatabase::getInstance();

    if(iDataBase.initModel()){
        ui->tableView->setModel(iDataBase.TabModel);
        ui->tableView->setSelectionModel(iDataBase.Selection);
        qDebug() << "tableView";
    }
}

history_Dialog::~history_Dialog()
{
    delete ui;
}

//删除
void history_Dialog::on_pushButton_2_clicked()
{
    IDatabase::getInstance().deleteCurrent();
}

//删除全部
void history_Dialog::on_pushButton_3_clicked()
{
    IDatabase::getInstance().deleteAll();
}

//打开历史文件
void history_Dialog::on_pushButton_clicked()
{

}

