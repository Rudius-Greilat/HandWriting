#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //��������ǰ�Ҳ����ό��
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::WindowMinMaxButtonsHint);
    setFixedSize(size());
    // ʹ�� Windows API ������չ��ʽ
    HWND hwnd = (HWND)this->winId();  // ��ȡ���ھ��
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE); // ��ȡ��ǰ��չ��ʽ
    if (exStyle != 0) {
        exStyle |= 0x8000000; // ��� WS_EX_NOACTIVATE ��ʽ
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle); // ������չ��ʽ
    }

    // ��ʼ����д��
    WId hShow = ui->label->winId();
    if (!inputs.inputInit((HWND)hShow)) {  // ���� inputInit ���� bool ��ʾ�ɹ�/ʧ��
        QMessageBox::critical(this, u8"����", u8"��д���ʼ��ʧ��");
        exit(1);  // ֱ�ӽ�������
        return;
    }

    //��ʼ����������ݿ�
    m_suggestion = new InputSuggestion(this);
    qDebug() << "database init starting";
    if (m_suggestion->initDatabase())
    {
        DatabaseInit = true;
        qDebug() << "database init successfully";
    }
    else
        QMessageBox::critical(this, u8"����", u8"���ݿ��ʼ��ʧ��,���빦��ʧЧ");
    
    //��дѡ�����󶨲ۺ���
    QList<QPushButton*> HandWritingButtons =
    {
        ui->Btn_word1,ui->Btn_word2,ui->Btn_word3,ui->Btn_word4,ui->Btn_word5,ui->Btn_word6,ui->Btn_word7,ui->Btn_word8,ui->Btn_word9
    };
    for (QPushButton* button : HandWritingButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::on_HandWritingClicked);
    }

    //��ʼ������ҳ��
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
