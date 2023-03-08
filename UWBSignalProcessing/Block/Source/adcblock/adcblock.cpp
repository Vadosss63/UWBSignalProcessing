#include "adcblock.h"

ADCServerBlock::ADCServerBlock():
    SourceBlock("АЦП")
{
    setToolTip("Получет информацию от группы АЦП и\nотправляет ее на I и Q порты");
}

AbstractBlock* ADCServerBlock::Clone() const
{
    return new ADCServerBlock;
}

AbstractModule* ADCServerBlock::GetModule() const
{
    return m_module.get();
}

QString ADCServerBlock::GetType() const
{
    return "ADCServerBlock";
}

void ADCServerBlock::Change()
{
    if (m_module && m_dialog)
    {

    }
}

void ADCServerBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*)
{
    DrawRect(painter, option);
    DrawImage(painter);
}

void ADCServerBlock::DrawImage(QPainter* painter)
{
    const QRectF& rect = GetborderRect();

    if(!isInit)
    {
        painter->drawText(rect, Qt::AlignCenter, "АЦП \n сервер");
        return;
    }
    
    
    if(m_listNameIP.empty())
    {
        isInit = false;
        return;
    }

    // вычисление для построения
    short sizeList = 25 + 38 * m_listNameIP.size();
    for(short i = 25; i < sizeList; i += 38)
    {
        painter->drawText(static_cast<int>(rect.right()) - 12, static_cast<int>(rect.top()) + i, "I");
        painter->drawText(static_cast<int>(rect.right()) - 15, static_cast<int>(rect.top()) + i + 19, "Q");
    }
    int j = 0;
    painter->setFont(QFont("Times", 10));
    painter->setPen(Qt::black);
    for (short i = 25; i < sizeList; i += 38)
    {

        painter->drawText(QRectF(rect.right() - 30, rect.top() + i - 5, 18, 19),
                          Qt::AlignCenter, QString::number(m_listNameIP[j]));
        j++;
    }
    painter->setFont(QFont());
    
}

void ADCServerBlock::ChangeCountPorts()
{
    isInit = true;
    CreateBlockPorts();
}

void ADCServerBlock::CreateBlockPorts()
{
    uint8_t size = m_module->CountOutputPort();
    QVector<OPort*> ports;
    for (uint8_t i = 0; i < size; ++i)
    {
        ports.push_back(m_module->GetOutput(i));
    }
    CreatePorts(ports);
}

void ADCServerBlock::AddInitADCiP(uint8_t name)
{
    m_listNameIP.push_back(name + 1);
}

void ADCServerBlock::CleanListNameIP()
{
    isInit = false;
    m_listNameIP.clear();
}

void ADCServerBlock::InitBlock(QWidget *wparent)
{
    m_module = std::make_unique<ADCModule>();
    m_dialog = std::make_unique<ADCServerDialogBox>(m_module.get(), wparent);
    isInit = false;
    SetAbstractDialogCommand(m_dialog.get());
    m_dialog->CheckIPs();
    ChangeCountPorts();
}

PluginBlock::PluginBlock(QObject* parent): QObject(parent){}

AbstractBlock* PluginBlock::LoudBlock() const
{
    return new ADCServerBlock;
}


void ADCServerBlock::RegistrOperationManager(AbstractOperationManager* operationManager)
{
    if(m_module)
        m_module->RegistrOperationManager(operationManager);
}
