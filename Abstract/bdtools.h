#ifndef BDTOOLS_H
#define BDTOOLS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QCompleter>
#include <QSqlRelationalTableModel>
#include <stdint.h>

typedef unsigned short int usint;

typedef uint32_t ullint;

class BDTools : public QObject
{
    Q_OBJECT
public: // função
    explicit BDTools(QObject *parent = 0);

    enum typeModel{
        TableModel = 0,
        QueryModel = 1,
        RelationalModel = 2
    };

    bool start(QString database, QString connectionName, QString driver = "QSQLITE");

    void createComponent();

    bool exec(QString sql);

    bool insert(QString table, QList<QString> values);
    bool insert(QString table, QList<QString> fields, QList<QString> values);
    bool insert(QString table, QString field, QString value);

    bool remove(QString table, QString fieldCondition, QString valueCondition);
    bool remove(QString table, int column); // fazer

    bool update(QString table, QList<QString> fields, QList<QString> values, QString fieldCondition, QString valueCondition);
    bool update(QString table, QString field, QString value, QString fieldCondition, QString valueCondition);

    ullint search(QString sql, int modelSearchIndex = 0);
    ullint search(QString table, QList<QString> fieldsCondition, QList<QString> valuesCondition, int modelSearchIndex = 0);
    ullint search(QString table, QString fieldCondition, QString valueCondition, int modelSearchIndex = 0);

    QString loadSimple(int column, int modelSearchIndex = 0);
    QList<QString> loadLine(int line, int modelSearchIndex = 0);
    QList<QString> loadColumn(int column, int modelSearchIndex = 0);

    bool createModel(QString table, QList<QString> headersTable = QList<QString>());
    bool createModelQuery(QString table, QString sql);
    bool createModelQuery(QString table, QString sql, QList<QString> headersTable);
    bool createModelRelational(QString table, int column, QString relationalTable, QString relationalField, QString displayColumn, QList<QString> headersTable);
    bool updateModelQuery(QString table, QString newSQL);

    QSqlTableModel *model(QString table);
    QSqlQueryModel *modelQuery(QString table);
    QSqlRelationalTableModel *modelRelational(QString table);

    bool refreshModel(QString table);
    bool refreshQueryModel(QString table);
    bool refreshRelationalModel(QString table);

    int headerToColumn(QString table, QString header); // pega o cabeçalho e retorna a coluna dele

    bool createCompleter(QString name, QString table, int column, bool caseSensitive, QCompleter::CompletionMode mode, typeModel type = TableModel);
    bool createCompleter(QString name, QString table, QString column, bool caseSensitive, QCompleter::CompletionMode mode, typeModel type = TableModel);
    QCompleter *completer(QString name);

    int countRecord(QString table);

    int generateIdInt(QString table, QString field);

    QString generateIdStr(QString table, QString field);

    QString lastError();

    int qntRecordsLastSearch();

    QSqlQueryModel *modelSearch(int index = 0);

signals:

public:
    QSqlDatabase sdb;

    QCompleter *completers[5];
    QList<QString> nameCompleters;

    QSqlTableModel *modelsTable[5];
    QList<QString> nameModelsTable;
    QList<QString> headersTable[5];

    QSqlRelationalTableModel *modelsRelational[3];
    QList<QString> nameModelsRelational;
    QList<QString> headersModelsRelational[3];

    QSqlQueryModel *modelsQuery[5];
    QList<QString> nameModelsQuery;
    QList<QString> modelsQuerySql;
    QList<QList<QString> > headersModelsQuery;



protected: // variaveis
    QString _lastError; // guarda o erro dado no bd.exec, caso tenha

    unsigned long int _qntRecordsLastSearch; // guarda a quantidade de registros da ultima busca

    QSqlQueryModel *_modelSearch[5];

public slots:

};

#endif // BDTOOLS_H
