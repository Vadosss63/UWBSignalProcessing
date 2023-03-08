#ifndef SCANSCENEDIALOGBOX_H
#define SCANSCENEDIALOGBOX_H

#include <QDialog>
#include <QDial>
#include <QRect>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QGraphicsView>
#include <QPushButton>
#include <QLCDNumber>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QAction>
#include "dialogwindow.h"
#include "changeevent.h"
#include "scanscenemodule.h"
#include "sectoritem.h"
#include "sectorgriditem.h"
#include "panelscan.h"
#include "vrliviewmodule.h"

// дальность действия локатора
enum rangeValue : uint32_t
{
    // 2048 * 0.3 = 614.4
    range_2048_03 = 614,
    // 2048 * 2 * 0.3 = 1228.8
    range_2048_2_03 = 1229,
    // 2048  * 4 * 0.3 = 2457.6
    range_2048_4_03 = 2458,
    // 2048  * 8 * 0.3 = 4915.2
    range_2048_8_03 = 4915
};

class ScanView: public QGraphicsView
{
    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
};


class ScanSceneDialogBox : public QDialog, public DialogWindow
{
    Q_OBJECT
public:

    ScanSceneDialogBox(ScanBufferSceneModule *mParent, QWidget *wparent = nullptr);
    ~ScanSceneDialogBox() override;

    QList<std::pair<QString, QVariant>> GetSetting() override;
    bool SetSetting(QList<std::pair<QString, QVariant>> listSetting) override;
    void ShowDialog() override;
    void SendCommand() override;


    void Attach(ChangeEvent* observer) override;
    void Notification() override;

    uint8_t GetCoefStreatch() const;
    int GetVerticalShift() const;
    int GetGain() const ;
    int GetUpdateStep() const;

    bool GetUpdateScan() const;
    bool GetIsColorView() const;
    bool GetIsScanLine() const;
    bool GetIsGridOnScan() const;

    void SetCoefStreatch(uint8_t coef);
    void SetVerticalShift(int shift);
    void SetGain(int gain);
    void SetUpdateStep(int updateStep);

    void SetIsGridOnScan(bool isGridOnScan);
    void SetUpdateScan(bool isUpdateScan);
    void SetColorView(bool isColorView);
    void SetIsScanLine(bool isScanLine);

    void SetVRLIModule(VRLIViewModule* vrliViewModule);

    void ClearVRLI();

public slots:

    void SetGrid(bool state);
    void ZoomIn();
    void ZoomOut();
    void SetRange(uint32_t val);
    void SetViewColor(bool isViewColor);
    void SetLineVisible(bool visible);
    void SetScanMode(bool);
    // включение режима отображения OpenGL
    void OpenGLEnable(bool isOGL);

protected slots:
    // слот для кнопки пременить
    void SendCommandAbstractModule();
    // фукция создания сектора отображения
    void CreateSector(int simpleSizeAzimuth);
    void DumpEvent(qreal bering, qreal range);
    void UpdataScene();

    void UpdateVRLI();

    void ChangeColorGrid();
    void ChangeColorTarget();

    void PrintScreen();

protected:
    // изменение размеров формы
    void resizeEvent(QResizeEvent *) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:

    void CreateGrid();
    void CreateStyle();
    // Сброс секторов
    void ResetSectors();
    // Добавление сектора
    void AddSector(SectorArc* sector);

    void CreateContextMenu();

    void WriteSettingColor();
    void ReadSettingColor();

    // указательна модуль
    ScanBufferSceneModule* m_module;
    // параметры для отображения блоков
    ScanView* m_view;
    QGraphicsScene* m_scene;
    //Отображение сетки на секторе
    SectorGridBaseItem* m_grid;
    // количество углов в сетки
    size_t m_simpleSizeAzimuth = 64;
    // для быстрого доступа к элементам сектора
    std::vector<SectorArc*> m_vectorSector;

    PanelScan* m_panelScan;
    // размер прямоугольника 2*1024 ширина и высота
    qreal m_sizeRect = 1024 + 20;
    bool m_lineVisible = true;
    bool m_rubberBandIsShown = false;
    QRect m_rubberBandRect;

    ChangeEvent* m_observer = nullptr;

    VRLIViewModule* m_vrliViewModule = nullptr;

    QGraphicsItemGroup* m_currentShowTargets = nullptr;
    QList<QGraphicsItem *> grupList;

    float m_currentResolution = 0.3f;

    QAction* m_colorGrid;
    QAction* m_colorTarget;

};

#endif // SCANSCENEDIALOGBOX_H
