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

    TabModel->setHeaderData(0, Qt::Horizontal, tr("上次打开时间"));
    TabModel->setHeaderData(1, Qt::Horizontal, tr("文件路径"));

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
    TabModel->removeRows(0, TabModel->rowCount());
    TabModel->submitAll();
    TabModel->select();
    qDebug() << "deleteAll";

    return true;
}

bool IDatabase::addHistory(const QString str)
{
    QSqlQuery query;
    query.prepare("select * from history where src = :SRC");
    query.bindValue("SRC", str);
    query.exec();

    QString sql = "";
    if(query.first()){
        sql = "update history set date = strftime('%Y-%m-%d %H:%M:%S','now') where src = '" + str + "'";
        if(query.exec(sql)){
            qDebug() << "更改成功";
            return true;
        }else{
            qDebug() << "更改失败";
            return false;
        }
    }else{
        sql = "INSERT INTO history VALUES(strftime('%Y-%m-%d %H:%M:%S','now'), '" + str + "')";
        if(query.exec(sql)){
            qDebug() << "插入成功";
            return true;
        }else{
            qDebug() << "插入失败";
            return false;
        }
    }
}


IDatabase::IDatabase(QObject *parent) : QObject(parent)
{
    initDatabase();
}
