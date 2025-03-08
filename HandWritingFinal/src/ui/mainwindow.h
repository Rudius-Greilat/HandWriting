#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMouseEvent>
#include <QRect>
#include <QTimer>
#include <QTextCursor>
#include <QDebug>
#include <QVBoxLayout>  // 使用垂直布局
#include <QGridLayout>  // 或者使用网格布局
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

    // 切换页面
    void on_Btn_setting_clicked();
    // 手写处理
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
    InputSuggestion* m_suggestion;//存储联想结果
    QString m_lastInput;          // 记录上一次输入的字符
    bool Lenovo = false;          //联想开关
    bool DatabaseInit = false;    //数据库初始化标记
    QVector<QPushButton*> pageButtons;//切换按钮
    // 生成其他页面
    void createNumberButtons();
    void createSymbolButtons();
    void createEnglishButtons();
    void initSettingWidget();
    // 初始化切换按键和更新状态
    void initPageButtons();            
    void updatePageButtonStyles(int activeIndex);
    // 设定
    void applySettings(int fontSize, QColor penColor, int penWidth);//应用设置
    void syncCurrentSettings();//还原设置界面
    // 手写输入
    void input_word(QString new_word);
    void SendKeys(HWND hwnd, const std::wstring& text);
    void recognizeHand();
};

#endif