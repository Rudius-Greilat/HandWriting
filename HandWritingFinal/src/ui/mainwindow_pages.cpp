#include "mainwindow.h"
#include "ui_mainwindow.h"
//切换页面按键
void MainWindow::initPageButtons()
{
    // 设置按钮样式表
    QString normalStyle = "QPushButton { "
        "    background-color: white; "
        "    border: 1px solid #CCCCCC; "
        "}";

    QString activeStyle = "QPushButton { "
        "    background-color: #90EE90; "  // 浅绿色
        "    border: 1px solid #2E8B57; "  // 深绿色边框
        "}";

    // 存储所有页面切换按钮的列表
    pageButtons = {
        ui->Btn_HandWriting,
        ui->Btn_en,
        ui->Btn_num,
        ui->Btn_sym
    };

    // 设置所有按钮的初始样式
    for (QPushButton* btn : pageButtons) {
        btn->setStyleSheet(normalStyle);
    }

    // 设置初始页面对应的按钮为激活状态
    pageButtons[0]->setStyleSheet(activeStyle);

    // 连接按钮点击信号
    connect(ui->Btn_HandWriting, &QPushButton::clicked, this, [=]() {
        updatePageButtonStyles(0);
        ui->stackedWidget->setCurrentWidget(ui->page_handwrting);
        });

    connect(ui->Btn_en, &QPushButton::clicked, this, [=]() {
        updatePageButtonStyles(1);
        ui->stackedWidget->setCurrentWidget(ui->page_en);
        });

    connect(ui->Btn_num, &QPushButton::clicked, this, [=]() {
        updatePageButtonStyles(2);
        ui->stackedWidget->setCurrentWidget(ui->page_num);
        });

    connect(ui->Btn_sym, &QPushButton::clicked, this, [=]() {
        updatePageButtonStyles(3);
        ui->stackedWidget->setCurrentWidget(ui->page_sym);
        });
}
//设置数字按钮
void MainWindow::createNumberButtons()
{
    QWidget* pageNum = ui->page_num;
    if (!pageNum) {
        qDebug() << "page_num not found!";
        return;
    }

    QGridLayout* layout = new QGridLayout(pageNum);
    layout->setSpacing(1);  // 设置按钮之间的间距最小
    layout->setContentsMargins(1, 1, 1, 1);  // 设置边距最小

    // 按照图片顺序创建按钮：7-8-9, 4-5-6, 1-2-3, +/--0--. 
    const QVector<QString> buttonTexts = {
        "7", "8", "9",
        "4", "5", "6",
        "1", "2", "3",
        "+", "0", "."
    };

    for (int i = 0; i < buttonTexts.size(); ++i) {
        QPushButton* button = new QPushButton(buttonTexts[i], pageNum);

        // 设置按钮样式，让按钮更方正
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setMinimumSize(50, 50);  // 设置最小大小

        // 设置统一的样式表
        button->setStyleSheet(
            "QPushButton {"
            "   border: 1px solid #CCCCCC;"
            "   background-color: white;"
            //"   font-size: 16px;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #E6E6E6;"
            "}"
        );

        int row = i / 3;    // 计算行位置
        int col = i % 3;    // 计算列位置
        layout->addWidget(button, row, col);

        // 连接信号槽
        connect(button, &QPushButton::clicked, [this, button]() {
            HWND hwndFocused = GetForegroundWindow();
            std::wstring wstr = button->text().toStdWString();
            SendKeys(hwndFocused, wstr);
            });
    }

    // 设置行列的拉伸因子，使按钮均匀分布
    for (int i = 0; i < 4; ++i) {
        layout->setRowStretch(i, 1);
    }
    for (int i = 0; i < 3; ++i) {
        layout->setColumnStretch(i, 1);
    }

    pageNum->setLayout(layout);
}
//设置字符
void MainWindow::createSymbolButtons()
{
    QWidget* pageSym = ui->page_sym;
    if (!pageSym) {
        qDebug() << "page_sym not found!";
        return;
    }

    QGridLayout* layout = new QGridLayout(pageSym);
    layout->setSpacing(0);  // 移除间距
    layout->setContentsMargins(0, 0, 0, 0);  // 移除边距

    QList<QString> symbols;
    // 第一行
    symbols << QString(QChar(0xFF0C))  // 中文逗号
        << QString(QChar(0x3002))  // 中文句号
        << QString(QChar(0x3001))  // 中文顿号
        << QString(QChar(0xFF1B))  // 中文分号
        << QString(QChar(0xFF1A))  // 中文冒号
        << QString(QChar(0xFF1F))  // 中文问号
        << QString(QChar(0xFF01))  // 中文感叹号
        << QString(QChar(0x00B7))  // 中点
        << "...";                  // 省略号

 // 第二行
    symbols << QString(QChar(0x2014))  // 破折号
        << QString(QChar(0xFF5E))  // 波浪号
        << QString(QChar(0xFF08))  // 中文左括号
        << QString(QChar(0xFF09))  // 中文右括号
        << QString(QChar(0x2018))  // 中文左单引号
        << QString(QChar(0x2019))  // 中文右单引号
        << QString(QChar(0x201C))  // 中文左双引号
        << QString(QChar(0x201D))  // 中文右双引号
        << QString(QChar(0x2013));  // 破折短号

 // 第三行
    symbols << QString(QChar(0x002C))  // 英文逗号
        << QString(QChar(0x002E))  // 英文句号
        << QString(QChar(0x0027))  // 英文单引号
        << QString(QChar(0x0022))  // 英文双引号
        << QString(QChar(0x0021))  // 英文感叹号
        << QString(QChar(0x003A))  // 英文冒号
        << QString(QChar(0x003B))  // 英文分号
        << QString(QChar(0x003F))  // 英文问号
        << QString(QChar(0x005C));  // 反斜杠

 // 第四行
    symbols << QString(QChar(0x3010))  // 中文左中括号
        << QString(QChar(0x3011))  // 中文右中括号
        << QString(QChar(0x005B))  // 英文左中括号
        << QString(QChar(0x005D))  // 英文右中括号
        << QString(QChar(0x007B))  // 英文左大括号
        << QString(QChar(0x007D))  // 英文右大括号
        << QString(QChar(0x002B))  // 加号
        << QString(QChar(0x2212))  // 减号
        << QString(QChar(0x002F));  // 斜杠

    int row = 0;
    int col = 0;

    for (const QString& symbol : symbols) {
        QPushButton* button = new QPushButton(symbol, pageSym);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QFont font = button->font();
        font.setFamily("Microsoft YaHei");
        //font.setPointSize(12);
        button->setFont(font);

        button->setStyleSheet(
            "QPushButton {"
            "   border: 1px solid #CCCCCC;"
            "   background-color: white;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #E6E6E6;"
            "}"
        );

        layout->addWidget(button, row, col);

        connect(button, &QPushButton::clicked, [this, button]() {
            HWND hwndFocused = GetForegroundWindow();
            std::wstring wstr = button->text().toStdWString();
            SendKeys(hwndFocused, wstr);
            });

        col++;
        if (col >= 9) {
            col = 0;
            row++;
        }
    }

    // 设置行列的拉伸因子
    for (int i = 0; i < 4; ++i) {
        layout->setRowStretch(i, 1);
    }
    for (int i = 0; i < 9; ++i) {
        layout->setColumnStretch(i, 1);
    }

    pageSym->setLayout(layout);
}
//设置英文字母
void MainWindow::createEnglishButtons()
{
    QWidget* pageEn = ui->page_en;
    if (!pageEn) {
        qDebug() << "page_en not found!";
        return;
    }

    QGridLayout* layout = new QGridLayout(pageEn);
    layout->setSpacing(1);
    layout->setContentsMargins(1, 1, 1, 1);

    // 定义大写字母
    QList<QString> upperLetters = {
        "A", "B", "C", "D", "E", "F", "G",
        "H", "I", "J", "K", "L", "M", "N",
        "O", "P", "Q", "R", "S", "T", "U",
        "V", "W", "X", "Y", "Z"
    };

    // 定义小写字母
    QList<QString> lowerLetters = {
        "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s", "t", "u",
        "v", "w", "x", "y", "z"
    };

    // 创建大写字母按钮 (前4行)
    int row = 0, col = 0;
    for (const QString& letter : upperLetters) {
        QPushButton* button = new QPushButton(letter, pageEn);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setStyleSheet(
            "QPushButton {"
            "   border: 1px solid #CCCCCC;"
            "   background-color: white;"
            //"   font-size: 16px;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #E6E6E6;"
            "}"
        );

        layout->addWidget(button, row, col);

        // 连接信号槽
        connect(button, &QPushButton::clicked, [this, button]() {
            HWND hwndFocused = GetForegroundWindow();
            std::wstring wstr = button->text().toStdWString();
            SendKeys(hwndFocused, wstr);
            });

        col++;
        if (col >= 7) {
            col = 0;
            row++;
        }
    }

    // 创建小写字母按钮 (后4行)
    row = 4; col = 0;  // 从第5行开始
    for (const QString& letter : lowerLetters) {
        QPushButton* button = new QPushButton(letter, pageEn);
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setStyleSheet(
            "QPushButton {"
            "   border: 1px solid #CCCCCC;"
            "   background-color: white;"
            //"   font-size: 16px;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #E6E6E6;"
            "}"
        );

        layout->addWidget(button, row, col);

        // 连接信号槽
        connect(button, &QPushButton::clicked, [this, button]() {
            HWND hwndFocused = GetForegroundWindow();
            std::wstring wstr = button->text().toStdWString();
            SendKeys(hwndFocused, wstr);
            });

        col++;
        if (col >= 7) {
            col = 0;
            row++;
        }
    }

    // 设置行列的拉伸因子
    for (int i = 0; i < 8; ++i) {
        layout->setRowStretch(i, 1);
        if (i < 7) layout->setColumnStretch(i, 1);
    }

    pageEn->setLayout(layout);
}


void MainWindow::updatePageButtonStyles(int activeIndex)
{
    if(activeIndex)
        on_Btn_clear_clicked();

    QString normalStyle = "QPushButton { "
        "    background-color: white; "
        "    border: 1px solid #CCCCCC; "
        "}";

    QString activeStyle = "QPushButton { "
        "    background-color: #90EE90; "  // 浅绿色
        "    border: 1px solid #2E8B57; "  // 深绿色边框
        "}";

    // 更新所有按钮样式
    for (int i = 0; i < pageButtons.size(); ++i) {
        pageButtons[i]->setStyleSheet(i == activeIndex ? activeStyle : normalStyle);
    }
}