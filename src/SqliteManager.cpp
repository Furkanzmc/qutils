#include "qutils/SqliteManager.h"
#include "qutils/Macros.h"
// Qt
#include <QSqlQuery>
#include <QSqlRecord>

namespace zmc
{

SqliteManager::SqliteManager()
    : m_LastError()
{

}

QSqlDatabase SqliteManager::openDatabase(const QString &databasePath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);
    if (db.open() == false) {
        LOG_ERROR("Cannot open the database at " << databasePath);
    }

    return db;
}

void SqliteManager::closeDatabase(QSqlDatabase &database)
{
    database.close();
}

bool SqliteManager::createTable(QSqlDatabase &database, const QList<ColumnDefinition> &columns, const QString &tableName)
{
    bool successful = false;
    if (database.isOpen() == false) {
        LOG_ERROR("Database is not open!");
        return successful;
    }

    if (isTableExist(database, tableName)) {
        LOG_ERROR("A table with the name " << tableName << " already exists in the database!");
        return successful;
    }

    QString sqlQueryStr = "CREATE TABLE \"main\".\"" + tableName + "\" (";
    int index = 0;
    for (const ColumnDefinition &def : columns) {
        sqlQueryStr += "\"" + def.name + "\" " + getColumnTypeName(def.type) + " " + def.getNullText();
        index++;
        if (index < columns.size()) {
            sqlQueryStr += ",";
        }
    }

    // Add the ending parenthesis
    sqlQueryStr += ")";

    QSqlQuery query(database);
    bool isExecSuccessful = query.exec(sqlQueryStr);
    if (isExecSuccessful == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
        successful = false;
    }
    else {
        LOG("Table \"" << tableName << "\" was succesffuly created!");
        successful = true;
    }

    return successful;
}

bool SqliteManager::isTableExist(QSqlDatabase &database, const QString &tableName)
{
    bool isExist = false;

    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return isExist;
    }

    const QString sqlQueryStr = "SELECT COUNT(name) FROM sqlite_master WHERE type='table' AND name='" + tableName + "'";
    QSqlQuery query(database);
    bool successfull = query.exec(sqlQueryStr);
    if (successfull == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
    }
    else {
        while (query.next()) {
            const QVariant countVar = query.value(0);
            if (countVar.isValid()) {
                bool ok = false;
                const int count = countVar.toInt(&ok);
                if (ok && count == 1) {
                    isExist = true;
                }
            }
        }
    }

    return isExist;
}

bool SqliteManager::dropTable(QSqlDatabase &database, const QString &tableName)
{
    bool successful = false;
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return successful;
    }

    if (isTableExist(database, tableName) == false) {
        LOG_ERROR("Given table, " << tableName << ", does not exist!");
        return successful;
    }

    const QString sqlQueryStr = "DROP TABLE " + tableName;
    QSqlQuery query(database);
    bool successfull = query.exec(sqlQueryStr);
    if (successfull == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
    }
    else {
        successful = true;
    }

    return successful;
}

QString SqliteManager::constructWhereQuery(const QList<SqliteManager::Constraint> &values)
{
    QString query = "WHERE ";
    int index = 0;
    for (const auto &t : values) {
        if (index != 0) {
            query += " ";
        }

        query += std::get<0>(t) + "='" + std::get<1>(t) + "'";
        if (index < values.size() - 1) {
            query += " " + std::get<2>(t);
        }

        index++;
    }

    return query;
}

QList<QMap<QString, QVariant> > SqliteManager::executeSelectQuery(QSqlDatabase &database, const QString &sqlQueryStr)
{
    QList<QMap<QString, QVariant>> resultList;
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return resultList;
    }

    QSqlQuery query(database);
    if (query.exec(sqlQueryStr) == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
    }
    else {
        while (query.next()) {
            const QSqlRecord record = query.record();
            const unsigned int count = record.count();
            QVariantMap resultMap;
            for (unsigned int columnIndex = 0; columnIndex < count; columnIndex++) {
                resultMap[record.fieldName(columnIndex)] = record.value(columnIndex);
            }

            resultList.append(resultMap);
        }
    }

    return resultList;
}

QList<QMap<QString, QVariant> > SqliteManager::getFromTable(QSqlDatabase &database, const QString &tableName, const unsigned int &limit,
        const QList<Constraint> *constraints, const SelectOrder *selectOrder)
{
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return QList<QMap<QString, QVariant>>();
    }

    if (isTableExist(database, tableName) == false) {
        LOG_ERROR("Given table, " << tableName << ", is does not exist!");
        return QList<QMap<QString, QVariant>>();
    }

    QString sqlQueryStr = "SELECT * FROM " + tableName;
    if (constraints && constraints->size() > 0) {
        sqlQueryStr += " " + constructWhereQuery(*constraints);
    }

    if (selectOrder && selectOrder->fieldName.length() > 0) {
        const QString orderType = selectOrder->order == SelectOrder::OrderType::ASC ? "ASC" : "DESC";
        sqlQueryStr += " ORDER BY " + selectOrder->fieldName + " " + orderType;
    }

    if (limit > 0) {
        sqlQueryStr += " LIMIT " + QString::number(limit);
    }

    return executeSelectQuery(database, sqlQueryStr);
}

bool SqliteManager::insertIntoTable(QSqlDatabase &database, const QString &tableName, const QMap<QString, QVariant> &row)
{
    bool successful = false;
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return successful;
    }

    if (isTableExist(database, tableName) == false) {
        LOG_ERROR("Given table, " << tableName << ", is does not exist!");
        return successful;
    }

    QSqlQuery query(database);
    QString sqlQueryStr = "INSERT INTO " + tableName + " ";
    QStringList columnNames, valuePlaceholders;
    for (auto it = row.constBegin(); it != row.constEnd(); it++) {
        columnNames.append(it.key());
        valuePlaceholders.append(":" + it.key());
    }

    sqlQueryStr.append("(" + columnNames.join(',') + ") ");
    sqlQueryStr.append("VALUES(" + valuePlaceholders.join(',') + ")");
    query.prepare(sqlQueryStr);

    // Now bind the values
    for (auto it = row.constBegin(); it != row.constEnd(); it++) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (query.exec() == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text() << ". Query: " << query.executedQuery());
    }
    else {
        successful = true;
    }

    return successful;
}

bool SqliteManager::updateInTable(QSqlDatabase &database, const QString &tableName, const QMap<QString, QVariant> &row, const QList<Constraint> &constraints)
{
    bool successful = false;
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return successful;
    }

    if (isTableExist(database, tableName) == false) {
        LOG_ERROR("Given table, " << tableName << ", is does not exist!");
        return successful;
    }

    QSqlQuery query(database);
    QString sqlQueryStr = "UPDATE " + tableName + " SET ";
    QStringList newValues;
    for (auto it = row.constBegin(); it != row.constEnd(); it++) {
        newValues.append(it.key() + "=" + ":" + it.key());
    }

    sqlQueryStr.append(newValues.join(','));
    sqlQueryStr.append(" " + constructWhereQuery(constraints));

    query.prepare(sqlQueryStr);
    // Now bind the values
    for (auto it = row.constBegin(); it != row.constEnd(); it++) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (query.exec() == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
    }
    else {
        successful = true;
    }

    return successful;
}

bool SqliteManager::deleteInTable(QSqlDatabase &database, const QString &tableName, const QList<Constraint> &constraints)
{
    bool successful = false;
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return successful;
    }

    if (isTableExist(database, tableName) == false) {
        LOG_ERROR("Given table, " << tableName << ", is does not exist!");
        return successful;
    }

    QSqlQuery query(database);
    QString sqlQueryStr = "DELETE FROM " + tableName + " ";
    sqlQueryStr.append(constructWhereQuery(constraints));

    if (query.exec(sqlQueryStr) == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
    }
    else {
        successful = true;
    }

    return successful;
}

bool SqliteManager::exists(QSqlDatabase &database, const QString &tableName, const QList<Constraint> &constraints)
{
    bool exists = false;
    if (database.isOpen() == false) {
        LOG_ERROR("Given database is not open!");
        return exists;
    }

    if (isTableExist(database, tableName) == false) {
        LOG_ERROR("Given table, " << tableName << ", is does not exist!");
        return exists;
    }

    if (constraints.size() == 0) {
        LOG_ERROR("Constraints size cannot be 0!");
        return exists;
    }

    const QString sqlQueryStr = "SELECT COUNT(id) FROM " + tableName + " " + constructWhereQuery(constraints);
    QSqlQuery query(database);
    bool successfull = query.exec(sqlQueryStr);
    if (successfull == false) {
        updateError(database, sqlQueryStr);
        LOG_ERROR("Error occurred. Message: " << database.lastError().text());
    }
    else {
        const int count = query.value(0).toInt();
        exists = count > 0;
    }

    return exists;
}

const SqliteManager::SqliteError &SqliteManager::getLastError() const
{
    return m_LastError;
}

QString SqliteManager::getColumnTypeName(const ColumnTypes &type) const
{
    QString name = "";
    if (type == ColumnTypes::TEXT) {
        name = "TEXT";
    }
    else if (type == ColumnTypes::PK_INTEGER) {
        name = "INTEGER PRIMARY KEY";
    }
    else if (type == ColumnTypes::PK_AUTOINCREMENT) {
        name = "INTEGER PRIMARY KEY AUTOINCREMENT";
    }
    else if (type == ColumnTypes::INTEGER) {
        name = "INTEGER";
    }
    else if (type == ColumnTypes::REAL) {
        name = "REAL";
    }
    else if (type == ColumnTypes::BLOB) {
        name = "BLOB";
    }
    else if (type == ColumnTypes::NULL_TYPE) {
        name = "NULL";
    }

    return name;
}

SqliteManager::ColumnTypes SqliteManager::getColumnType(const QString &typeName) const
{
    ColumnTypes type;
    if (typeName == "TEXT") {
        type = ColumnTypes::TEXT;
    }
    else if (typeName == "INTEGER PRIMARY KEY") {
        type = ColumnTypes::PK_INTEGER;
    }
    else if (typeName == "INTEGER PRIMARY KEY AUTOINCREMENT") {
        type = ColumnTypes::PK_AUTOINCREMENT;
    }
    else if (typeName == "INTEGER") {
        type = ColumnTypes::INTEGER;
    }
    else if (typeName == "REAL") {
        type = ColumnTypes::REAL;
    }
    else if (typeName == "BLOB") {
        type = ColumnTypes::BLOB;
    }
    else if (typeName == "NULL") {
        type = ColumnTypes::NULL_TYPE;
    }

    return type;
}

void SqliteManager::updateError(QSqlDatabase &db, const QString &query)
{
    m_LastError.error = db.lastError();
    m_LastError.query = query;
}

}
