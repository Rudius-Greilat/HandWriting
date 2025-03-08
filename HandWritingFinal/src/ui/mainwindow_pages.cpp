#include "mainwindow.h"
#include "ui_mainwindow.h"
//�л�ҳ�水��
void MainWindow::initPageButtons()
{
    // ���ð�ť��ʽ��
    QString normalStyle = "QPushButton { "
        "    background-color: white; "
        "    border: 1px solid #CCCCCC; "
        "}";

    QString activeStyle = "QPushButton { "
        "    background-color: #90EE90; "  // ǳ��ɫ
        "    border: 1px solid #2E8B57; "  // ����ɫ�߿�
        "}";

    // �洢����ҳ���л���ť���б�
    pageButtons = {
        ui->Btn_HandWriting,
        ui->Btn_en,
        ui->Btn_num,
        ui->Btn_sym
    };

    // �������а�ť�ĳ�ʼ��ʽ
    for (QPushButton* btn : pageButtons) {
        btn->setStyleSheet(normalStyle);
    }

    // ���ó�ʼҳ���Ӧ�İ�ťΪ����״̬
    pageButtons[0]->setStyleSheet(activeStyle);

    // ���Ӱ�ť����ź�
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
//�������ְ�ť
void MainWindow::createNumberButtons()
{
    QWidget* pageNum = ui->page_num;
    if (!pageNum) {
        qDebug() << "page_num not found!";
        return;
    }

    QGridLayout* layout = new QGridLayout(pageNum);
    layout->setSpacing(1);  // ���ð�ť֮��ļ����С
    layout->setContentsMargins(1, 1, 1, 1);  // ���ñ߾���С

    // ����ͼƬ˳�򴴽���ť��7-8-9, 4-5-6, 1-2-3, +/--0--. 
    const QVector<QString> buttonTexts = {
        "7", "8", "9",
        "4", "5", "6",
        "1", "2", "3",
        "+", "0", "."
    };

    for (int i = 0; i < buttonTexts.size(); ++i) {
        QPushButton* button = new QPushButton(buttonTexts[i], pageNum);

        // ���ð�ť��ʽ���ð�ť������
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setMinimumSize(50, 50);  // ������С��С

        // ����ͳһ����ʽ��
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

        int row = i / 3;    // ������λ��
        int col = i % 3;    // ������λ��
        layout->addWidget(button, row, col);

        // �����źŲ�
        connect(button, &QPushButton::clicked, [this, button]() {
            HWND hwndFocused = GetForegroundWindow();
            std::wstring wstr = button->text().toStdWString();
            SendKeys(hwndFocused, wstr);
            });
    }

    // �������е��������ӣ�ʹ��ť���ȷֲ�
    for (int i = 0; i < 4; ++i) {
        layout->setRowStretch(i, 1);
    }
    for (int i = 0; i < 3; ++i) {
        layout->setColumnStretch(i, 1);
    }

    pageNum->setLayout(layout);
}
//�����ַ�
void MainWindow::createSymbolButtons()
{
    QWidget* pageSym = ui->page_sym;
    if (!pageSym) {
        qDebug() << "page_sym not found!";
        return;
    }

    QGridLayout* layout = new QGridLayout(pageSym);
    layout->setSpacing(0);  // �Ƴ����
    layout->setContentsMargins(0, 0, 0, 0);  // �Ƴ��߾�

    QList<QString> symbols;
    // ��һ��
    symbols << QString(QChar(0xFF0C))  // ���Ķ���
        << QString(QChar(0x3002))  // ���ľ��
        << QString(QChar(0x3001))  // ���Ķٺ�
        << QString(QChar(0xFF1B))  // ���ķֺ�
        << QString(QChar(0xFF1A))  // ����ð��
        << QString(QChar(0xFF1F))  // �����ʺ�
        << QString(QChar(0xFF01))  // ���ĸ�̾��
        << QString(QChar(0x00B7))  // �е�
        << "...";                  // ʡ�Ժ�

 // �ڶ���
    symbols << QString(QChar(0x2014))  // ���ۺ�
        << QString(QChar(0xFF5E))  // ���˺�
        << QString(QChar(0xFF08))  // ����������
        << QString(QChar(0xFF09))  // ����������
        << QString(QChar(0x2018))  // ����������
        << QString(QChar(0x2019))  // �����ҵ�����
        << QString(QChar(0x201C))  // ������˫����
        << QString(QChar(0x201D))  // ������˫����
        << QString(QChar(0x2013));  // ���۶̺�

 // ������
    symbols << QString(QChar(0x002C))  // Ӣ�Ķ���
        << QString(QChar(0x002E))  // Ӣ�ľ��
        << QString(QChar(0x0027))  // Ӣ�ĵ�����
        << QString(QChar(0x0022))  // Ӣ��˫����
        << QString(QChar(0x0021))  // Ӣ�ĸ�̾��
        << QString(QChar(0x003A))  // Ӣ��ð��
        << QString(QChar(0x003B))  // Ӣ�ķֺ�
        << QString(QChar(0x003F))  // Ӣ���ʺ�
        << QString(QChar(0x005C));  // ��б��

 // ������
    symbols << QString(QChar(0x3010))  // ������������
        << QString(QChar(0x3011))  // ������������
        << QString(QChar(0x005B))  // Ӣ����������
        << QString(QChar(0x005D))  // Ӣ����������
        << QString(QChar(0x007B))  // Ӣ���������
        << QString(QChar(0x007D))  // Ӣ���Ҵ�����
        << QString(QChar(0x002B))  // �Ӻ�
        << QString(QChar(0x2212))  // ����
        << QString(QChar(0x002F));  // б��

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

    // �������е���������
    for (int i = 0; i < 4; ++i) {
        layout->setRowStretch(i, 1);
    }
    for (int i = 0; i < 9; ++i) {
        layout->setColumnStretch(i, 1);
    }

    pageSym->setLayout(layout);
}
//����Ӣ����ĸ
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

    // �����д��ĸ
    QList<QString> upperLetters = {
        "A", "B", "C", "D", "E", "F", "G",
        "H", "I", "J", "K", "L", "M", "N",
        "O", "P", "Q", "R", "S", "T", "U",
        "V", "W", "X", "Y", "Z"
    };

    // ����Сд��ĸ
    QList<QString> lowerLetters = {
        "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s", "t", "u",
        "v", "w", "x", "y", "z"
    };

    // ������д��ĸ��ť (ǰ4��)
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

        // �����źŲ�
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

    // ����Сд��ĸ��ť (��4��)
    row = 4; col = 0;  // �ӵ�5�п�ʼ
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

        // �����źŲ�
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

    // �������е���������
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
        "    background-color: #90EE90; "  // ǳ��ɫ
        "    border: 1px solid #2E8B57; "  // ����ɫ�߿�
        "}";

    // �������а�ť��ʽ
    for (int i = 0; i < pageButtons.size(); ++i) {
        pageButtons[i]->setStyleSheet(i == activeIndex ? activeStyle : normalStyle);
    }
}