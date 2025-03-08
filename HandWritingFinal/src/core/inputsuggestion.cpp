#include "inputSuggestion.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

InputSuggestion::InputSuggestion(QObject* parent) : QObject(parent) {
    initDatabase();
}

InputSuggestion::~InputSuggestion() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool InputSuggestion::initDatabase() {
    m_db = QSqlDatabase::addDatabase("QSQLITE", "dictionary");
    m_db.setDatabaseName("dictionary.db");

    if (!m_db.open()) {
        //qDebug() << "Cannot open database:" << m_db.lastError().text();
        qDebug() << "Cannot open database:";
        return false;
    }

    if (!createTable()) {
        return false;
    }

    // 如果词库为空，加载默认词库
    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM dictionary");
    if (query.next() && query.value(0).toInt() == 0) 
    {
        loadDefaultDictionary();
    }

    return true;
}

bool InputSuggestion::createTable() {
    QSqlQuery query(m_db);
    return query.exec(
        "CREATE TABLE IF NOT EXISTS dictionary ("
        "word TEXT PRIMARY KEY,"
        "frequency INTEGER DEFAULT 1,"
        "last_used DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    );
}

QVector<QString> InputSuggestion::getSuggestions(const QString& input, int maxResults) {
    QVector<QString> suggestions;

    if (input.isEmpty() || !m_db.isOpen()) {
        return suggestions;
    }

    QSqlQuery query(m_db);
    query.prepare(
        "SELECT word FROM dictionary "
        "WHERE word LIKE :pattern "
        "ORDER BY LENGTH(word) ASC, frequency DESC, last_used DESC " // 按字数升序，然后是频率和最后使用时间降序
        "LIMIT :limit"
    );


    query.bindValue(":pattern", input + "%");
    query.bindValue(":limit", maxResults);

    if (query.exec()) {
        while (query.next()) {
            suggestions.append(query.value(0).toString());
        }
    }

    return suggestions;
}

bool InputSuggestion::addWord(const QString& word, int frequency) {
    QSqlQuery query(m_db);
    query.prepare(
        "INSERT OR REPLACE INTO dictionary (word, frequency) "
        "VALUES (:word, :frequency)"
    );

    query.bindValue(":word", word);
    query.bindValue(":frequency", frequency);

    return query.exec();
}

bool InputSuggestion::updateFrequency(const QString& word) {
    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE dictionary "
        "SET frequency = frequency + 1, "
        "last_used = CURRENT_TIMESTAMP "
        "WHERE word = :word"
    );

    query.bindValue(":word", word);

    return query.exec();
}

bool InputSuggestion::loadDefaultDictionary() {
    // 这里可以从文件加载默认词库
    QFile file(":/dictionary/default.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) 
    {
        qDebug() << "File error:" << file.errorString();
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    m_db.transaction();

    while (!in.atEnd()) {
        QString line = in.readLine();
        addWord(line.trimmed());
    }

    m_db.commit();
    file.close();

    return true;
}
bool InputSuggestion::resetDatabase() {
    if (m_db.isOpen()) {
        m_db.close();
    }
    QFile::remove("dictionary.db");
    return initDatabase();
}