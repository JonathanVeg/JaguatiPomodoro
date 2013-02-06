#include "bdtools.h"
#include <QDebug>
#include <QFile>
#include <QDir>

BDTools::BDTools(QObject *parent) :
    QObject(parent)
{
    createComponent();
}

bool BDTools::start(QString database, QString connectionName, QString driver){
    sdb = QSqlDatabase::addDatabase(driver, connectionName);

    sdb.setDatabaseName(database);

//    QString username;
//    QString password;
//    QString ip;
//    int port;

//    QFile file(QDir::currentPath() + "/config.txt");
//    if (file.open(QIODevice::ReadOnly)){
//        QTextStream in(&file);
//        username = e.decrypt(in.readLine());
//        password = e.decrypt(in.readLine());
//        ip = e.decrypt(in.readLine());
//        port = e.decrypt(in.readLine()).toInt();
//    }
//    file.close();

//    ip = "localhost";


//    sdb.setHostName(ip);
//    sdb.setUserName(username);
//    sdb.setPassword(password);
//    sdb.setPort(port);
    sdb.setDatabaseName(database);

    bool open = sdb.open();
    if (! open)
        qDebug() << "[BDTools] [START]" << sdb.lastError().text();

    return open;
}

void BDTools::createComponent(){
    // instancia os models de busca
    _modelSearch[0] = new QSqlQueryModel();
    _modelSearch[1] = new QSqlQueryModel();
    _modelSearch[2] = new QSqlQueryModel();
    _modelSearch[3] = new QSqlQueryModel();
    _modelSearch[4] = new QSqlQueryModel();
}

bool BDTools::exec(QString sql){
    QSqlQuery query(sdb);

    bool b = query.exec(sql);

    if (! b){
        _lastError = "Exec error:";
        _lastError += "\n\tSQL: " + sql;
        _lastError += "\n\tErro: " + query.lastError().text();

        qDebug() << lastError();
    }
    else
        _lastError.clear();

    return b;
}

bool BDTools::insert(QString table, QList<QString> values){
    QString sql = "insert into " + table + " values(";

    for (int i = 0; i < values.length(); i++){

        if (values[i].isEmpty())
            sql += "null, ";
        else{
            sql += "'" + values[i].replace("'", "''") + "', ";
        }
    }

    sql = sql.mid(0, sql.length() - 2) + ")";

    bool b = exec(sql);

    if (! b)
        qDebug() << "[BDTools] [INSERT] Erro na SQL: " << sql << "\n" << lastError();

    return b;
}

bool BDTools::insert(QString table, QList<QString> fields, QList<QString> values){
    QString sql = "insert into " + table + " (";

    for (int i = 0; i < fields.length(); i++){
        sql += fields[i] + ", ";
    }

    sql = sql.mid(0, sql.length() - 2);

    sql += ") values (";

    for (int i = 0; i < values.length(); i++){

        if (values[i].isEmpty())
            sql += "null, ";
        else{
            sql += "'" + values[i].replace("'", "''") + "', ";
        }
    }

    sql = sql.mid(0, sql.length() - 2);

    sql += ");";

    bool b = exec(sql);
    if (! b)
        qDebug() << "[BDTools] [INSERT] Erro na SQL: " << sql;

    try{
        model(table)->submitAll();
        model(table)->select();
    }catch(...){
        qDebug() << "[BDTOOLS] [INSERT] Erro no submitAll()/select() da tabela " + table;
    }

    return b;
}

bool BDTools::insert(QString table, QString field, QString value){
    value.replace("'", "''");

    QString sql = "insert into " + table + " (" + field + ") values ('" + value + "');";

    bool b = exec(sql);
    if (! b)
        qDebug() << "[BDTools] [INSERT] Erro na SQL: " << sql;

    try{
        model(table)->submitAll();
        model(table)->select();
    }catch(...){
        qDebug() << "[BDTOOLS] [INSERT] Erro no submitAll()/select() da tabela " + table;
    }

    return b;
}

bool BDTools::remove(QString table, QString fieldCondition, QString valueCondition){
    QString sql = "delete from " + table + " where " + fieldCondition + " = '" + valueCondition + "'";
    bool b = exec(sql);
    if (! b)
        qDebug() << "[BDTools] [UPDATE] Erro na SQL: " << sql;

    return b;
}

bool BDTools::update(QString table, QList<QString> fields, QList<QString> values, QString fieldCondition, QString valueCondition){
    QString sql = "update " + table + " set ";

    for (int i = 0; i < fields.length(); i++){

        if (values[i].isEmpty())
            sql += fields[i] + " = null, ";
        else{
            values[i].replace("'", "''");

            sql += fields[i] + " = '" + values[i] + "', ";
        }
    }

    sql = sql.mid(0, sql.length() - 2);

    sql += " where " + fieldCondition + " = '" + valueCondition + "'";

    bool b = exec(sql);
    if (! b)
        qDebug() << "[BDTools] [UPDATE] Erro na SQL: " << sql;

    return b;
}

bool BDTools::update(QString table, QString field, QString value, QString fieldCondition, QString valueCondition){

    value.replace("'", "''");

    QString sql = "update " + table + " set " + field + " = '" + value + "' where " + fieldCondition + " = '" + valueCondition + "'";
    bool b = exec(sql);
    if (! b)
        qDebug() << "[BDTools] [UPDATE] Erro na SQL: " << sql;

    return b;
}

ullint BDTools::search(QString sql, int modelSearchIndex){
    if (modelSearchIndex > 4)
        return false;

    QSqlQuery query(sdb);

    query.exec(sql);

    _modelSearch[modelSearchIndex]->setQuery(query);

    return _modelSearch[modelSearchIndex]->rowCount();
}

ullint BDTools::search(QString table, QList<QString> fieldsCondition, QList<QString> valuesCondition, int modelSearchIndex){
    QString sql = "select * from " + table + " where ";
    for (int i = 0; i < fieldsCondition.length(); i++){
        sql += fieldsCondition[i] + " = '" + valuesCondition[i] + "' and ";
    }

    sql = sql.mid(0, sql.length() - 5);

    return search(sql, modelSearchIndex);
}

ullint BDTools::search(QString table, QString fieldCondition, QString valueCondition, int modelSearchIndex){
    return search("select * from " + table + " where " + fieldCondition + " = '" + valueCondition + "'", modelSearchIndex);
}

QString BDTools::loadSimple(int column, int modelSearchIndex){
    QSqlRecord record = _modelSearch[modelSearchIndex]->record(0);

    return record.value(column).toString();
}

QList<QString> BDTools::loadLine(int line, int modelSearchIndex){
    QList<QString> lines;

    for (int i = 0; i < _modelSearch[modelSearchIndex]->columnCount(); i++){
        lines << _modelSearch[modelSearchIndex]->index(line, i).data().toString();
    }

    return lines;
}

QList<QString> BDTools::loadColumn(int column, int modelSearchIndex){
    QList<QString> columns;

    for (int i = 0; i < _modelSearch[modelSearchIndex]->rowCount(); i++){
        columns << _modelSearch[modelSearchIndex]->index(i, column).data().toString();
    }

    return columns;
}

bool BDTools::createModel(QString table, QList<QString> headers){
    if(nameModelsTable.indexOf(table) >= 0)
        return false;

    nameModelsTable.append(table);

    usint index = nameModelsTable.indexOf(table);

    modelsTable[index] = new QSqlTableModel(this,sdb);

    modelsTable[index]->setTable(table);

    modelsTable[index]->setSort(1, Qt::AscendingOrder);

    this->headersTable[index] = headers;

    foreach (QString header, headers)
        modelsTable[index]->setHeaderData(headers.indexOf(header), Qt::Horizontal, header);

    modelsTable[index]->select();

    return true;
}

bool BDTools::createModelQuery(QString table, QString sql){
    if (nameModelsQuery.indexOf(table) >= 0)
        return false;

    nameModelsQuery.append(table);

    usint index = nameModelsQuery.indexOf(table);

    modelsQuery[index] = new QSqlQueryModel();

    modelsQuery[index]->setQuery(sql, sdb);

    modelsQuerySql.append(sql);

    return true;
}

bool BDTools::createModelQuery(QString table, QString sql, QList<QString> headers){
    if (nameModelsQuery.indexOf(table) >= 0)
        return false;

    nameModelsQuery.append(table);

    usint index = nameModelsQuery.indexOf(table);

    modelsQuery[index] = new QSqlQueryModel();

    modelsQuery[index]->setQuery(sql, sdb);

    modelsQuerySql.append(sql);
    headersModelsQuery.append(headers);

    foreach (QString header, headers)
        modelsQuery[index]->setHeaderData(headers.indexOf(header), Qt::Horizontal, header);

    return true;
}

bool BDTools::updateModelQuery(QString table, QString newSql){
    int index = nameModelsQuery.indexOf(table);
    if (index < 0)
        return false;

    modelsQuery[index]->setQuery(newSql, sdb);
    modelsQuerySql[index] = newSql;

    return true;
}

bool BDTools::createModelRelational(QString table, int column, QString relationalTable, QString relationalField, QString displayColumn, QList<QString> headers){
    if (nameModelsRelational.indexOf(table) >= 0)
        return false;

    nameModelsRelational.append(table);

    usint index = nameModelsRelational.indexOf(table);

    modelsRelational[index] = new QSqlRelationalTableModel(this, sdb);

    modelsRelational[index]->setTable(table);

    modelsRelational[index]->setRelation(column, QSqlRelation(relationalTable, relationalField, displayColumn));

    modelsRelational[index]->setRelation(4, QSqlRelation("subcategory", "idsubcategory", "name"));

    foreach (QString header, headers)
        modelsRelational[index]->setHeaderData(headers.indexOf(header), Qt::Horizontal, header);

    modelsRelational[index]->select();

    return true;
}

QSqlTableModel *BDTools::model(QString table){
    int i = nameModelsTable.indexOf(table);
    if (i >= 0){
        modelsTable[i]->select();
        return modelsTable[i];
    }

    QString err = "Model " + table + " não encontrado!";
    throw(err);
}

QSqlQueryModel *BDTools::modelQuery(QString table){
    int i = nameModelsQuery.indexOf(table);
    if (i >= 0){
        return modelsQuery[i];
    }

    QString err = "Completer " + table + " não encontrado!";
    throw(err);
}

QSqlRelationalTableModel *BDTools::modelRelational(QString table){
    int i = nameModelsRelational.indexOf(table);
    if (i >= 0){
        modelsRelational[i]->select();
        return modelsRelational[i];
    }

    QString err = "Completer " + table + " não encontrado!";
    throw(err);
}

bool BDTools::refreshModel(QString table){
    int i = nameModelsTable.indexOf(table);
    if (i >= 0)
    {
        modelsTable[i]->select();
        modelsTable[i]->submitAll();

        return true;
    }

    return false;
}

bool BDTools::refreshQueryModel(QString table){
    int i = nameModelsQuery.indexOf(table);
    if (i >= 0){
        modelsQuery[i]->setQuery(modelsQuerySql[i], sdb);
        return true;
    }

    qDebug() << "[BDTools] [refreshQueryModel]" << table;

    return false;
}

bool BDTools::refreshRelationalModel(QString table){
    int i = nameModelsRelational.indexOf(table);
    if (i >= 0){
        modelsRelational[i]->select();
        return true;
    }

    qDebug() << "[BDTools] [refreshRelationalModel]" << table;

    return false;
}

int BDTools::headerToColumn(QString table, QString header){
    return headersTable[nameModelsTable.indexOf(table)].indexOf(header);
}

bool BDTools::createCompleter(QString name, QString table, QString column, bool caseSensitive, QCompleter::CompletionMode mode, typeModel type){
    if (headerToColumn(table,column) >= 0)
        return createCompleter(name, table, headerToColumn(table,column), caseSensitive, mode, type);

    return false;
}

bool BDTools::createCompleter(QString name, QString table, int column, bool caseSensitive, QCompleter::CompletionMode mode, typeModel type){
    if (nameCompleters.indexOf(name) >= 0)
        return false;

    nameCompleters.append(name);

    usint index = nameCompleters.indexOf(name);

    completers[index] = new QCompleter(this);

    if(caseSensitive)
        completers[index]->setCaseSensitivity(Qt::CaseSensitive);
    else
        completers[index]->setCaseSensitivity(Qt::CaseInsensitive);

    completers[index]->setCompletionMode(mode);

    switch (type){

    case TableModel:
    {
        try{
            completers[index]->setModel(model(table));
        }catch(QString err){
            qDebug() << "[BDTools] [createCompleter] [model] Completer da tabela" << table << "não criado";

            nameCompleters[index].clear();
        }

        break;
    }

    case QueryModel:
    {
        try{
            completers[index]->setModel(modelQuery(table));
        }catch(QString err){
            qDebug() << "[BDTools] [createCompleter] [modelQuery] Completer da tabela" << table << "não criado.";
        }

        break;
    }

    case RelationalModel:
    {
        try{
            completers[index]->setModel(modelRelational(table));
        }catch(QString err){
            qDebug() << "[BDTools] [createCompleter] [modelRelational] Completer da tabela" << table << "não criado";
        }

        break;
    }

    }

    completers[index]->setCompletionColumn(column);

    return true;
}

QCompleter *BDTools::completer(QString name){
    int i = nameCompleters.indexOf(name);
    if (i >= 0){
        return completers[i];
    }

    QString err = "Completer " + name + " não encontrado!";
    throw(err);
}

int BDTools::countRecord(QString table)
{
    QSqlQuery query(sdb);
    QSqlQueryModel queryModel;
    query.exec("select count(*) from " + table);

    queryModel.setQuery(query);

    return queryModel.index(0,0).data().toInt();
}

int BDTools::generateIdInt(QString table, QString field)
{
    return generateIdStr(table, field).toInt();
}

QString BDTools::generateIdStr(QString table, QString field)
{
    search("select max(" + field + ") from " + table);

    return QString::number(loadSimple(0).toInt() + 1);
}

QString BDTools::lastError()
{
    return _lastError;
}

int BDTools::qntRecordsLastSearch()
{
    return _qntRecordsLastSearch;
}

QSqlQueryModel * BDTools::modelSearch(int index)
{
    if (index > 4)
        throw QString("Index inválido na busca do model.");

    return _modelSearch[index];
}
