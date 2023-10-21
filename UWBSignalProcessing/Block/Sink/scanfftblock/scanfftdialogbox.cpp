#include "scanfftdialogbox.h"

ScanfftDialogBox::ScanfftDialogBox(QWidget *wparent)
    : QDialog(wparent, Qt::Window | Qt::WindowSystemMenuHint |
                           Qt::WindowMinimizeButtonHint |
                           Qt::WindowMaximizeButtonHint |
                           Qt::WindowCloseButtonHint) {
  m_view = new ScanView();
  m_scene = new QGraphicsScene(-m_margin, -m_margin, m_sizeRect + 2 * m_margin,
                               m_sizeRect + 2 * m_margin, this);
  m_view->setScene(m_scene);
  m_view->setBackgroundBrush(QBrush(Qt::black));
  auto layoutH = new QHBoxLayout;
  layoutH->addWidget(m_view);
  setLayout(layoutH);
  resizeEvent(nullptr);
  connect(this, SIGNAL(IsUpdate()), SLOT(UpdataScene()), Qt::QueuedConnection);

  InitSetting();
  CreateSector(m_range);
  CreateContextMenu();
}

QList<std::pair<QString, QVariant>> ScanfftDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("range", m_range));
  listSetting.append(std::make_pair("resolution", m_resolution));
  listSetting.append(std::make_pair("frec", m_frec));
  return listSetting;
}

bool ScanfftDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() < 3)
    return false;

  for (auto &val : listSetting) {
    if (val.first == "range")
      m_range = val.second.toInt();

    if (val.first == "resolution")
      m_resolution = val.second.toInt();

    if (val.first == "frec")
      m_frec = val.second.toInt();
  }

  InitSetting();
  return true;
}

void ScanfftDialogBox::ShowDialog() {
  if (isHidden()) {
    show();
    // showFullScreen();
  } else {
    raise();
    activateWindow();
    showNormal();
  }
}

void ScanfftDialogBox::SendCommandAbstractModule() {
  m_scene->update(m_scene->sceneRect());
  Notification();
}

void ScanfftDialogBox::resizeEvent(QResizeEvent *) {
  // выполняем авто марштабирование
  if (m_grid) {
    //        QRectF rect = m_grid->boundingRect();
    //        if(m_drawCurverProxy)
    //        {
    //            qreal h = m_drawCurverProxy->size().height();
    //            //            qreal w = m_drawCurverProxy->size().width();
    //            rect.setHeight(h + rect.height());
    //            //            rect.setWidth(w + rect.width());
    //        }
    QRectF rect = m_scene->itemsBoundingRect();
    rect.setTop(-m_margin);
    m_scene->setSceneRect(rect);
    //        m_view->fitInView(rect, Qt::KeepAspectRatio);
  }
  //    else
  //        m_view->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void ScanfftDialogBox::UpdataScene() { m_scene->update(); }

void ScanfftDialogBox::OpenCountRange() {
  auto dialog = new CountRangeDialog();
  dialog->SetCount(m_range);
  dialog->SetResolution(m_resolution);
  dialog->SetFrec(m_frec);
  if (dialog->exec() == QDialog::Accepted) {
    m_range = dialog->GetCount();
    m_resolution = dialog->GetResolution();
    m_frec = dialog->GetFrec();
    InitSetting();
  }
  delete dialog;
}

void ScanfftDialogBox::OpenSettings() {
  auto dialog = new SettingDialog();
  dialog->SetIsLog(m_isLog);
  dialog->SetLinePloter(m_linePloter);
  dialog->SetIsVerticalPloter(m_isVerticalPloter);
  if (dialog->exec() == QDialog::Accepted) {
    m_isLog = dialog->IsLog();
    m_linePloter = dialog->GetLinePloter();
    m_isVerticalPloter = dialog->IsVerticalPloter();
    SendCommand();
  }

  delete dialog;
}

void ScanfftDialogBox::ResetSectors() {
  if (m_vectorSector.empty())
    return;

  for (auto *sector : m_vectorSector)
    delete sector;

  m_vectorSector.clear();
}

void ScanfftDialogBox::AddSector(SectorRect *sector) {
  m_scene->addItem(sector);
  m_vectorSector.push_back(sector);
}

void ScanfftDialogBox::CreateGrid() {
  delete m_grid;
  m_grid = new SectorGrid();
  m_scene->addItem(m_grid);
  CreatePloter();
}

void ScanfftDialogBox::CreatePloter() {
  if (!m_drawCurverH) {
    m_drawCurverH = new DrawCurver();
    m_drawCurverH->SetPlotSetting(PlotSetting(512, 256, 0, 0, 10, 4));
    m_drawCurverProxyH = m_scene->addWidget(m_drawCurverH);
    //        m_drawCurver->setFixedSize(m_sizeRect, m_sizeRect / 6);
    QRectF rect(0, 0, m_sizeRect + 4 * m_margin, m_sizeRect / 3);
    m_drawCurverProxyH->setGeometry(rect);
  }

  m_drawCurverProxyH->setPos(-2 * m_margin, m_sizeRect + m_margin / 2);
  //    m_scene->addRect(m_drawCurverProxy->rect());
}

bool ScanfftDialogBox::GetIsVerticalPloter() const {
  return m_isVerticalPloter;
}

int ScanfftDialogBox::GetLinePloter() const { return m_linePloter; }

bool ScanfftDialogBox::GetIsLog() const { return m_isLog; }

void ScanfftDialogBox::CreateSector(int count) {
  m_range = count;
  // чистим вектора перед перестройкой сектора
  if (m_vectorSector.size() != static_cast<size_t>(count)) {
    ResetSectors();
    qreal width = 1024;
    qreal stepAngel = width / count;
    for (qreal i = 0; i < count; i++) {
      SectorRect *sector = new SectorRect(i * stepAngel, count);
      AddSector(sector);
    }
    Update();
    CreateGrid();
  }
}

void ScanfftDialogBox::InitSetting() {
  CreateSector(m_range);
  qreal maxRange = m_resolution * m_range / 100;
  m_grid->SetMaxRange(maxRange);
  qreal velocity = m_frec * 0.1 / 4; // частота * длинуВолны/4
  m_grid->SetMaxVelocity(velocity);
}

void ScanfftDialogBox::SendCommand() { SendCommandAbstractModule(); }

void ScanfftDialogBox::Attach(ChangeEvent *observer) { m_observer = observer; }

void ScanfftDialogBox::Notification() {
  if (m_observer)
    m_observer->Change();
}

void ScanView::mouseReleaseEvent(QMouseEvent *event) {
  QGraphicsView::mouseReleaseEvent(event);
}

void ScanfftDialogBox::Update() { emit IsUpdate(); }

void ScanfftDialogBox::ResizePloter(int size) {
  m_drawCurverH->ChangeSizeData(size);
}

std::vector<SectorBaseItem *> &ScanfftDialogBox::DataMatrix() {
  return m_vectorSector;
}

void ScanfftDialogBox::CreateContextMenu() {
  m_countRange = new QAction("Количество дискретов дальности");
  m_settings = new QAction("Настройки осей");
  connect(m_countRange, SIGNAL(triggered()), this, SLOT(OpenCountRange()));
  connect(m_settings, SIGNAL(triggered()), this, SLOT(OpenSettings()));

  m_view->addAction(m_countRange);
  m_view->addAction(m_settings);
  m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
}

CountRangeDialog::CountRangeDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint) {
  CreateDialog();
}

int CountRangeDialog::GetCount() { return m_countRange->value(); }

qreal CountRangeDialog::MaxRange() {
  return m_resolution->value() * GetCount() / 100;
}

qreal CountRangeDialog::MaxVelocity() { return m_F->value() * 0.1 / 4; }

void CountRangeDialog::SetCount(int count) { m_countRange->setValue(count); }

void CountRangeDialog::SetResolution(int resolution) {
  m_resolution->setValue(resolution);
}

void CountRangeDialog::SetFrec(int frec) { m_F->setValue(frec); }

int CountRangeDialog::GetResolution() { return m_resolution->value(); }

int CountRangeDialog::GetFrec() { return m_F->value(); }

void CountRangeDialog::CreateDialog() {
  auto layout = new QGridLayout;

  m_countRange = new QSpinBox;
  m_countRange->setRange(1, 1024);
  m_resolution = new QSpinBox;
  m_resolution->setRange(1, 100);

  m_F = new QSpinBox;
  m_F->setRange(100, 100000);

  QPushButton *pbOk = new QPushButton("&Ок");
  QPushButton *pbCencel = new QPushButton("О&тмена");

  connect(pbOk, SIGNAL(clicked()), SLOT(accept()));
  connect(pbCencel, SIGNAL(clicked()), SLOT(reject()));

  layout->addWidget(new QLabel("Количество дискретов"), 0, 0);
  layout->addWidget(m_countRange, 0, 1);
  layout->addWidget(new QLabel("Разрешение по дальности,см"), 1, 0);
  layout->addWidget(m_resolution, 1, 1);

  layout->addWidget(new QLabel("Частота повторения,Гц"), 2, 0);
  layout->addWidget(m_F, 2, 1);

  layout->addWidget(pbOk, 3, 0);
  layout->addWidget(pbCencel, 3, 1);
  setLayout(layout);
}

SettingDialog::SettingDialog() { CreateDialog(); }

void SettingDialog::SetIsLog(bool isLog) { m_isLog->setChecked(isLog); }

bool SettingDialog::IsLog() { return m_isLog->isChecked(); }

void SettingDialog::SetIsVerticalPloter(bool isVert) {
  m_isVerticalPloter->setChecked(isVert);
}

bool SettingDialog::IsVerticalPloter() {
  return m_isVerticalPloter->isChecked();
}

int SettingDialog::GetLinePloter() { return m_linePloter->value(); }

void SettingDialog::SetLinePloter(int linePloter) {
  m_linePloter->setValue(linePloter);
}

void SettingDialog::CreateDialog() {
  QGridLayout *layout = new QGridLayout;
  m_isLog = new QCheckBox("Log масштаб");
  QPushButton *pbOk = new QPushButton("&Ок");
  QPushButton *pbCencel = new QPushButton("О&тмена");

  m_linePloter = new QSpinBox;
  m_linePloter->setRange(0, 1024);
  m_isVerticalPloter = new QCheckBox("Вертикальная развертка плотера");
  m_isVerticalPloter->setChecked(false);

  connect(pbOk, SIGNAL(clicked()), SLOT(accept()));
  connect(pbCencel, SIGNAL(clicked()), SLOT(reject()));

  layout->addWidget(m_isLog, 0, 0);
  layout->addWidget(m_isVerticalPloter, 0, 1);

  layout->addWidget(new QLabel("Номер линии"), 1, 0);
  layout->addWidget(m_linePloter, 1, 1);

  layout->addWidget(pbOk, 2, 0);
  layout->addWidget(pbCencel, 2, 1);
  setLayout(layout);
}

QVector<QPointF> &ScanfftDialogBox::DataPloter() {
  // if(m_drawCurverH)
  return m_drawCurverH->GetCuver();
}

void ScanfftDialogBox::UpdatePloter() {
  if (m_drawCurverH)
    m_drawCurverH->RefreshPixmap();
}
