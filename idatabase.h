#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDatabase : public QObject
{
    Q_OBJECT
public:

    static IDatabase &getInstance(){
        static IDatabase instance;
        return instance;
    }

private:
    explicit IDatabase(QObject *parent = nullptr);

    IDatabase(IDatabase const &) = delete;
    void operator=(IDatabase const &) = delete;

    QSqlDatabase database;

    void initDatabase();



signals:

public:
    bool initModel();

    QSqlTableModel *TabModel; //数据模型
    QItemSelectionModel *Selection; //选择模型

};

#endif // IDATABASE_H
