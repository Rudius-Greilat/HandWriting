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

    // ��ʼ���ʿ����ݿ�
    bool initDatabase();
    //�������ݿ⣨�����ã�
    bool resetDatabase();
    // ��ȡ�����
    QVector<QString> getSuggestions(const QString& input, int maxResults = 10);

    // ����´ʵ��ʿ�
    bool addWord(const QString& word, int frequency = 1);

    // ���´�Ƶ
    bool updateFrequency(const QString& word);

private:
    QSqlDatabase m_db;

    // �����ʿ��
    bool createTable();

    // ����Ĭ�ϴʿ�
    bool loadDefaultDictionary();
   
};

#endif // INPUTSUGGESTION_H