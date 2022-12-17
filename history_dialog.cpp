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

    ui->listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setAlternatingRowColors(true);

    IDatabase &iDataBase = IDatabase::getInstance();

    if(iDataBase.initModel()){
        ui->listView->setModel(iDataBase.TabModel);
        ui->listView->setSelectionModel(iDataBase.Selection);
    }
}

history_Dialog::~history_Dialog()
{
    delete ui;
}
