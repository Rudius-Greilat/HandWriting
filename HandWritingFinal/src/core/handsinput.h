#ifndef HANDSINPUT_H
#define HANDSINPUT_H

#include <QWidget>

#include <msinkaut_i.c>
#include <msinkaut.h>
#include <vector>
#include <QMessageBox>
using namespace std;

typedef struct tag_InputRlt
{
    char result[128];
}InputResult;

typedef vector <InputResult> InputResults;

class handsInput : public QWidget
{
    Q_OBJECT
public:
    explicit handsInput(QWidget *parent = 0);
    ~handsInput();

    IInkCollector *     g_pIInkCollector;
    IInkDisp *          g_pIInkDisp;
    IInkRecognizerContext *   g_pIInkRecoContext;


    void clear();
    void undo();
    bool inputInit( HWND hwnd );
    void ProcessInkData(std::vector<InputResult>& ResultsContainer);
    void setRecognitionMode(bool multipleWords);
    
};

#endif // HANDSINPUT_H
