#include "mainwindow.h"
#include "ui_mainwindow.h"
//��ʼ������
void MainWindow::initSettingWidget()
{
    QIcon icon(":/images/settings.png");
    ui->Btn_setting->setIcon(icon);
    ui->Btn_setting->setIconSize(QSize(20, 20));

    //����ͳһ�����С
    QFont settingsFont;
    settingsFont.setPointSize(12);


    // ���ñ���������
    ui->settingWidget->setRowCount(4);  // 4��
    ui->settingWidget->setColumnCount(2);  // 2��
    ui->settingWidget->verticalHeader()->hide();    // ���ش�ֱ��ͷ
    ui->settingWidget->horizontalHeader()->hide();  // ����ˮƽ��ͷ

    // ���ñ����п���и��Զ�����
    ui->settingWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->settingWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // ������Ͻǹرհ�ť
    QPushButton* closeButton = new QPushButton(u8"��", ui->settingWidget);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: gray;"
        "    border: none;"
        "    font-size: 32px;"
        "}"
        "QPushButton:hover {"
        "    color: red;"
        "}"
    );
    closeButton->setFixedSize(30, 30);
    closeButton->move(ui->settingWidget->width() - 30, 0);
    connect(closeButton, &QPushButton::clicked, ui->settingWidget, &QWidget::hide);


    // �����С���ò���
    QLabel* fontLabel = new QLabel(u8"�����Сѡ��:");
    fontLabel->setFont(settingsFont);
    QComboBox* fontSizeBox = new QComboBox();
    fontSizeBox->addItems({ u8"С", u8"��", u8"��" });
    fontSizeBox->setFont(settingsFont);
    ui->settingWidget->setCellWidget(0, 0, fontLabel);
    ui->settingWidget->setCellWidget(0, 1, fontSizeBox);

    // ��ɫѡ�񲿷�
    QLabel* colorLabel = new QLabel(u8"��ɫѡ��:");
    colorLabel->setFont(settingsFont);
    QWidget* colorContainer = new QWidget();
    QVBoxLayout* colorMainLayout = new QVBoxLayout(colorContainer);
    QHBoxLayout* colorButtonLayout = new QHBoxLayout();
    QButtonGroup* colorGroup = new QButtonGroup(this);

    // ������ɫѡ��͵�ǰ��ɫ��ʾ
    QWidget* currentColorWidget = new QWidget();
    QHBoxLayout* currentColorLayout = new QHBoxLayout(currentColorWidget);
    QLabel* currentColorTextLabel = new QLabel(u8"��ǰѡ��: ");
    currentColorTextLabel->setFont(settingsFont);
    QLabel* currentColorDisplay = new QLabel();
    currentColorDisplay->setFixedSize(20, 20);  // ������ɫ���С
    currentColorDisplay->setStyleSheet("background-color: black;");  // Ĭ�Ϻ�ɫ
    currentColorLayout->addWidget(currentColorTextLabel);
    currentColorLayout->addWidget(currentColorDisplay);
    currentColorLayout->addStretch();
    currentColorLayout->setAlignment(Qt::AlignLeft);

    // �����ѡ��ɫ
    QList<QPair<QString, QColor>> colors = {
        {"Black", Qt::black},
        {"Red", Qt::red},
        {"Green", Qt::green},
        {"Blue", Qt::blue},
        {"Yellow", Qt::yellow}
    };

    // ������ɫѡ��ť
    for (const auto& color : colors) {
        QRadioButton* btn = new QRadioButton();
        btn->setStyleSheet(QString("QRadioButton::indicator { width: 20px; height: 20px; background-color: %1; }")
            .arg(color.second.name()));
        colorButtonLayout->addWidget(btn);
        colorGroup->addButton(btn);

        // ���µ�ǰ��ɫ��ʾ
        connect(btn, &QRadioButton::clicked, [=]() {
            currentColorDisplay->setStyleSheet(QString("background-color: %1;").arg(color.second.name()));
            });
    }
    colorGroup->buttons().first()->setChecked(true);  // Ĭ��ѡ�е�һ������ɫ��

    // ����settingWidget����ʾ�¼�
    connect(ui->settingWidget, &QWidget::windowTitleChanged, this, [=](const QString&) {
        if (ui->settingWidget->isVisible())
        {
            // ��ȡ��ǰʵ��ʹ�õ���ɫ
            IInkDrawingAttributes* attr;
            if (SUCCEEDED(inputs.g_pIInkCollector->get_DefaultDrawingAttributes(&attr)))
            {
                long currentColor;
                attr->get_Color(&currentColor);
                attr->Release();
                // ���ݵ�ǰ��ɫ�ҵ���Ӧ�İ�ť��ѡ��
                for (QAbstractButton* btn : colorGroup->buttons()) {
                    QString style = btn->styleSheet();
                    int start = style.indexOf("background-color:") + 17;
                    int end = style.indexOf(";", start);
                    QString colorName = style.mid(start, end - start);
                    QColor btnColor(colorName.trimmed());
                    // �Ƚ���ɫֵ
                    if (btnColor.red() == GetRValue(currentColor) &&
                        btnColor.green() == GetGValue(currentColor) &&
                        btnColor.blue() == GetBValue(currentColor)) {
                        btn->setChecked(true);
                        currentColorDisplay->setStyleSheet(QString("background-color: %1;").arg(colorName));
                        break;
                    }
                }
            }
        }
        });
    // �ڵ��setting��ťʱ������ʾ
    connect(ui->Btn_setting, &QPushButton::clicked, this, [=]() {
        ui->settingWidget->setWindowTitle(ui->settingWidget->windowTitle());
        });

    colorMainLayout->addLayout(colorButtonLayout);
    colorMainLayout->addWidget(currentColorWidget);
    colorMainLayout->setContentsMargins(0, 0, 0, 0);

    ui->settingWidget->setCellWidget(1, 0, colorLabel);
    ui->settingWidget->setCellWidget(1, 1, colorContainer);

    // �ʻ���ϸ���ò���
    QLabel* widthLabel = new QLabel(u8"�ʻ���ϸ����:");
    widthLabel->setFont(settingsFont);
    QWidget* widthContainer = new QWidget();
    QHBoxLayout* widthLayout = new QHBoxLayout(widthContainer);

    QLabel* thinLabel = new QLabel(u8"ϸ"); thinLabel->setFont(settingsFont);
    QLabel* thickLabel = new QLabel(u8"��"); thickLabel->setFont(settingsFont);
    QSlider* widthSlider = new QSlider(Qt::Horizontal);
    widthSlider->setRange(1, 10);
    widthSlider->setPageStep(1);

    widthLayout->addWidget(thinLabel);
    widthLayout->addWidget(widthSlider);
    widthLayout->addWidget(thickLabel);
    widthLayout->setContentsMargins(0, 0, 0, 0);

    ui->settingWidget->setCellWidget(2, 0, widthLabel);
    ui->settingWidget->setCellWidget(2, 1, widthContainer);


    // ��ť����
    QWidget* resetBtnWidget = new QWidget();
    QHBoxLayout* resetBtnLayout = new QHBoxLayout(resetBtnWidget);
    QPushButton* resetBtn = new QPushButton(u8"�ָ�Ĭ��ֵ");
    resetBtn->setFixedSize(200,50);  // ���ù̶���С
    resetBtnLayout->addWidget(resetBtn);
    resetBtnLayout->setAlignment(Qt::AlignCenter);  // �������о���
    resetBtnLayout->setContentsMargins(0, 0, 0, 0);
    ui->settingWidget->setCellWidget(3, 0, resetBtnWidget);
    resetBtn->setFont(settingsFont);
    // �����ұ߰�ť��ȷ�ϰ�ť���������Ͳ���
    QWidget* confirmBtnWidget = new QWidget();
    QHBoxLayout* confirmBtnLayout = new QHBoxLayout(confirmBtnWidget);
    QPushButton* confirmBtn = new QPushButton(u8"���沢�˳�");
    confirmBtn->setFixedSize(200, 50);  // ���ù̶���С
    confirmBtnLayout->addWidget(confirmBtn);
    confirmBtnLayout->setAlignment(Qt::AlignCenter);  // �������о���
    confirmBtnLayout->setContentsMargins(0, 0, 0, 0);
    ui->settingWidget->setCellWidget(3, 1, confirmBtnWidget);
    confirmBtn->setFont(settingsFont);
    // �����źŲ�
    connect(confirmBtn, &QPushButton::clicked, this, [=]() {
        // ��ȡ�����С����
        int fontSize = 12 + fontSizeBox->currentIndex() * 4;  // s=12, m=16, l=20

        // ��ȡѡ�е���ɫ
        QColor penColor = Qt::black;
        for (QAbstractButton* btn : colorGroup->buttons()) {
            if (btn->isChecked()) {
                QString style = btn->styleSheet();
                int start = style.indexOf("background-color:") + 17;
                int end = style.indexOf(";", start);
                QString colorName = style.mid(start, end - start);
                penColor = QColor(colorName.trimmed());
                break;
            }
        }

        // ��ȡ�ʻ���ϸ
        int penWidth = widthSlider->value();

        // Ӧ������
        applySettings(fontSize, penColor, penWidth);
        ui->settingWidget->hide();
        });

    // �������ú���
    auto resetSettings = [=]() {
        fontSizeBox->setCurrentIndex(0);  // ���������С
        colorGroup->buttons().first()->setChecked(true);  // ������ɫѡ��
        currentColorDisplay->setStyleSheet("background-color: black;");  // ���õ�ǰ��ɫ��ʾ
        widthSlider->setValue(2);  // ���ñʻ���ϸ

        // ����Ӧ��Ĭ������
        applySettings(12, Qt::black, 2);
    };

    // ���ð�ť����
    connect(resetBtn, &QPushButton::clicked, this, resetSettings);

    // ��ʼ��ʱ����һ������
    resetSettings();
    // Ĭ���������ô���
    ui->settingWidget->hide();
}
//��������
void MainWindow::applySettings(int fontSize, QColor penColor, int penWidth)
{
    // Ӧ�ñʼ�����
    if (inputs.g_pIInkCollector) {
        IInkDrawingAttributes* attr;
        if (SUCCEEDED(inputs.g_pIInkCollector->get_DefaultDrawingAttributes(&attr))) {
            // ������ɫ
            attr->put_Color(RGB(penColor.red(), penColor.green(), penColor.blue()));

            // ���ñʼ���ȣ����ݻ�����ֵӳ�䵽���ʵıʼ���С��Χ
            float scaledWidth = penWidth * 20.5f;  // ��1-10�ķ�Χӳ�䵽�����ʵıʼ����
            attr->put_Width(scaledWidth);
            attr->put_Height(scaledWidth);  // ͬʱ���ø߶�ȷ���ʼ���ϸһ��

            attr->Release();
        }
    }
    on_Btn_clear_clicked();

    // ������������������С
    QFont newFont;
    newFont.setPointSize(fontSize);

    // ������дѡ��ť����
    QList<QPushButton*> handwritingButtons = {
        ui->Btn_word1, ui->Btn_word2, ui->Btn_word3,
        ui->Btn_word4, ui->Btn_word5, ui->Btn_word6,
        ui->Btn_word7, ui->Btn_word8, ui->Btn_word9
    };
    for (auto btn : handwritingButtons) {
        btn->setFont(newFont);
    }

    // ���¹��ܰ�ť����
    ui->Btn_CtrlZ->setFont(newFont);
    ui->Btn_clear->setFont(newFont);
    ui->Btn_HandWriting->setFont(newFont);
    ui->Btn_en->setFont(newFont);
    ui->Btn_num->setFont(newFont);
    ui->Btn_sym->setFont(newFont);
    ui->Btn_setting->setFont(newFont);
    ui->Multiple_words->setFont(newFont);
    ui->Lenovo_words->setFont(newFont);
    QWidget* pageNum = ui->page_num;
    if (pageNum) {
        QList<QPushButton*> numButtons = pageNum->findChildren<QPushButton*>();
        for (auto btn : numButtons) {
            btn->setFont(newFont);
        }
    }

    // ���¶�̬������Ӣ�İ�ť����
    QWidget* pageEn = ui->page_en;
    if (pageEn) {
        QList<QPushButton*> enButtons = pageEn->findChildren<QPushButton*>();
        for (auto btn : enButtons) {
            btn->setFont(newFont);
        }
    }

    // ���¶�̬�����ķ��Ű�ť����
    QWidget* pageSym = ui->page_sym;
    if (pageSym) {
        QList<QPushButton*> symButtons = pageSym->findChildren<QPushButton*>();
        for (auto btn : symButtons) {
            btn->setFont(newFont);
        }
    }

}
//��ԭ���Ķ���û�����ѡ��
void MainWindow::syncCurrentSettings()
{
    // 1. ��ȡ��ǰ�ʻ���ϸ
    IInkDrawingAttributes* attr;
    if (SUCCEEDED(inputs.g_pIInkCollector->get_DefaultDrawingAttributes(&attr)))
    {
        // ��ȡ�ʻ����
        float currentWidth;
        attr->get_Width(&currentWidth);
        // �ͷ���Դ
        attr->Release();
        // ͬ���ʻ���ϸ������
        QSlider* widthSlider = nullptr;
        QWidget* widthContainer = ui->settingWidget->cellWidget(2, 1);
        if (widthContainer) {
            widthSlider = widthContainer->findChild<QSlider*>();
            if (widthSlider) {
                // ��ʵ�ʿ��ת���ػ�����ֵ��1-10��
                int sliderValue = qRound(currentWidth / 20.5f);
                sliderValue = qBound(1, sliderValue, 10);
                widthSlider->setValue(sliderValue);
            }
        }
    }

    // 2. ��ȡ�����С������������
    QComboBox* fontSizeBox = nullptr;
    QWidget* fontContainer = ui->settingWidget->cellWidget(0, 1);
    if (fontContainer && (fontSizeBox = qobject_cast<QComboBox*>(fontContainer))) {
        // ��ȡ��ǰ�����С
        QPushButton* anyButton = ui->Btn_word1;  // ʹ������һ����ť��Ϊ�ο�
        if (anyButton) {
            int currentSize = anyButton->font().pointSize();
            // �������Сת��Ϊ��Ӧ��������12->0, 16->1, 20->2��
            int index = (currentSize - 12) / 4;
            fontSizeBox->setCurrentIndex(qBound(0, index, 2));
        }
    }
}

void MainWindow::on_Btn_setting_clicked()
{
    if (ui->settingWidget->isHidden())
    {
        syncCurrentSettings();
        on_Btn_clear_clicked();
        ui->settingWidget->show();
    }

    else
        ui->settingWidget->hide();
}