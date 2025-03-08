#include "mainwindow.h"
#include "ui_mainwindow.h"
//调用虚拟键盘
void MainWindow::input_word(QString selectedChar)
{
    HWND hwndFocused = GetForegroundWindow();
    std::wstring wstr = selectedChar.toStdWString();
    //std::wcout << wstr << endl;
    SendKeys(hwndFocused, wstr);
    on_Btn_clear_clicked();
    
    //是否打开联想
    if (Lenovo&&DatabaseInit)
    {
        if (!m_lastInput.isEmpty()) {
            QString wordPair = m_lastInput + selectedChar;

            // 查询这个词组是否存在
            QVector<QString> checkExist = m_suggestion->getSuggestions(wordPair, 1);

            if (checkExist.isEmpty() || checkExist[0] != wordPair) {
                // 如果不存在，添加新词
                m_suggestion->addWord(wordPair, 2);
            }
            else {
                // 如果存在，更新频率
                m_suggestion->updateFrequency(wordPair);
            }
        }
        // 更新上一次输入的字符
        m_lastInput = selectedChar;

        QVector<QString> suggestions = m_suggestion->getSuggestions(selectedChar);
        //m_suggestion->suggestions = 
        for (QString& suggestion : suggestions) {
            if (!suggestion.isEmpty()) {
                // 使用remove方法移除第一个字符
                suggestion.remove(0, 1);
            }
        }

        // 显示联想词
        QList<QPushButton*> buttons = {
            ui->Btn_word1, ui->Btn_word2, ui->Btn_word3,
            ui->Btn_word4, ui->Btn_word5, ui->Btn_word6,
            ui->Btn_word7, ui->Btn_word8, ui->Btn_word9
        };
        for (int i = 0; i < buttons.size(); ++i) {
            if (i < suggestions.size()) {
                buttons[i]->setText(suggestions[i]);
                InputResult tempRlt = { 0 };
                //QString str = QString::fromWCharArray(recognitionString);
                strncpy(tempRlt.result, suggestions[i].toUtf8().data(), sizeof(tempRlt.result) - 1);
                tempResult.push_back(tempRlt);
            }
            else {
                buttons[i]->setText("");
            }
        }
    }
}
//虚拟键盘发送字符
void MainWindow::SendKeys(HWND hwnd, const std::wstring& text)
{
    for (wchar_t ch : text) {
        // 发送WM_CHAR消息，模拟字符输入
        SendMessageW(hwnd, WM_CHAR, ch, 0);
    }
}
//识别手写并设置按键
void MainWindow::recognizeHand()
{
    tempResult.clear();
    QList<QPushButton*> buttons = {
       ui->Btn_word1, ui->Btn_word2, ui->Btn_word3,
       ui->Btn_word4, ui->Btn_word5, ui->Btn_word6,
       ui->Btn_word7, ui->Btn_word8, ui->Btn_word9
    };

    for (auto btn : buttons) {
        btn->setText("");
    }
    inputs.ProcessInkData(tempResult);
    // 检查 tempResult 是否包含数据
    int dataSize = tempResult.size();
    if (dataSize < 1)
        return; // 如果没有数据，直接返回

    //设置按钮上的字
    for (int i = 0; i < dataSize && i < 9; ++i)
    {
        QString buttonName = QString("Btn_word%1").arg(i + 1);
        QPushButton* button = findChild<QPushButton*>(buttonName);
        if (button)
        {
            button->setText(QString::fromUtf8(QByteArray(tempResult[i].result)));
        }
    }
}
//选择识别结果
void MainWindow::on_HandWritingClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QChar key = button->objectName().back(); // 获取按钮的命名
        int number = key.digitValue();
        qDebug() << "number chosen clicked:" << key;
        if (tempResult.size() >= number)
            input_word(QString::fromUtf8(QByteArray(tempResult[number - 1].result)));
        else
            return;
    }
    else
        return;
}

//设置识别模式
void MainWindow::on_Multiple_words_stateChanged(int state)
{
    bool multipleWords = (state == Qt::Checked);
    inputs.setRecognitionMode(multipleWords);

    // 切换模式时清空当前内容
    on_Btn_clear_clicked();
}

//设置联想模式
void MainWindow::on_Lenovo_words_stateChanged(int state)
{
    Lenovo = (state == Qt::Checked);
}

//撤销
void MainWindow::on_Btn_CtrlZ_clicked()
{
    qDebug() << "Starting undo in MainWindow...";

    // 存储当前窗口
    HWND hwnd = (HWND)ui->label->winId();
    // 撤销
    inputs.undo();
    // 强制更新UI
    qDebug() << "Forcing UI refresh...";
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
    //重新识别
    recognizeHand();
}

//删除
void MainWindow::on_Btn_clear_clicked()
{
    inputs.clear();
    ui->label->clear();
    //m_lastInput.clear();
    ui->Btn_word1->setText("");
    ui->Btn_word2->setText("");
    ui->Btn_word3->setText("");
    ui->Btn_word4->setText("");
    ui->Btn_word5->setText("");
    ui->Btn_word6->setText("");
    ui->Btn_word7->setText("");
    ui->Btn_word8->setText("");
    ui->Btn_word9->setText("");

    tempResult.clear();
}

