#ifndef SCANFFTDIALOGBOX_H
#define SCANFFTDIALOGBOX_H
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
#include <QGraphicsProxyWidget>
#include "fft_plotter.h"
#include "dialogwindow.h"
#include "changeevent.h"
#include "sectoritem.h"
#include "sectorgriditem.h"
#include "intrface.h"


class ScanView: public QGraphicsView
{
    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
};

class ScanfftDialogBox : public QDialog, public DialogWindow, public UpdateScane
{
    Q_OBJECT
public:

    ScanfftDialogBox(QWidget *wparent = nullptr);
    ~ScanfftDialogBox() override = default;

    QList<std::pair<QString, QVariant>> GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;

    void ShowDialog() override;
    void SendCommand() override;
    void Attach(ChangeEvent* observer) override;
    void Notification() override;
    void CreateSector(int count) override;
    void Update() override;
    void ResizePloter(int size) override;
    QVector<QPointF>& DataPloter() override;
    void UpdatePloter() override;
    std::vector<SectorBaseItem*>& DataMatrix() override;

    void SendCommandAbstractModule();
    bool GetIsLog() const;
    int GetLinePloter() const;
    bool GetIsVerticalPloter() const;

signals:

    void IsUpdate(); // фукция создания сектора отображения

protected slots:

    void UpdataScene();
    void OpenCountRange();
    void OpenSettings();

protected:
    // изменение размеров формы
    void resizeEvent(QResizeEvent *) override;

private:

    // Сброс секторов
    void ResetSectors();
    // Добавление сектора
    void AddSector(SectorRect* sector);
    void CreateContextMenu();
    void InitSetting();
    void CreateGrid();
    void CreatePloter();

    DrawCurver* m_drawCurverH = nullptr;
    QGraphicsProxyWidget* m_drawCurverProxyH = nullptr;

    // параметры для отображения блоков
    ScanView* m_view;
    QGraphicsScene* m_scene;
    int m_range = 512;
    int m_resolution = 30;
    int m_frec = 600;
    bool m_isLog = false;

    int m_linePloter = 128;
    bool m_isVerticalPloter = false;

    // для быстрого доступа к элементам сектора
    std::vector<SectorBaseItem*> m_vectorSector;
    // размер прямоугольника 2*1024 ширина и высота
    qreal m_margin = 40;
    qreal m_sizeRect = 1024;
    SectorGrid* m_grid = nullptr;
    QAction* m_countRange = nullptr;
    QAction* m_settings = nullptr;
    ChangeEvent* m_observer = nullptr;
};

class CountRangeDialog: public QDialog
{
    Q_OBJECT
public:

    CountRangeDialog(QWidget *parent = nullptr);
    virtual ~CountRangeDialog() = default;

    int GetCount();
    qreal MaxRange();
    qreal MaxVelocity();

    void SetCount(int count);
    void SetResolution(int resolution);
    void SetFrec(int frec);

    int GetResolution();
    int GetFrec();

private:

    void CreateDialog();

    QSpinBox* m_countRange = nullptr;
    QSpinBox* m_resolution = nullptr;
    // частота повторения гц
    QSpinBox* m_F = nullptr;
};

class SettingDialog: public QDialog
{
    Q_OBJECT
public:

    SettingDialog();
    virtual ~SettingDialog() = default;

    void SetIsLog(bool isLog);
    bool IsLog();

    void SetIsVerticalPloter(bool isVert);
    bool IsVerticalPloter();

    int GetLinePloter();
    void SetLinePloter(int linePloter);

private:

    void CreateDialog();

    QCheckBox* m_isLog = nullptr;
    QSpinBox* m_linePloter = nullptr;
    QCheckBox* m_isVerticalPloter = nullptr;

};

#endif // SCANFFTDIALOGBOX_H
