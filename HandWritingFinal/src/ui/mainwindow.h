#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QRect>
#include <QTimer>
#include <QTextCursor>
#include <QDebug>
#include <QVBoxLayout>  // ʹ�ô�ֱ����
#include <QGridLayout>  // ����ʹ�����񲼾�
#include<QComboBox>
#include<iostream>
#include<QRadioButton>
#include<QButtonGroup>
#include <QMessageBox>
#include "core/handsinput.h"
#include "core/inputsuggestion.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;

private slots:

    // �л�ҳ��
    void on_Btn_setting_clicked();
    // ��д����
    void on_HandWritingClicked();
    void on_Multiple_words_stateChanged(int state);
    void on_Btn_clear_clicked();
    void on_Btn_CtrlZ_clicked();
    void on_Lenovo_words_stateChanged(int state);
private:
    // Core UI
    Ui::MainWindow* ui;
    handsInput inputs;
    InputResults tempResult; 
    InputSuggestion* m_suggestion;//�洢������
    QString m_lastInput;          // ��¼��һ��������ַ�
    bool Lenovo = false;          //���뿪��
    bool DatabaseInit = false;    //���ݿ��ʼ�����
    QVector<QPushButton*> pageButtons;//�л���ť
    // ��������ҳ��
    void createNumberButtons();
    void createSymbolButtons();
    void createEnglishButtons();
    void initSettingWidget();
    // ��ʼ���л������͸���״̬
    void initPageButtons();            
    void updatePageButtonStyles(int activeIndex);
    // �趨
    void applySettings(int fontSize, QColor penColor, int penWidth);//Ӧ������
    void syncCurrentSettings();//��ԭ���ý���
    // ��д����
    void input_word(QString new_word);
    void SendKeys(HWND hwnd, const std::wstring& text);
    void recognizeHand();
};

#endif