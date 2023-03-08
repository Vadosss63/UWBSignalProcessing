#ifndef DISPLAYVIEW_H
#define DISPLAYVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QLabel>
#include <QSlider>
#include "drawline.h"
#include "scopewidgetopgl.h"

class DisplayView: public QWidget
{
    Q_OBJECT
public:
    DisplayView(QWidget* pwgt = 0);
    ~DisplayView();

    void AddView(DrawLine* drawLine);    
    void AddViewOrig(DrawLine *drawLine);
    void AddScope(ScopeWidgetOPGL *scope);

protected:
    void timerEvent(QTimerEvent *) override;

private slots:
    void Start();
    void Pause();
    void Stop();
    void OpenFile();
    void SetMaximumShift(int val);
    void SetChangeShift(int val);
    void ReadDataAll();

private:

    void CreateButtom();
    QString GenerateTextHeader(const Header& header);
    void ReadData();

    void ScopeLine(const std::vector<quint8>& data, bool upadeNow = true);
    void ShowHeaderData(const Header& header);
    void RenderNewLine(const std::vector<quint8>& data, bool upadeNow = true);
    void RenderNewLine(const std::vector<QColor>& data, bool upadeNow = true);

    QPushButton* m_startButtom;
    QPushButton* m_pauseButtom;
    QPushButton* m_stopButtom;
    QPushButton* m_openButtom;
    QPushButton* m_readAllButtom;
    QLabel*  m_headerValue;
    QSlider* m_sliderNavigate;
    // Класс отрисовки линий
    DrawLine* m_drawData = nullptr;
    DrawLine* m_drawDataOriginal = nullptr;
    // Класс осцилогрофа
    ScopeWidgetOPGL* m_scopeWidgetOPGL = nullptr;
    QGridLayout* m_mainLayout;
    QHBoxLayout* m_viewLayout;
    // Парсер данных из файла
    Parser<quint8, Header> m_parserData;
    // запущенного таймера
    int m_idTimer;
    // Путь к файлу с данными
    QString m_pathFile;
    void UpdateScope();
    void UpdateData();
    void RenderNewLineOrig(const std::vector<quint8> &data, bool upadeNow = true);
    void RenderNewLineOrig(const std::vector<QColor> &data, bool upadeNow = true);
};

#endif // DISPLAYVIEW_H
