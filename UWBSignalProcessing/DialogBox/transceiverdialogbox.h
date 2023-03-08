#ifndef TRANSCEIVERDIALOGBOX_H
#define TRANSCEIVERDIALOGBOX_H

#include "../MainWindow/abstractdialogcommand.h"
#include "../dspmLib/include/dspm.h"


namespace Ui {
class TransceiverDialogBox;
}


class TransceiverDialogBox: public AbstractDialogCommand
{
    Q_OBJECT
public:
    TransceiverDialogBox(dspm::TransceiverControlModule *parent, QWidget *wparent = 0);
    ~TransceiverDialogBox();
    TransceiverDialogBox(const TransceiverDialogBox& rhs) = delete;
    TransceiverDialogBox& operator=(const TransceiverDialogBox& rhs) = delete;



signals:

protected slots:
    virtual void SendCommandAbstractModule() override;





private:
    //    создание сетки
    QGridLayout* m_gridLayout;

    // загрузка формы диоллогового окна
    Ui::TransceiverDialogBox* ui;

    // для добавления виждета
    QWidget* m_layout;




    dspm::TransceiverControlModule* m_module;

};


#endif // TRANSCEIVERDIALOGBOX_H
