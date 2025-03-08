#ifndef INPUTSUGGESTION_H
#define INPUTSUGGESTION_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>

class InputSuggestion : public QObject {
    Q_OBJECT
public:
    explicit InputSuggestion(QObject* parent = nullptr);
    ~InputSuggestion();

    // 初始化词库数据库
    bool initDatabase();
    //重置数据库（测试用）
    bool resetDatabase();
    // 获取联想词
    QVector<QString> getSuggestions(const QString& input, int maxResults = 10);

    // 添加新词到词库
    bool addWord(const QString& word, int frequency = 1);

    // 更新词频
    bool updateFrequency(const QString& word);

private:
    QSqlDatabase m_db;

    // 创建词库表
    bool createTable();

    // 加载默认词库
    bool loadDefaultDictionary();
   
};

#endif // INPUTSUGGESTION_H