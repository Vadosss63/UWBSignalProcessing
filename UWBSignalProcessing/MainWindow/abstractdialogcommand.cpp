#include "abstractdialogcommand.h"

AbstractDialogCommand::AbstractDialogCommand(QWidget *parent) :
    QDialog(parent)
{
    m_topLayout = new QHBoxLayout;
    m_applyButton = new QPushButton("&Применить");
    m_okButton = new QPushButton("&Ок");
    m_okButton->setDefault(true);
    m_cancelButton = new QPushButton("О&тменить");
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(SendCommandAbstractModule()));
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(SendCommandAbstractModule()));

    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->addWidget(m_okButton);
    m_buttonLayout->addWidget(m_applyButton);
    m_buttonLayout->addWidget(m_cancelButton);
    m_mainLayout = new QGridLayout;
    m_mainLayout->addLayout(m_topLayout, 0, 0);
    m_mainLayout->addLayout(m_buttonLayout, 1, 0);
    setLayout(m_mainLayout);
    setWindowTitle("Параметры блока");
}

AbstractDialogCommand::~AbstractDialogCommand(){}

void AbstractDialogCommand::SetTopLayout(QLayout *topLayout)
{
    m_topLayout->addLayout(topLayout);
}

void AbstractDialogCommand::SetEnabledOkButton(bool flag)
{
    m_okButton->setEnabled(flag);
}

void AbstractDialogCommand::SetEnabledApplyButton(bool flag)
{
    m_applyButton->setEnabled(flag);
}

void AbstractDialogCommand::ShowDialog()
{
    if(isHidden())
        show();
    else {
        raise();
        activateWindow();
        showNormal();
    }
}

void AbstractDialogCommand::SendCommand()
{
    Notification();
}

void AbstractDialogCommand::Attach(ChangeEvent* observer)
{
    m_observer = observer;
}

void AbstractDialogCommand::Notification()
{
    if(m_observer)
        m_observer->Change();
}

void AbstractDialogCommand::SendCommandAbstractModule()
{
    SendCommand();
}
