#include "displayview.h"

DisplayView::DisplayView(QWidget *pwgt): QWidget(pwgt)
{
    m_mainLayout = new QGridLayout;
    setLayout(m_mainLayout);
    CreateButtom();
}

DisplayView::~DisplayView()
{
    delete m_drawData;
}

void DisplayView::timerEvent(QTimerEvent *)
{
    ReadData();
}

void DisplayView::Start()
{
    m_idTimer = startTimer(20);
}

void DisplayView::Pause()
{
    killTimer(m_idTimer);
}

void DisplayView::Stop()
{
    killTimer(m_idTimer);
    m_parserData.SetPathFile(m_pathFile.toStdString());
    m_drawData->ClearBuffer();
    m_scopeWidgetOPGL->ClearBuffer();
}

void DisplayView::OpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Открыть файл","",
                                                "rmp(*.rmp)");
    if(path.isEmpty())
        return;

    m_pathFile = path;
    m_parserData.SetPathFile(m_pathFile.toStdString());

}

void DisplayView::SetMaximumShift(int val)
{
    m_sliderNavigate->setMaximum(val);
}

void DisplayView::SetChangeShift(int val)
{
    m_drawData->setShiftStart(val);
    m_scopeWidgetOPGL->SetShiftStart(val);
}

void DisplayView::UpdateScope()
{
    m_scopeWidgetOPGL->makeCurrent();
    m_scopeWidgetOPGL->updateGL();
}

void DisplayView::UpdateData()
{
    m_drawData->makeCurrent();
    m_drawData->updateGL();
}

void DisplayView::ReadDataAll()
{
    m_drawData->ClearBuffer();
    m_scopeWidgetOPGL->ClearBuffer();
    while(m_parserData.IsNextData())
    {
        Package<std::vector<quint8>, Header> data = m_parserData.GetData();
        ScopeLine(data.infoBody(), false);
        RenderNewLine(data.infoBody(), false);
    }
    UpdateScope();
    UpdateData();
}

void DisplayView::ShowHeaderData(const Header &header)
{
    QString headertext = GenerateTextHeader(header);
    m_headerValue->setText(headertext);
}

void DisplayView::AddView(DrawLine *drawLine)
{
    m_drawData = drawLine;
    m_drawData->setFixedSize(QSize(512, 800));
    m_viewLayout->addWidget(m_drawData);
    connect(m_drawData, SIGNAL(MaxShift(int)), this, SLOT(SetMaximumShift(int))); 
}

void DisplayView::AddViewOrig(DrawLine *drawLine)
{
    m_drawDataOriginal = drawLine;
    m_drawDataOriginal->setFixedSize(QSize(512, 800));
    m_viewLayout->addWidget(m_drawDataOriginal);
    connect(m_drawDataOriginal, SIGNAL(MaxShift(int)), this, SLOT(SetMaximumShift(int)));
}

void DisplayView::AddScope(ScopeWidgetOPGL* scope)
{
    m_scopeWidgetOPGL = scope;
    m_scopeWidgetOPGL->setFixedSize(QSize(512, 256));
    m_mainLayout->addWidget(m_scopeWidgetOPGL, 1, 2, Qt::AlignTop);
}
void DisplayView::CreateButtom()
{
    m_viewLayout = new QHBoxLayout;
    m_mainLayout->addLayout(m_viewLayout, 0, 0, 3, 1);
    m_sliderNavigate = new QSlider(Qt::Orientation::Vertical);
    m_mainLayout->addWidget(m_sliderNavigate, 0, 1, 3, 1,  Qt::AlignCenter);
    m_sliderNavigate->setFixedHeight(800);
    m_sliderNavigate->setMaximum(0);
    connect(m_sliderNavigate, SIGNAL(valueChanged(int)), this, SLOT(SetChangeShift(int )));

    QHBoxLayout* layoutButtom = new QHBoxLayout;
    m_startButtom = new QPushButton("Start");
    m_pauseButtom = new QPushButton("Pause");
    m_stopButtom = new QPushButton("Stop");
    m_openButtom = new QPushButton("OpenFile");
    m_readAllButtom = new QPushButton("ReadAllData");
    layoutButtom->addWidget(m_startButtom);
    layoutButtom->addWidget(m_pauseButtom);
    layoutButtom->addWidget(m_stopButtom);
    layoutButtom->addWidget(m_openButtom);
    layoutButtom->addWidget(m_readAllButtom);

    connect(m_startButtom, SIGNAL(clicked()), this, SLOT(Start()));
    connect(m_pauseButtom, SIGNAL(clicked()), this, SLOT(Pause()));
    connect(m_stopButtom, SIGNAL(clicked()), this, SLOT(Stop()));
    connect(m_openButtom, SIGNAL(clicked()), this, SLOT(OpenFile()));
    connect(m_readAllButtom, SIGNAL(clicked()), this, SLOT(ReadDataAll()));

    m_mainLayout->addLayout(layoutButtom, 0, 2, Qt::AlignTop);

    m_headerValue = new QLabel("");
    m_mainLayout->addWidget(m_headerValue, 2, 2, Qt::AlignTop);

}

QString DisplayView::GenerateTextHeader(const Header &header)
{
    QString data;
    data += QString("Имя файла: %1").arg(m_pathFile);
    data += QString("\nBlockSize: %1").arg(header.BlockSize);
    data += QString("\nLineSize: %1").arg(header.LineSize);
    data += QString("\nBlockNumber: %1").arg(header.BlockNumber);
    data += QString("\nAntGain: %1").arg(header.AntGain);
    data += QString("\nAntAzimut: %1").arg(header.AntAzimut);
    data += QString("\nRange: %1").arg(header.Range);
    data += QString("\nPitchAngle: %1").arg(header.PitchAngle);
    data += QString("\nHeelingAngle: %1").arg(header.HeelingAngle);
    data += QString("\nAltitude: %1").arg(header.Altitude);
    data += QString("\nVSSSpeed: %1").arg(header.VSSSpeed);
    data += QString("\nAKGain: %1").arg(header.AKGain);
    data += QString("\nAKValue: %1").arg(header.AKValue);
    data += QString("\nSNSLatitude: %1").arg(header.SNSLatitude);
    data += QString("\nSNSLongitude: %1").arg(header.SNSLongitude);
    data += QString("\nVSSLatitude: %1").arg(header.VSSLatitude);
    data += QString("\nVSSLongitude: %1").arg(header.VSSLongitude);
    return data;
}

void DisplayView::ScopeLine(const std::vector<quint8> &data, bool upadeNow)
{
    if(m_scopeWidgetOPGL){
        m_scopeWidgetOPGL->makeCurrent();
        m_scopeWidgetOPGL->AddData(data, upadeNow);
    }
}

void DisplayView::RenderNewLine(const std::vector<quint8> &data, bool upadeNow)
{
    m_drawData->makeCurrent();
    m_drawData->AddData(data, upadeNow);
}

void DisplayView::RenderNewLine(const std::vector<QColor> &data, bool upadeNow)
{
    m_drawData->makeCurrent();
    m_drawData->AddData(data, upadeNow);
}

void DisplayView::RenderNewLineOrig(const std::vector<quint8> &data, bool upadeNow)
{
    m_drawDataOriginal->makeCurrent();
    m_drawDataOriginal->AddData(data, upadeNow);
}

void DisplayView::RenderNewLineOrig(const std::vector<QColor> &data, bool upadeNow)
{
    m_drawDataOriginal->makeCurrent();
    m_drawDataOriginal->AddData(data, upadeNow);
}

void DisplayView::ReadData()
{
    if(m_parserData.IsNextData())
    {
        Package<std::vector<quint8>, Header> data = m_parserData.GetData();
        ScopeLine(data.infoBody());
        RenderNewLine(data.infoBody());
        RenderNewLineOrig(data.infoBody());
        ShowHeaderData(data.header());
    }
}
