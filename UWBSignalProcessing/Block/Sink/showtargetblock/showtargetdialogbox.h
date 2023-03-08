#ifndef SHOWTARGETDIALOGBOX_H
#define SHOWTARGETDIALOGBOX_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsPathItem>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QAction>
#include <QSpinBox>
#include <QLabel>
#include <QSettings>
#include <QPushButton>
#include <qcheckbox.h>
#include "dialogwindow.h"
#include "changeevent.h"
#include "sectorgriditem.h"
#include "intrface.h"
#include "target.h"
#include "targetitem.h"


class ScanView: public QGraphicsView
{

public:
    void ZoomIn();
    void ZoomOriginal();
    void ZoomOut();

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    // действия при нажажатии клавиш
    void keyPressEvent(QKeyEvent* event) override;

};

class DialogSettingTarget: public QDialog
{
public:
    DialogSettingTarget(int currentVal, int currentValDynamic);

    int GetMaxVal();
    int GetMaxValDynamic();

private:
    QSpinBox* m_maxVal;
    QSpinBox* m_maxValDynamic;
};

class ShowTargetDialogBox: public QDialog, public DialogWindow, public UpdateScane
{
    Q_OBJECT
public:

    ShowTargetDialogBox(QWidget *wparent = nullptr);
    ~ShowTargetDialogBox() override = default;

    QList<std::pair<QString, QVariant>> GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
    void ShowDialog() override;
    void SendCommand() override;
    void Attach(ChangeEvent* observer) override;
    void Notification() override;

    // добавление объектов на сцену
    void AddTarget(std::vector<Target>&& targets) override;
    // сброс буфура
    void ResetTarget() override;
    // отображение новых данных
    void Update() override;

    void SendCommandAbstractModule();

protected slots:

    void UpdataScene();
    void ResetTargetScene();
    void Add(QVector<TargetItem*> targets);

    void OpenDialogSetting();


signals:
    void AddData(QVector<TargetItem*>);
    void Reset();
    void UpdateTarget();

protected:
    // изменение размеров формы
    void resizeEvent(QResizeEvent *) override;

private:

    void CreateScene();
    void CreateContextMenu();
    void InitSetting();
    void CreateGrid();

    // параметры для отображения блоков
    ScanView* m_view = nullptr;
    QGraphicsScene* m_scene = nullptr;

    // размер прямоугольника 2*1024 ширина и высота
    qreal m_margin = 40;
    qreal m_sizeRect = 1024;
    SectorGrid* m_grid = nullptr;
    QAction* m_settings = nullptr;
    ChangeEvent* m_observer = nullptr;

    QVector<TargetItem*> m_target;
    QColor m_colorBackgraund = QColor("#C0C0C0)");

};

#endif // SHOWTARGETDIALOGBOX_H
