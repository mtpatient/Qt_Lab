#include "idatabase.h"
#include <QUuid>

void IDatabase::initDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE"); //添加 SQL LITE数据库驱动
    QString aFile = "Labdb.db";
    database.setDatabaseName(aFile); //设置数据库名称

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
    TabModel->setSort(TabModel->fieldIndex("date"), Qt::AscendingOrder);//排序

    if(!(TabModel->select())){
        return false;
    }

    Selection = new QItemSelectionModel(TabModel);
    return true;
}


//bool IDatabase::searchPatient(QString filter)
//{
//    patientTabModel->setFilter(filter);
//    return patientTabModel->select();
//}

//bool IDatabase::deleteCurrentPatient()
//{
//    QModelIndex curIndex = patientSelection->currentIndex();
//    patientTabModel->removeRow(curIndex.row());
//    patientTabModel->submitAll();
//    patientTabModel->select();
//}

//bool IDatabase::submitPatientEdit()
//{
//    return patientTabModel->submitAll();
//}

//void IDatabase::revertPatientEdit()
//{
//    patientTabModel->revertAll();
//}

IDatabase::IDatabase(QObject *parent) : QObject(parent)
{
    initDatabase();
}
