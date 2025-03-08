#include "mainwindow.h"
#include "ui_mainwindow.h"
//�����������
void MainWindow::input_word(QString selectedChar)
{
    HWND hwndFocused = GetForegroundWindow();
    std::wstring wstr = selectedChar.toStdWString();
    //std::wcout << wstr << endl;
    SendKeys(hwndFocused, wstr);
    on_Btn_clear_clicked();
    
    //�Ƿ������
    if (Lenovo&&DatabaseInit)
    {
        if (!m_lastInput.isEmpty()) {
            QString wordPair = m_lastInput + selectedChar;

            // ��ѯ��������Ƿ����
            QVector<QString> checkExist = m_suggestion->getSuggestions(wordPair, 1);

            if (checkExist.isEmpty() || checkExist[0] != wordPair) {
                // ��������ڣ�����´�
                m_suggestion->addWord(wordPair, 2);
            }
            else {
                // ������ڣ�����Ƶ��
                m_suggestion->updateFrequency(wordPair);
            }
        }
        // ������һ��������ַ�
        m_lastInput = selectedChar;

        QVector<QString> suggestions = m_suggestion->getSuggestions(selectedChar);
        //m_suggestion->suggestions = 
        for (QString& suggestion : suggestions) {
            if (!suggestion.isEmpty()) {
                // ʹ��remove�����Ƴ���һ���ַ�
                suggestion.remove(0, 1);
            }
        }

        // ��ʾ�����
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
//������̷����ַ�
void MainWindow::SendKeys(HWND hwnd, const std::wstring& text)
{
    for (wchar_t ch : text) {
        // ����WM_CHAR��Ϣ��ģ���ַ�����
        SendMessageW(hwnd, WM_CHAR, ch, 0);
    }
}
//ʶ����д�����ð���
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
    // ��� tempResult �Ƿ��������
    int dataSize = tempResult.size();
    if (dataSize < 1)
        return; // ���û�����ݣ�ֱ�ӷ���

    //���ð�ť�ϵ���
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
//ѡ��ʶ����
void MainWindow::on_HandWritingClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QChar key = button->objectName().back(); // ��ȡ��ť������
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

//����ʶ��ģʽ
void MainWindow::on_Multiple_words_stateChanged(int state)
{
    bool multipleWords = (state == Qt::Checked);
    inputs.setRecognitionMode(multipleWords);

    // �л�ģʽʱ��յ�ǰ����
    on_Btn_clear_clicked();
}

//��������ģʽ
void MainWindow::on_Lenovo_words_stateChanged(int state)
{
    Lenovo = (state == Qt::Checked);
}

//����
void MainWindow::on_Btn_CtrlZ_clicked()
{
    qDebug() << "Starting undo in MainWindow...";

    // �洢��ǰ����
    HWND hwnd = (HWND)ui->label->winId();
    // ����
    inputs.undo();
    // ǿ�Ƹ���UI
    qDebug() << "Forcing UI refresh...";
    InvalidateRect(hwnd, NULL, TRUE);
    UpdateWindow(hwnd);
    //����ʶ��
    recognizeHand();
}

//ɾ��
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

