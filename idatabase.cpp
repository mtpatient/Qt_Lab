#include "idatabase.h"
#include <QUuid>

void IDatabase::initDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    QString aFile = "Labdb.db";
    database.setDatabaseName(aFile); //设置数据库名称
    database.setUserName("root");
    database.setPassword("123456");

    if (!database.open())   //打开数据库
    {
        qDebug() << "failed to open database";
    }else{
        qDebug() << "open database";
    }
}

bool IDatabase::initModel()
{
    TabModel = new QSqlTableModel(this, database);
    TabModel->setTable("history");
    TabModel->setEditStrategy(QSqlTableModel::OnManualSubmit); //数据保存方式
    TabModel->setSort(TabModel->fieldIndex("date"), Qt::DescendingOrder);//排序

    if(!TabModel->select()){
        qDebug() << "false";
        return false;
    }

    qDebug() << "init";
    Selection = new QItemSelectionModel(TabModel);
    return true;
}


bool IDatabase::deleteCurrent()
{
    QModelIndex curIndex = Selection->currentIndex();
    qDebug() << curIndex.row();
    if(TabModel->removeRows(curIndex.row(), 1)){
        TabModel->submitAll();
        TabModel->select();
        qDebug() << "deleteCurrent";
    }else{
        qDebug() << "删除失败";
    }

    return true;
}

bool IDatabase::deleteAll()
{
    for(auto s : Selection->selectedRows()){
        TabModel->removeRows(s.row(), 1);
    }
    TabModel->submitAll();
    TabModel->select();
    qDebug() << "deleteAll";

    return true;
}


IDatabase::IDatabase(QObject *parent) : QObject(parent)
{
    initDatabase();
}
