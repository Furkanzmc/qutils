#pragma once
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariantList>
// STD
#include <string>
#include <map>
#include <vector>
#include <functional>

namespace qutils
{

class SqliteManager
{
public:
    enum class ColumnTypes : unsigned int {
        TEXT,
        PK_INTEGER,
        PK_AUTOINCREMENT,
        INTEGER,
        REAL,
        BLOB,
        NULL_TYPE,
        NONE
    };

    struct SelectOrder {
        enum class OrderType {
            ASC,
            DESC
        };

        SelectOrder() = default;

        SelectOrder(OrderType _order, QString columnName)
            : order(_order)
            , fieldName(columnName)
        {
        }

        OrderType order;
        QString fieldName;
    };

    struct ColumnDefinition {
        bool isNull = false;
        ColumnTypes type = ColumnTypes::TEXT;
        QString name;

        ColumnDefinition() = default;
        ColumnDefinition(bool _null, ColumnTypes _type, const QString _name)
            : isNull(_null)
            , type(_type)
            , name(_name)
        {}

        QString getNullText() const
        {
            return isNull ? "" : "NOT NULL";
        }
    };

    struct Index {
        Index() = default;
        Index(int c, int r)
            : column(c)
            , row(r)
        {}

        int column = 0, row = 0;
    };

    struct SqliteError {
        SqliteError() = default;

        QString query;
        QSqlError error;

        friend std::ostream &operator<<(std::ostream &os, const SqliteError &err)
        {
            os << "Sqlite Error: " << err.error.text().toStdString() << "\nQuery: " << err.query.toStdString() << std::endl;
            return os;
        }
    };

    using Constraint = std::tuple<QString/*columnName*/, QString/*value*/, QString/*AND|OR*/>;

public:
    SqliteManager();

    /**
     * @brief Creates a sqlite3 instance and returns it. If there's an error, returns nullptr. You can get the error with getLastError()
     * @param databasePath
     * @param createIfFileAbsent
     * @return QSqlDatabase
     */
    QSqlDatabase openDatabase(const QString &databasePath);

    /**
     * @brief Closes the given database
     * @param database
     * @return void
     */
    void closeDatabase(QSqlDatabase &database);

    /**
     * @brief createTable
     * **Example Usage:**
     * @code
     *    SqliteManager man;
     *    std::vector<SqliteManager::ColumnDefinition> columns {
     *        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, "first"),
     *        SqliteManager::ColumnDefinition(false, SqliteManager::ColumnTypes::INTEGER, "second")
     *    };
     *
     *    QSqlDatabase db = man.openDatabase("C:/Users/Furkanzmc/Desktop/test.sqlite");
     *    man.createTable(db, columns, "my_table");
     * @endcode
     * @param database
     * @param columns
     * @return bool
     */
    bool createTable(QSqlDatabase &database, const std::vector<ColumnDefinition> &columns, const QString &tableName);

    /**
     * @brief Returns true If the table with the given name exists, returns false otherwise.
     * @param database
     * @param tableName
     * @return bool
     */
    bool isTableExist(QSqlDatabase &database, const QString &tableName);

    /**
     * @brief Deletes the given table from the database
     * @param database
     * @param tableName
     * @return bool
     */
    bool dropTable(QSqlDatabase &database, const QString &tableName);

    /**
     * @brief Constructs a string for the WHERE queries.
     * @param values - It's a tuple where item:
     *                 #1 is column name
     *                 #2 is value
     *                 #3 is constraint (OR or AND)
     * The constraint is added to the end of the query. So for odd number of items, the query will not end with the contraint.
     * **Example Usage:**
     * @code
     *    SqliteManager man;
     *    std::vector<std::tuple<QString, QString, QString>> values {
     *        std::make_tuple("first", "1", "AND"),
     *        std::make_tuple("second", "1", "OR")
     *    };
     *
     *    std::cerr << man.constructWhereQuery(values) << std::endl;
     *    // Output is: WHERE first=1 AND second=1
     * @endcode
     * @return QString Returns the query WITHOUT the WHERE clause
     */
    QString constructWhereQuery(const std::vector<Constraint> &values);

    /**
     * @brief Executes the given query and if it is successful iterates through the results and calls the `callback` in each step.
     * You can then use the `sqlite3_stmt` to get the value of the column. You can either provide the `callback` here, or you can use
     * `onRowProcessed` callback. The iteration will continue as long as the `callback` returns `true`, if it returns `false` the operation
     * will be terminated.
     * @param database
     * @param query
     * @param callback
     *
     * **Example Usage:**
     * @code
     *    SqliteManager man;
     *    sqlite3 *db = man.openDatabase("C:/Users/Furkanzmc/Desktop/test.sqlite");
     *    const QString query = "SELECT * from new_table_2";
     *    const QVariantList data = man.executeSelectQuery(db, query);
     *    qDebug() << data;
     * @endcode
     * @return QList<QVariantMap>
     */
    QList<QVariantMap> executeSelectQuery(QSqlDatabase &database, const QString &sqlQueryStr);

    /**
     * @brief Executes a select query with the given constraints on the given table. If it succeeds, the table data is returned as a QList<QVariantMap>.
     * If there's an error, the map will be empty.
     * @param tableName
     * @param constraints
     * @param limit
     * @param selectOrder If it is empty, it is ignored.
     * @return QList<QVariantMap>
     */
    QList<QVariantMap> getFromTable(QSqlDatabase &database, const QString &tableName, const unsigned int &limit = -1,
                                    const std::vector<Constraint> *constraints = nullptr,
                                    const SelectOrder *selectOrder = nullptr);

    /**
     * @brief Insert row(s) into the given table. If all of the rows have the same number of items, one SQL command will be used to insert them.
     * If they have different sizes, multiple SQL commands are used.
     * **Example Usage:**
     * @code
     *    qutils::SqliteManager man;
     *    std::vector<qutils::SqliteManager::ColumnDefinition> columns {
     *        qutils::SqliteManager::ColumnDefinition(false, qutils::SqliteManager::ColumnTypes::INTEGER, "first"),
     *        qutils::SqliteManager::ColumnDefinition(false, qutils::SqliteManager::ColumnTypes::INTEGER, "second"),
     *        qutils::SqliteManager::ColumnDefinition(false, qutils::SqliteManager::ColumnTypes::BLOB, "image")
     *    };
     *
     *    QFile file("E:/Users/Furkanzmc/Pictures/Wallpapers/batman_trilogy_2-wallpaper-1440x1080.jpg");
     *    file.open(QIODevice::ReadOnly);
     *    const QByteArray inByteArray = file.readAll();
     *
     *    QSqlDatabase db = man.openDatabase("E:/Users/Furkanzmc/Desktop/test.sqlite");
     *    man.createTable(db, columns, "my_table");
     *    QVariantMap map;
     *    map["first"] = 122;
     *    map["second"] = 122;
     *    map["image"] = inByteArray;
     *    man.insertIntoTable(db, "my_table", map);
     * @endcode
     * @param database
     * @param tableName
     * @param rows Rows must be a list of QVarianMap
     * @return bool
     */
    bool insertIntoTable(QSqlDatabase &database, const QString &tableName, const QVariantMap &row);

    const SqliteError &getLastError() const;

    QString getColumnTypeName(const ColumnTypes &type) const;
    ColumnTypes getColumnType(const QString &typeName) const;

private:
    SqliteError m_LastError;

private:
    void updateError(QSqlDatabase &db, const QString &query = "");

};

}
