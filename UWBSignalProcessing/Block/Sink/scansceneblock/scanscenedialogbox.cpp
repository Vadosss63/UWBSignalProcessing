#include "scanscenedialogbox.h"
#include "ui_panelscan.h"
#include <QColorDialog>
#include <QGraphicsPathItem>
#include <QGraphicsProxyWidget>
#include <QGroupBox>
#include <QLabel>
#include <QOpenGLWidget>
#include <QResizeEvent>
#include <QScrollBar>
#include <QSettings>
#include <QToolBar>

ScanSceneDialogBox::ScanSceneDialogBox(ScanBufferSceneModule *mParent,
                                       QWidget *wparent)
    : QDialog(wparent, Qt::Window | Qt::WindowSystemMenuHint |
                           Qt::WindowMinimizeButtonHint |
                           Qt::WindowMaximizeButtonHint |
                           Qt::WindowCloseButtonHint),
      m_module(mParent), m_grid(nullptr) {
  m_view = new ScanView();

  m_scene =
      new QGraphicsScene(-m_sizeRect / std::sqrt(2), -m_sizeRect,
                         2 * (m_sizeRect / std::sqrt(2)), m_sizeRect, this);
  m_view->setScene(m_scene);

  m_view->setBackgroundBrush(QBrush(Qt::black));
  //    m_view->setCacheMode(QGraphicsView::CacheBackground);

  m_view->setDragMode(QGraphicsView::RubberBandDrag);

  QHBoxLayout *layoutH = new QHBoxLayout;
  layoutH->addWidget(m_view);
  m_panelScan = new PanelScan(this);
  m_panelScan->setFixedWidth(m_panelScan->sizeHint().width());

  QCheckBox *OGLButton = new QCheckBox;
  OGLButton->setChecked(false);
  connect(OGLButton, SIGNAL(clicked(bool)), this, SLOT(OpenGLEnable(bool)));

  //    layoutH->addWidget(OGLButton);
  layoutH->addWidget(m_panelScan);

  //    QGraphicsProxyWidget* proxyWidget = m_scene->addWidget(m_panelScan);
  //    proxyWidget->setPos(0, 0);

  setLayout(layoutH);
  connect(mParent, SIGNAL(ScanUpdated()), this, SLOT(UpdataScene()));
  CreateSector(1);
  DumpEvent(0, 0);
  resizeEvent(nullptr);
  SetRange(rangeValue::range_2048_8_03);
  setProperty("setStyle", true);
  m_panelScan->setProperty("setStyle", true);
  CreateStyle();

  connect(m_panelScan->GetUi()->pushButton, SIGNAL(clicked()), this,
          SLOT(SendCommandAbstractModule()));
  connect(m_panelScan->GetUi()->zoomIn, SIGNAL(clicked()), this,
          SLOT(ZoomIn()));
  connect(m_panelScan->GetUi()->zoomOut, SIGNAL(clicked()), this,
          SLOT(ZoomOut()));
  connect(m_panelScan->GetUi()->isLineOnScan, SIGNAL(clicked(bool)), this,
          SLOT(SetLineVisible(bool)));
  connect(m_panelScan->GetUi()->isColorScan, SIGNAL(clicked(bool)), this,
          SLOT(SetViewColor(bool)));
  connect(m_panelScan->GetUi()->isScanMode, SIGNAL(clicked(bool)), this,
          SLOT(SetScanMode(bool)));

  CreateGrid();
  CreateContextMenu();
  ReadSettingColor();
}

ScanSceneDialogBox::~ScanSceneDialogBox() { WriteSettingColor(); }

QList<std::pair<QString, QVariant>> ScanSceneDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.push_back(
      std::make_pair(QString("CoefStreatch"), GetCoefStreatch()));
  listSetting.push_back(
      std::make_pair(QString("VerticalShift"), GetVerticalShift()));
  listSetting.push_back(std::make_pair(QString("Gain"), GetGain()));
  listSetting.push_back(std::make_pair(QString("UpdateStep"), GetUpdateStep()));
  listSetting.push_back(std::make_pair(QString("UpdateScan"), GetUpdateScan()));
  listSetting.push_back(
      std::make_pair(QString("IsColorView"), GetIsColorView()));
  listSetting.push_back(std::make_pair(QString("IsScanLine"), GetIsScanLine()));
  listSetting.push_back(
      std::make_pair(QString("IsGridOnScan"), GetIsGridOnScan()));
  return listSetting;
}

bool ScanSceneDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 8)
    return false;

  foreach (auto &parmSetting, listSetting) {
    if (parmSetting.first == "CoefStreatch")
      SetCoefStreatch(static_cast<uint8_t>(parmSetting.second.toUInt()));

    if (parmSetting.first == "VerticalShift")
      SetVerticalShift(static_cast<int>(parmSetting.second.toInt()));

    if (parmSetting.first == "Gain")
      SetGain(static_cast<int>(parmSetting.second.toInt()));

    if (parmSetting.first == "UpdateStep")
      SetUpdateStep(static_cast<int>(parmSetting.second.toInt()));

    if (parmSetting.first == "UpdateScan")
      SetUpdateScan(parmSetting.second.toBool());

    if (parmSetting.first == "IsColorView")
      SetColorView(parmSetting.second.toBool());

    if (parmSetting.first == "IsScanLine")
      SetIsScanLine(parmSetting.second.toBool());

    if (parmSetting.first == "IsGridOnScan")
      SetIsGridOnScan(parmSetting.second.toBool());
  }
  return true;
}

void ScanSceneDialogBox::SetGrid(bool) {
  m_grid->SetVisible(m_panelScan->GetIsGridOnScan());
  m_grid->update();
}

void ScanSceneDialogBox::ShowDialog() {
  if (isHidden()) {
    //        show();
    showFullScreen();
  } else {
    raise();
    activateWindow();
    showNormal();
  }
}

void ScanSceneDialogBox::ZoomIn() { m_view->scale(1.1, 1.1); }

void ScanSceneDialogBox::ZoomOut() { m_view->scale(1 / 1.1, 1 / 1.1); }

void ScanSceneDialogBox::SetRange(uint32_t val) { m_panelScan->SetRange(val); }

void ScanSceneDialogBox::SetViewColor(bool isViewColor) {
  m_module->SetColorView(isViewColor);
}

void ScanSceneDialogBox::SendCommandAbstractModule() {
  m_module->SetValue(m_panelScan->GetVerticalShift(), m_panelScan->GetGain(),
                     m_panelScan->GetCoefStreatch());

  m_scene->update(m_scene->sceneRect());

  if (m_grid) {
    float resolution =
        2.f * m_currentResolution / m_panelScan->GetCoefStreatch();
    m_grid->SetResolution(resolution);
  }

  Notification();

  PrintScreen();
}

void ScanSceneDialogBox::resizeEvent(QResizeEvent *) {
  // выполняем авто марштабирование
  if (m_grid)
    m_view->fitInView(m_grid->boundingRect(), Qt::KeepAspectRatio);
  else
    m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void ScanSceneDialogBox::mouseReleaseEvent(QMouseEvent *event) {
  m_view->rubberBandRect();
  QWidget::mouseReleaseEvent(event);
}

void ScanSceneDialogBox::DumpEvent(qreal bering, qreal range) {
  m_panelScan->DumpEvent(bering, range);
}

void ScanSceneDialogBox::SetLineVisible(bool) {
  m_module->SetWhiteLine(m_panelScan->GetIsScanLine());
}

void ScanSceneDialogBox::SetScanMode(bool) {
  m_module->SetUpdateScan(m_panelScan->GetUpdateScan(),
                          m_panelScan->GetUpdateStep());
}

void ScanSceneDialogBox::UpdataScene() { m_scene->update(); }

void ScanSceneDialogBox::ClearVRLI() {
  if (m_vrliViewModule) {
    if (m_currentShowTargets) {
      m_scene->destroyItemGroup(m_currentShowTargets);
      m_currentShowTargets = nullptr;
      qDeleteAll(grupList);
      grupList.clear();
    }
  }
}

void ScanSceneDialogBox::UpdateVRLI() {
  if (m_vrliViewModule) {
    ClearVRLI();

    for (VRLIItem *item : m_vrliViewModule->GetVRLIItem())
      grupList.append(item);

    m_currentShowTargets = m_scene->createItemGroup(grupList);
    m_vrliViewModule->GetVRLIItem().clear();
    UpdataScene();
  }
}

void ScanSceneDialogBox::ChangeColorGrid() {
  QColor color =
      QColorDialog::getColor(m_grid->GetColorGrid(), this, "Цвет сетки");
  if (color.isValid()) {
    m_grid->SetColorGrid(color);
    m_grid->update();
  }
}

void ScanSceneDialogBox::ChangeColorTarget() {
  QColor color =
      QColorDialog::getColor(VRLIItem::GetColorItem(), this, "Цвет целей");
  if (color.isValid()) {
    VRLIItem::SetColorItem(color);
  }
}

void ScanSceneDialogBox::PrintScreen() {
  QImage img(m_scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
  img.fill(Qt::black);
  QPainter painter(&img);
  m_scene->render(&painter);
  img.save("/home/vadosss63/PrintScreen.png", "png");
}

void ScanSceneDialogBox::CreateGrid() {
  delete m_grid;

  m_grid = new SectorArcGridItem;
  m_grid->CreateGrid();

  connect(m_panelScan->GetUi()->isGridOnScan, SIGNAL(clicked(bool)), this,
          SLOT(SetGrid(bool)));
  connect(m_grid, SIGNAL(SelectPoint(qreal, qreal)), this,
          SLOT(DumpEvent(qreal, qreal)));
  m_scene->addItem(m_grid);
  SetGrid(true);
}

void ScanSceneDialogBox::ResetSectors() {
  if (m_vectorSector.empty())
    return;

  for (auto *sector : m_vectorSector)
    delete sector;

  m_vectorSector.clear();
  // матрица удаляется при удалении сектора
  m_module->DataMatrix().clear();
}

void ScanSceneDialogBox::AddSector(SectorArc *sector) {
  m_scene->addItem(sector);
  m_module->DataMatrix().push_back(sector);
  m_vectorSector.push_back(sector);
}

void ScanSceneDialogBox::CreateContextMenu() {
  m_colorGrid = new QAction("Цвет сетки");
  m_colorTarget = new QAction("Цвет целей");
  connect(m_colorGrid, SIGNAL(triggered()), this, SLOT(ChangeColorGrid()));
  connect(m_colorTarget, SIGNAL(triggered()), this, SLOT(ChangeColorTarget()));

  m_view->addAction(m_colorGrid);
  m_view->addAction(m_colorTarget);
  m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void ScanSceneDialogBox::CreateSector(int simpleSizeAzimuth) {
  m_simpleSizeAzimuth = static_cast<size_t>(std::pow(2, 5 + simpleSizeAzimuth));
  // опеределения шага между углами
  qreal stepAngel = 90.0 / m_simpleSizeAzimuth;

  // чистим вектора перед перестройкой сектора
  ResetSectors();

  for (qreal bering = 0, i = -45; i < 45; i += stepAngel, bering++) {
    qreal currentBering = bering * stepAngel - 45.;
    SectorArc *sector = new SectorArc(i, currentBering, stepAngel);
    //        connect(sector, SIGNAL(PressEvent(qreal, qreal)), this ,
    //        SLOT(DumpEvent(qreal, qreal)));
    AddSector(sector);
  }
  m_scene->update();
}

void ScanSceneDialogBox::SendCommand() { SendCommandAbstractModule(); }

uint8_t ScanSceneDialogBox::GetCoefStreatch() const {
  return m_panelScan->GetCoefStreatch();
}

int ScanSceneDialogBox::GetVerticalShift() const {
  return m_panelScan->GetVerticalShift();
}

int ScanSceneDialogBox::GetGain() const { return m_panelScan->GetGain(); }

int ScanSceneDialogBox::GetUpdateStep() const {
  return m_panelScan->GetUpdateStep();
}

bool ScanSceneDialogBox::GetUpdateScan() const {
  return m_panelScan->GetUpdateScan();
}

bool ScanSceneDialogBox::GetIsColorView() const {
  return m_panelScan->GetIsColorView();
}

bool ScanSceneDialogBox::GetIsScanLine() const {
  return m_panelScan->GetIsScanLine();
}

bool ScanSceneDialogBox::GetIsGridOnScan() const {
  return m_panelScan->GetIsGridOnScan();
}

void ScanSceneDialogBox::SetCoefStreatch(uint8_t coef) {
  m_panelScan->SetCoefStreatch(coef);
}

void ScanSceneDialogBox::SetVerticalShift(int shift) {
  m_panelScan->SetVerticalShift(shift);
}

void ScanSceneDialogBox::SetGain(int gain) { m_panelScan->SetGain(gain); }

void ScanSceneDialogBox::SetUpdateStep(int updateStep) {
  m_panelScan->SetUpdateStep(updateStep);
}

void ScanSceneDialogBox::SetIsGridOnScan(bool isGridOnScan) {
  m_panelScan->SetIsGridOnScan(isGridOnScan);
}

void ScanSceneDialogBox::SetUpdateScan(bool isUpdateScan) {
  m_panelScan->SetUpdateScan(isUpdateScan);
  SetScanMode(isUpdateScan);
}

void ScanSceneDialogBox::SetColorView(bool isColorView) {
  m_panelScan->SetColorView(isColorView);
  SetViewColor(isColorView);
}

void ScanSceneDialogBox::SetIsScanLine(bool isScanLine) {
  m_panelScan->SetIsScanLine(isScanLine);
  SetLineVisible(isScanLine);
}

void ScanSceneDialogBox::Attach(ChangeEvent *observer) {
  m_observer = observer;
}

void ScanSceneDialogBox::Notification() {
  if (m_observer)
    m_observer->Change();
}

void ScanSceneDialogBox::SetVRLIModule(VRLIViewModule *vrliViewModule) {
  m_vrliViewModule = vrliViewModule;
  connect(vrliViewModule, SIGNAL(ScanUpdated()), this, SLOT(UpdateVRLI()));
}

void ScanSceneDialogBox::OpenGLEnable(bool isOGL) {
  if (isOGL)
    m_view->setViewport(new QOpenGLWidget());
  else
    m_view->setViewport(new QWidget());
}

void ScanView::mouseReleaseEvent(QMouseEvent *event) {
  /// TODO сделать обработку картинки
  //   QRectF rect = rubberBandRect().normalized();
  //   if(rect.width() >= 10 && rect.height() >= 10)
  //       fitInView(rect, Qt::KeepAspectRatioByExpanding);

  //    QImage img(rubberBandRect().size(), QImage::Format_Mono);
  //    QPainter painter(&img);
  //    render(&painter);
  //    img.save("PrintScreen.png", "png");

  QGraphicsView::mouseReleaseEvent(event);
}

void ScanSceneDialogBox::CreateStyle() {

  /// для настройки стилей отображения
  m_view->setProperty("name", "ScanDialog");
  m_panelScan->setProperty("name", "ScanDialogPanelScan");

  setStyleSheet("background-color: #000000;");
  m_view->setStyleSheet("*{border: 2px solid white; border-radius: 10px; "
                        "border: 1px solid white; border-radius: 5px;}"

  );

  m_view->verticalScrollBar()->setStyleSheet(
      "QAbstractScrollArea::QScrollBar{"
      " background-color: rgb(150, 150, 150); "
      "}"

      "QAbstractScrollArea::QScrollBar::handle:vertical, "
      "QScrollBar::handle:horisontal{"
      "background-color: rgb(100, 100, 100); "
      "margin: 10px;"
      " padding 10px;"
      "}");

  m_view->horizontalScrollBar()->setStyleSheet(
      "QAbstractScrollArea::QScrollBar{"
      " background-color: rgb(150, 150, 150); "
      "}"

      "QAbstractScrollArea::QScrollBar::handle:vertical, "
      "QScrollBar::handle:horisontal{"
      "background-color: rgb(100, 100, 100); "
      "margin: 10px;"
      " padding 10px;"
      "}");

  m_panelScan->setStyleSheet(
      "*{color: white;background-color: black;}"
      "QGroupBox {"
      "margin: 5 ; border: 1px solid white; border-radius: 5px; padding: 10px "
      "4px 4px 0px; "
      "}"
      "QGroupBox::title {"
      "margin: 0 ; subcontrol-position: center top;"
      "}"
      "QPushButton {"
      "min-width: 16px; background: qlineargradient(x1:0, y1:1, x2:0, y2:0, "
      "stop:1 rgb(133, 133, 135), stop:0.4 rgb(31, 31, 33,));"
      "color: white; border: 1px solid white; border-radius: 5px;"
      "}"
      "QPushButton:hover {"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:1 rgb(133, "
      "133, 135), stop:0.4 rgb(31, 31, 33), stop:0.2 rgb(0, 0, 150,) );"
      "}"
      "QPushButton:pressed {background: qlineargradient(x1:0, y1:1, x2:0, "
      "y2:0, stop:0 rgba(1, 1, 5, 80), stop:0.6 rgba(18, 18, 212, 80), "
      "stop:0.5 rgba(142, 142, 245, 80) );"
      "border: 1px solid rgb(18, 18, 212);"
      "}"
      "QCheckBox::indicator:unchecked {"
      "border: 1px solid white;"
      "}"
      "QCheckBox{"
      "color: white;"
      "background-color: black;"
      " }"
      "QComboBox{"
      "background-color: black;"
      "}"
      "QComboBox QListView{"
      "border: 1px solid white;"
      "}"
      "QSpinBox{"
      " color: white;"
      " padding: 0px 0px 0px 5px;"
      " background-color: black;"
      " selection-background-color: white;"
      " selection-color: black;"
      " border-style: outset;"
      "border-width: 0px;"
      "border: 1px solid white;"
      "border-radius: 11px;"
      "}"
      "QSpinBox::up-button {"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:1 rgb(133, "
      "133, 135), stop:0.4 rgb(31, 31, 33,));"
      "border: 1px solid  white; border-top-right-radius: 10px;"
      "border-top-left-radius: 3px; subcontrol-position: top right; "
      "subcontrol-origin: padding;"
      "}"
      "QSpinBox::up-button:pressed{"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:0 rgba(1, 1, "
      "5, 80), stop:0.6 rgba(18, 18, 212, 80), stop:0.5 rgba(142, 142, 245, "
      "80) );"
      "border: 1px solid rgb(18, 18, 212);"
      "    }"
      "QSpinBox::up-button:hover {"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:1 rgb(133, "
      "133, 135), stop:0.4 rgb(31, 31, 33), stop:0.2 rgb(0, 0, 150,) );"
      " }"
      "QSpinBox::down-button {"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:1 rgb(133, "
      "133, 135), stop:0.4 rgb(31, 31, 33,));"
      "border: 1px solid  white; border-bottom-right-radius: 10px; "
      "border-bottom-left-radius: 3px; subcontrol-position: bottom right; "
      "subcontrol-origin: padding;"
      "    }"
      "QSpinBox::down-button:pressed{"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:0 rgba(1, 1, "
      "5, 80), stop:0.6 rgba(18, 18, 212, 80), stop:0.5 rgba(142, 142, 245, "
      "80) );"
      "border: 1px solid rgb(18, 18, 212);"
      " }"
      " QSpinBox::down-button:hover{"
      "background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:1 rgb(133, "
      "133, 135), stop:0.4 rgb(31, 31, 33), stop:0.2 rgb(0, 0, 150,) );"
      "}"

      "QScrollBar::handle:vertical, QScrollBar::handle:horisontal{"
      "background: rgb(130, 90, 178);"
      "color: green;"
      " margin: 10px;"
      " padding: 10px;}"

  );
}

void ScanSceneDialogBox::WriteSettingColor() {
  QSettings settings("Radar MMS", "UWBSignalProcessing");
  settings.beginGroup("ScanSceneDialogBox");
  settings.setValue("ColorGrid", m_grid->GetColorGrid());
  settings.setValue("ColorTarget", VRLIItem::GetColorItem());
  settings.endGroup();
}

void ScanSceneDialogBox::ReadSettingColor() {
  QSettings settings("Radar MMS", "UWBSignalProcessing");
  settings.beginGroup("ScanSceneDialogBox");
  if (!settings.allKeys().isEmpty()) {
    QColor colorGrig = settings.value("ColorGrid").value<QColor>();
    if (colorGrig.isValid())
      m_grid->SetColorGrid(colorGrig);
    QColor colorTarget = settings.value("ColorTarget").value<QColor>();

    if (colorTarget.isValid())
      VRLIItem::SetColorItem(colorTarget);
  }
  settings.endGroup();
}
