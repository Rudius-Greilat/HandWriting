#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //保持在最前且不抢夺焦点
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::WindowMinMaxButtonsHint);
    setFixedSize(size());
    // 使用 Windows API 设置扩展样式
    HWND hwnd = (HWND)this->winId();  // 获取窗口句柄
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE); // 获取当前扩展样式
    if (exStyle != 0) {
        exStyle |= 0x8000000; // 添加 WS_EX_NOACTIVATE 样式
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle); // 更新扩展样式
    }

    // 初始化手写板
    WId hShow = ui->label->winId();
    if (!inputs.inputInit((HWND)hShow)) {  // 假设 inputInit 返回 bool 表示成功/失败
        QMessageBox::critical(this, u8"错误", u8"手写板初始化失败");
        exit(1);  // 直接结束程序
        return;
    }

    //初始化联想词数据库
    m_suggestion = new InputSuggestion(this);
    qDebug() << "database init starting";
    if (m_suggestion->initDatabase())
    {
        DatabaseInit = true;
        qDebug() << "database init successfully";
    }
    else
        QMessageBox::critical(this, u8"错误", u8"数据库初始化失败,联想功能失效");
    
    //手写选择栏绑定槽函数
    QList<QPushButton*> HandWritingButtons =
    {
        ui->Btn_word1,ui->Btn_word2,ui->Btn_word3,ui->Btn_word4,ui->Btn_word5,ui->Btn_word6,ui->Btn_word7,ui->Btn_word8,ui->Btn_word9
    };
    for (QPushButton* button : HandWritingButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::on_HandWritingClicked);
    }

    //初始化其他页面
    initPageButtons();
    createNumberButtons();
    createSymbolButtons();
    createEnglishButtons();
    initSettingWidget();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    qDebug() << "Starting recognition...";
    recognizeHand();
}
