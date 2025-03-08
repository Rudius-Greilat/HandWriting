#include "mainwindow.h"
#include "ui_mainwindow.h"
//初始化设置
void MainWindow::initSettingWidget()
{
    QIcon icon(":/images/settings.png");
    ui->Btn_setting->setIcon(icon);
    ui->Btn_setting->setIconSize(QSize(20, 20));

    //设置统一字体大小
    QFont settingsFont;
    settingsFont.setPointSize(12);


    // 设置表格基本属性
    ui->settingWidget->setRowCount(4);  // 4行
    ui->settingWidget->setColumnCount(2);  // 2列
    ui->settingWidget->verticalHeader()->hide();    // 隐藏垂直表头
    ui->settingWidget->horizontalHeader()->hide();  // 隐藏水平表头

    // 设置表格的列宽和行高自动拉伸
    ui->settingWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->settingWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 添加右上角关闭按钮
    QPushButton* closeButton = new QPushButton(u8"×", ui->settingWidget);
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


    // 字体大小设置部分
    QLabel* fontLabel = new QLabel(u8"字体大小选择:");
    fontLabel->setFont(settingsFont);
    QComboBox* fontSizeBox = new QComboBox();
    fontSizeBox->addItems({ u8"小", u8"中", u8"大" });
    fontSizeBox->setFont(settingsFont);
    ui->settingWidget->setCellWidget(0, 0, fontLabel);
    ui->settingWidget->setCellWidget(0, 1, fontSizeBox);

    // 颜色选择部分
    QLabel* colorLabel = new QLabel(u8"颜色选择:");
    colorLabel->setFont(settingsFont);
    QWidget* colorContainer = new QWidget();
    QVBoxLayout* colorMainLayout = new QVBoxLayout(colorContainer);
    QHBoxLayout* colorButtonLayout = new QHBoxLayout();
    QButtonGroup* colorGroup = new QButtonGroup(this);

    // 创建颜色选项和当前颜色显示
    QWidget* currentColorWidget = new QWidget();
    QHBoxLayout* currentColorLayout = new QHBoxLayout(currentColorWidget);
    QLabel* currentColorTextLabel = new QLabel(u8"当前选中: ");
    currentColorTextLabel->setFont(settingsFont);
    QLabel* currentColorDisplay = new QLabel();
    currentColorDisplay->setFixedSize(20, 20);  // 设置颜色块大小
    currentColorDisplay->setStyleSheet("background-color: black;");  // 默认黑色
    currentColorLayout->addWidget(currentColorTextLabel);
    currentColorLayout->addWidget(currentColorDisplay);
    currentColorLayout->addStretch();
    currentColorLayout->setAlignment(Qt::AlignLeft);

    // 定义可选颜色
    QList<QPair<QString, QColor>> colors = {
        {"Black", Qt::black},
        {"Red", Qt::red},
        {"Green", Qt::green},
        {"Blue", Qt::blue},
        {"Yellow", Qt::yellow}
    };

    // 创建颜色选择按钮
    for (const auto& color : colors) {
        QRadioButton* btn = new QRadioButton();
        btn->setStyleSheet(QString("QRadioButton::indicator { width: 20px; height: 20px; background-color: %1; }")
            .arg(color.second.name()));
        colorButtonLayout->addWidget(btn);
        colorGroup->addButton(btn);

        // 更新当前颜色显示
        connect(btn, &QRadioButton::clicked, [=]() {
            currentColorDisplay->setStyleSheet(QString("background-color: %1;").arg(color.second.name()));
            });
    }
    colorGroup->buttons().first()->setChecked(true);  // 默认选中第一个（黑色）

    // 监听settingWidget的显示事件
    connect(ui->settingWidget, &QWidget::windowTitleChanged, this, [=](const QString&) {
        if (ui->settingWidget->isVisible())
        {
            // 获取当前实际使用的颜色
            IInkDrawingAttributes* attr;
            if (SUCCEEDED(inputs.g_pIInkCollector->get_DefaultDrawingAttributes(&attr)))
            {
                long currentColor;
                attr->get_Color(&currentColor);
                attr->Release();
                // 根据当前颜色找到对应的按钮并选中
                for (QAbstractButton* btn : colorGroup->buttons()) {
                    QString style = btn->styleSheet();
                    int start = style.indexOf("background-color:") + 17;
                    int end = style.indexOf(";", start);
                    QString colorName = style.mid(start, end - start);
                    QColor btnColor(colorName.trimmed());
                    // 比较颜色值
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
    // 在点击setting按钮时更新显示
    connect(ui->Btn_setting, &QPushButton::clicked, this, [=]() {
        ui->settingWidget->setWindowTitle(ui->settingWidget->windowTitle());
        });

    colorMainLayout->addLayout(colorButtonLayout);
    colorMainLayout->addWidget(currentColorWidget);
    colorMainLayout->setContentsMargins(0, 0, 0, 0);

    ui->settingWidget->setCellWidget(1, 0, colorLabel);
    ui->settingWidget->setCellWidget(1, 1, colorContainer);

    // 笔画粗细设置部分
    QLabel* widthLabel = new QLabel(u8"笔画粗细调节:");
    widthLabel->setFont(settingsFont);
    QWidget* widthContainer = new QWidget();
    QHBoxLayout* widthLayout = new QHBoxLayout(widthContainer);

    QLabel* thinLabel = new QLabel(u8"细"); thinLabel->setFont(settingsFont);
    QLabel* thickLabel = new QLabel(u8"粗"); thickLabel->setFont(settingsFont);
    QSlider* widthSlider = new QSlider(Qt::Horizontal);
    widthSlider->setRange(1, 10);
    widthSlider->setPageStep(1);

    widthLayout->addWidget(thinLabel);
    widthLayout->addWidget(widthSlider);
    widthLayout->addWidget(thickLabel);
    widthLayout->setContentsMargins(0, 0, 0, 0);

    ui->settingWidget->setCellWidget(2, 0, widthLabel);
    ui->settingWidget->setCellWidget(2, 1, widthContainer);


    // 按钮区域
    QWidget* resetBtnWidget = new QWidget();
    QHBoxLayout* resetBtnLayout = new QHBoxLayout(resetBtnWidget);
    QPushButton* resetBtn = new QPushButton(u8"恢复默认值");
    resetBtn->setFixedSize(200,50);  // 设置固定大小
    resetBtnLayout->addWidget(resetBtn);
    resetBtnLayout->setAlignment(Qt::AlignCenter);  // 在容器中居中
    resetBtnLayout->setContentsMargins(0, 0, 0, 0);
    ui->settingWidget->setCellWidget(3, 0, resetBtnWidget);
    resetBtn->setFont(settingsFont);
    // 创建右边按钮（确认按钮）的容器和布局
    QWidget* confirmBtnWidget = new QWidget();
    QHBoxLayout* confirmBtnLayout = new QHBoxLayout(confirmBtnWidget);
    QPushButton* confirmBtn = new QPushButton(u8"保存并退出");
    confirmBtn->setFixedSize(200, 50);  // 设置固定大小
    confirmBtnLayout->addWidget(confirmBtn);
    confirmBtnLayout->setAlignment(Qt::AlignCenter);  // 在容器中居中
    confirmBtnLayout->setContentsMargins(0, 0, 0, 0);
    ui->settingWidget->setCellWidget(3, 1, confirmBtnWidget);
    confirmBtn->setFont(settingsFont);
    // 连接信号槽
    connect(confirmBtn, &QPushButton::clicked, this, [=]() {
        // 获取字体大小设置
        int fontSize = 12 + fontSizeBox->currentIndex() * 4;  // s=12, m=16, l=20

        // 获取选中的颜色
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

        // 获取笔画粗细
        int penWidth = widthSlider->value();

        // 应用设置
        applySettings(fontSize, penColor, penWidth);
        ui->settingWidget->hide();
        });

    // 定义重置函数
    auto resetSettings = [=]() {
        fontSizeBox->setCurrentIndex(0);  // 重置字体大小
        colorGroup->buttons().first()->setChecked(true);  // 重置颜色选择
        currentColorDisplay->setStyleSheet("background-color: black;");  // 重置当前颜色显示
        widthSlider->setValue(2);  // 重置笔画粗细

        // 立即应用默认设置
        applySettings(12, Qt::black, 2);
    };

    // 重置按钮功能
    connect(resetBtn, &QPushButton::clicked, this, resetSettings);

    // 初始化时调用一次重置
    resetSettings();
    // 默认隐藏设置窗口
    ui->settingWidget->hide();
}
//保存设置
void MainWindow::applySettings(int fontSize, QColor penColor, int penWidth)
{
    // 应用笔迹设置
    if (inputs.g_pIInkCollector) {
        IInkDrawingAttributes* attr;
        if (SUCCEEDED(inputs.g_pIInkCollector->get_DefaultDrawingAttributes(&attr))) {
            // 设置颜色
            attr->put_Color(RGB(penColor.red(), penColor.green(), penColor.blue()));

            // 设置笔迹宽度，根据滑动条值映射到合适的笔迹大小范围
            float scaledWidth = penWidth * 20.5f;  // 将1-10的范围映射到更合适的笔迹宽度
            attr->put_Width(scaledWidth);
            attr->put_Height(scaledWidth);  // 同时设置高度确保笔迹粗细一致

            attr->Release();
        }
    }
    on_Btn_clear_clicked();

    // 更新所有组件的字体大小
    QFont newFont;
    newFont.setPointSize(fontSize);

    // 更新手写选择按钮字体
    QList<QPushButton*> handwritingButtons = {
        ui->Btn_word1, ui->Btn_word2, ui->Btn_word3,
        ui->Btn_word4, ui->Btn_word5, ui->Btn_word6,
        ui->Btn_word7, ui->Btn_word8, ui->Btn_word9
    };
    for (auto btn : handwritingButtons) {
        btn->setFont(newFont);
    }

    // 更新功能按钮字体
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

    // 更新动态创建的英文按钮字体
    QWidget* pageEn = ui->page_en;
    if (pageEn) {
        QList<QPushButton*> enButtons = pageEn->findChildren<QPushButton*>();
        for (auto btn : enButtons) {
            btn->setFont(newFont);
        }
    }

    // 更新动态创建的符号按钮字体
    QWidget* pageSym = ui->page_sym;
    if (pageSym) {
        QList<QPushButton*> symButtons = pageSym->findChildren<QPushButton*>();
        for (auto btn : symButtons) {
            btn->setFont(newFont);
        }
    }

}
//还原被改动但没保存的选项
void MainWindow::syncCurrentSettings()
{
    // 1. 获取当前笔画粗细
    IInkDrawingAttributes* attr;
    if (SUCCEEDED(inputs.g_pIInkCollector->get_DefaultDrawingAttributes(&attr)))
    {
        // 获取笔画宽度
        float currentWidth;
        attr->get_Width(&currentWidth);
        // 释放资源
        attr->Release();
        // 同步笔画粗细滑动条
        QSlider* widthSlider = nullptr;
        QWidget* widthContainer = ui->settingWidget->cellWidget(2, 1);
        if (widthContainer) {
            widthSlider = widthContainer->findChild<QSlider*>();
            if (widthSlider) {
                // 将实际宽度转换回滑动条值（1-10）
                int sliderValue = qRound(currentWidth / 20.5f);
                sliderValue = qBound(1, sliderValue, 10);
                widthSlider->setValue(sliderValue);
            }
        }
    }

    // 2. 获取字体大小并设置下拉框
    QComboBox* fontSizeBox = nullptr;
    QWidget* fontContainer = ui->settingWidget->cellWidget(0, 1);
    if (fontContainer && (fontSizeBox = qobject_cast<QComboBox*>(fontContainer))) {
        // 获取当前字体大小
        QPushButton* anyButton = ui->Btn_word1;  // 使用任意一个按钮作为参考
        if (anyButton) {
            int currentSize = anyButton->font().pointSize();
            // 将字体大小转换为对应的索引（12->0, 16->1, 20->2）
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