#include "handsinput.h"
#include <QDebug>


handsInput::handsInput(QWidget *parent) : QWidget(parent)
{

}


bool handsInput::inputInit(HWND hwnd)
{
    if(NULL == hwnd)
        return false;
    g_pIInkCollector = NULL;
    g_pIInkDisp = NULL;
    g_pIInkRecoContext = NULL;
    // 初始化 COM 接口
    CoInitialize(NULL);
    HRESULT hr;

    hr = CoCreateInstance(CLSID_InkRecognizerContext,NULL, CLSCTX_INPROC_SERVER,IID_IInkRecognizerContext,(void **) &g_pIInkRecoContext);
    if (FAILED(hr)) {
        QMessageBox::warning(this, u8"错误提示", u8"未安装手写识别软件,程序可能无法正常工作");
        qDebug() << "未安装手写识别软件=";
        return false;
    }
    // 创建一个墨迹收集对象
    hr = CoCreateInstance(CLSID_InkCollector,NULL, CLSCTX_INPROC_SERVER,IID_IInkCollector,(void **) &g_pIInkCollector);
    if (FAILED(hr))
        return false;

    // 获取墨迹对象的指针
    hr = g_pIInkCollector->get_Ink(&g_pIInkDisp);
    if (FAILED(hr))
        return false;

    // 将墨迹对象关联一个 hWnd 的窗口
    hr = g_pIInkCollector->put_hWnd((long)hwnd);
    if (FAILED(hr))
        return false;

    // 打开墨迹输入的开关
    hr = g_pIInkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr))
        return false;

    //设置识别模式
    setRecognitionMode(false);
    return true;

}

void handsInput::ProcessInkData(std::vector<InputResult>& ResultsContainer)
{
    // 清空现有结果
    ResultsContainer.clear();

    // 获取墨迹快照
    IInkStrokes* pInkStrokesSnapshot = nullptr;
    HRESULT hr = g_pIInkDisp->get_Strokes(&pInkStrokesSnapshot);
    if (SUCCEEDED(hr))
    {
        // 设置识别器的笔触源
        hr = g_pIInkRecoContext->putref_Strokes(pInkStrokesSnapshot);
        if (SUCCEEDED(hr))
        {
            // 执行识别过程
            IInkRecognitionResult* pRecognitionOutcome = nullptr;
            InkRecognitionStatus status = IRS_NoError;
            hr = g_pIInkRecoContext->Recognize(&status, &pRecognitionOutcome);
            if (SUCCEEDED(hr) && pRecognitionOutcome != nullptr)
            {
                // 获取识别的备选结果
                IInkRecognitionAlternates* pAlternates = nullptr;
                hr = pRecognitionOutcome->AlternatesFromSelection(0, -1, 10, &pAlternates);
                long alternateCount = 0;
                if (SUCCEEDED(hr) && pAlternates->get_Count(&alternateCount) == S_OK)
                {
                    // 遍历并存储备选结果
                    for (LONG index = 0; index < alternateCount && index < 10; ++index)
                    {
                        IInkRecognitionAlternate* pCurrentAlternate = nullptr;
                        if (pAlternates->Item(index, &pCurrentAlternate) == S_OK)
                        {
                            BSTR recognitionString = nullptr;
                            if (pCurrentAlternate->get_String(&recognitionString) == S_OK)
                            {
                                InputResult tempRlt = { 0 };
                                QString str = QString::fromWCharArray(recognitionString);
                                strncpy(tempRlt.result, str.toUtf8().data(), sizeof(tempRlt.result) - 1);
                                ResultsContainer.push_back(tempRlt);
                            }
                            pCurrentAlternate->Release();
                        }
                    }
                }
            }
            // 重置识别器笔触源
            g_pIInkRecoContext->putref_Strokes(nullptr);
        }
        pInkStrokesSnapshot->Release();

    }
}

void handsInput::clear()
{
    if (g_pIInkDisp) 
        g_pIInkDisp->DeleteStrokes(0);
}

void handsInput::undo()
{
    qDebug() << "=== Starting undo ===";
    if (!g_pIInkCollector || !g_pIInkDisp) 
    {
        qDebug() << "Error: Invalid collector or ink object";
        return;
    }

    // 获取当前墨迹集合
    IInkStrokes* strokes = nullptr;
    HRESULT hr = g_pIInkDisp->get_Strokes(&strokes);
    if (SUCCEEDED(hr) && strokes) 
    {
        LONG count;
        hr = strokes->get_Count(&count);
        qDebug() << "Current stroke count before remove:" << count;

        if (count > 0) 
        {
            // 获取并删除最后一笔
            IInkStrokeDisp* lastStroke = nullptr;
            hr = strokes->Item(count - 1, &lastStroke);
            if (SUCCEEDED(hr) && lastStroke)
            {
                // 创建一个 IInkStrokes 对象，用于存放将被删除的笔迹
                IInkStrokes* pRemoveStrokes = nullptr;
                VARIANT varEmpty;
                VariantInit(&varEmpty); // 初始化空的 VARIANT，表示不指定 StrokeIds

                hr = g_pIInkDisp->CreateStrokes(varEmpty, &pRemoveStrokes); // 调用 CreateStrokes 创建空的 IInkStrokes
                if (SUCCEEDED(hr) && pRemoveStrokes) 
                {
                    // 将最后一条笔迹添加到临时 IInkStrokes 对象中
                    pRemoveStrokes->Add(lastStroke);
                    // 删除该笔迹
                    hr = g_pIInkDisp->DeleteStrokes(pRemoveStrokes);
                    if (FAILED(hr)) 
                        qDebug() << "Failed to delete the last stroke.";

                    pRemoveStrokes->Release();  // 释放临时的 IInkStrokes 对象
                }
                lastStroke->Release();  // 释放最后一条笔迹对象
            }
        }
        strokes->Release();
    }
    // 从历史记录中移除
    /*IInkStrokeDisp* historicalStroke = m_strokesHistory.back();
    if (historicalStroke) {
        historicalStroke->Release();
    }
    m_strokesHistory.pop_back();*/
    qDebug() << "=== Finished undo ===";
}

void handsInput::setRecognitionMode(bool multipleWords)
{
    if (!g_pIInkRecoContext)
        return;

    HRESULT hr;
    IInkRecognizerGuide* RecognizerGuide;
    hr = CoCreateInstance(CLSID_InkRecognizerGuide, NULL, CLSCTX_INPROC_SERVER,
        IID_IInkRecognizerGuide, (void**)&RecognizerGuide);
    if (FAILED(hr))
        return;

    InkRecoGuide recoguide;
    RECT rect;

    if (multipleWords) {
        // 多字模式：设置较大的识别区域
        rect.bottom = 100;
        rect.left = 0;
        rect.right = 300;
        rect.top = 0;
        recoguide.cRows = 1;    // 允许多行
        recoguide.cColumns = 10; // 允许多列
    }
    else {
        // 单字模式：设置小的识别区域
        rect.bottom = 2;
        rect.left = 0;
        rect.right = 2;
        rect.top = 0;
        recoguide.cRows = 1;
        recoguide.cColumns = 1;
    }

    recoguide.rectWritingBox = rect;
    recoguide.rectDrawnBox = rect;
    recoguide.midline = -1;

    RecognizerGuide->put_GuideData(recoguide);
    g_pIInkRecoContext->putref_Guide(RecognizerGuide);
    RecognizerGuide->Release();
}


handsInput::~handsInput()
{
    if (g_pIInkRecoContext != NULL)
    {
        g_pIInkRecoContext->Release();
        g_pIInkRecoContext = NULL;
    }

    if (g_pIInkDisp != NULL)
    {
        g_pIInkDisp->Release();
        g_pIInkDisp = NULL;
    }

    if (g_pIInkCollector != NULL)
    {
        g_pIInkCollector->Release();
        g_pIInkCollector = NULL;
    }
    CoUninitialize();
}