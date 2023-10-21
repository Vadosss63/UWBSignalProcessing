#include "showtargetdialogbox.h"
#include <QGraphicsProxyWidget>
#include <QMovie>
#include <QScrollBar>
#include <QThread>
#include <QTimer>
#include <utility>

ShowTargetDialogBox::ShowTargetDialogBox(QWidget *wparent)
    : QDialog(wparent, Qt::Window | Qt::WindowSystemMenuHint |
                           Qt::WindowMinimizeButtonHint |
                           Qt::WindowMaximizeButtonHint |
                           Qt::WindowCloseButtonHint) {
  CreateScene();
  auto *layoutH = new QHBoxLayout;
  layoutH->addWidget(m_view);
  setLayout(layoutH);
  resizeEvent(nullptr);
  InitSetting();
  CreateContextMenu();
  CreateGrid();

  qRegisterMetaType<QVector<TargetItem *>>("QVector<TargetItem*>");

  connect(this, SIGNAL(AddData(QVector<TargetItem *>)),
          SLOT(Add(QVector<TargetItem *>)), Qt::QueuedConnection);

  ShowDialog();
}

QList<std::pair<QString, QVariant>> ShowTargetDialogBox::GetSetting() {
  QList<std::pair<QString, QVariant>> listSetting;
  listSetting.append(std::make_pair("maxAMP", TargetItem::GetMaxAMP()));
  listSetting.append(
      std::make_pair("maxAMPdymamic", TargetItem::GetMaxAMPdymamic()));
  return listSetting;
}

bool ShowTargetDialogBox::SetSetting(
    QList<std::pair<QString, QVariant>> listSetting) {
  if (listSetting.size() != 2)
    return false;

  for (auto &parmSetting : listSetting) {
    if (parmSetting.first == "maxAMP")
      TargetItem::SetMaxAMP(parmSetting.second.toInt());

    if (parmSetting.first == "maxAMPdymamic")
      TargetItem::SetMaxAMPdymamic(parmSetting.second.toInt());
  }
  return true;
}

void ShowTargetDialogBox::ShowDialog() {
  if (isHidden()) {
    show();
    showFullScreen();
  } else {
    raise();
    activateWindow();
    showNormal();
  }
}

void ShowTargetDialogBox::SendCommandAbstractModule() {
  m_scene->update(m_scene->sceneRect());
  Notification();
}

void ShowTargetDialogBox::resizeEvent(QResizeEvent *) {
  // выполняем авто марштабирование
  int marg = 50;
  QMargins qMarg(marg, marg, marg, marg);
  if (m_grid)
    m_view->fitInView(m_grid->boundingRect().marginsAdded(qMarg),
                      Qt::KeepAspectRatio);
  else
    m_view->fitInView(m_scene->sceneRect().marginsAdded(qMarg),
                      Qt::KeepAspectRatio);
}

//// человек
// void ShowTargetDialogBox::AddTarget(const double y , const double x)
//{
//     static QGraphicsProxyWidget* item = nullptr;
//     if(!item)
//     {
//         QRectF rectTarget(0, 0, 15, 15);
//         rectTarget.moveCenter(QPointF(SectorGrid::Val_XToPixel(x),
//         SectorGrid::Val_YToPixel(y))); QLabel* label = new QLabel(); QMovie*
//         movie = new QMovie(":/man.gif"); label->setMovie(movie);
//         label->setAttribute(Qt::WA_NoSystemBackground);
//         movie->start();
//         //        label->setFixedSize(QSize(15,15));
//         item = m_scene->addWidget(label);
//         item->setGeometry(rectTarget);
////        item->setScale(0.2);
//    }

//    QRectF rectTarget = item->rect();
//    rectTarget.moveCenter(QPointF(SectorGrid::Val_XToPixel(x),
//    SectorGrid::Val_YToPixel(y))); item->setGeometry(rectTarget);

//}

void ShowTargetDialogBox::AddTarget(std::vector<Target> &&targets) {
  QVector<TargetItem *> tar;
  tar.reserve(tar.size());
  for (const auto &target : targets) {
    auto *item = new TargetItem(target);
    tar.append(item);
    tar.append(item->GetItemZoom());
  }
  emit AddData(tar);
}

void ShowTargetDialogBox::ResetTarget() {
  if (thread() != QThread::currentThread())
    QMetaObject::invokeMethod(this, "ResetTargetScene", Qt::QueuedConnection);
  else
    ResetTargetScene();
}

void ShowTargetDialogBox::Update() {
  if (thread() != QThread::currentThread())
    QMetaObject::invokeMethod(this, "UpdataScene", Qt::QueuedConnection);
  else
    UpdataScene();
}

void ShowTargetDialogBox::UpdataScene() {
  for (auto *target : m_target)
    m_scene->addItem(target);

  if (!m_target.isEmpty())
    m_grid->ShowCurrentSpeed(m_target.first()->GetCurrentSpeed());

  m_scene->update();
}
#include <QDebug>
void ShowTargetDialogBox::ResetTargetScene() {
  if (m_target.isEmpty())
    return;

  for (TargetItem *item : m_target) {
    delete item;
    item = nullptr;
  }
  m_target.clear();
}

void ShowTargetDialogBox::Add(QVector<TargetItem *> targets) {
  ResetTargetScene();
  m_target = std::move(targets);
  Update();
}

void ShowTargetDialogBox::OpenDialogSetting() {
  DialogSettingTarget dialog(TargetItem::GetMaxAMP(),
                             TargetItem::GetMaxAMPdymamic());
  if (dialog.exec()) {
    TargetItem::SetMaxAMP(dialog.GetMaxVal());
    TargetItem::SetMaxAMPdymamic(dialog.GetMaxValDynamic());
  }
}

void ShowTargetDialogBox::CreateGrid() {

  delete m_grid;

  m_grid = new SectorGrid();
  m_scene->addItem(m_grid);
}

void ShowTargetDialogBox::InitSetting() {}

void ShowTargetDialogBox::SendCommand() { SendCommandAbstractModule(); }

void ShowTargetDialogBox::Attach(ChangeEvent *observer) {
  m_observer = observer;
}

void ShowTargetDialogBox::Notification() {
  if (m_observer)
    m_observer->Change();
}

void ScanView::ZoomIn() { scale(1.5, 1.5); }

void ScanView::ZoomOriginal() { resetTransform(); }

void ScanView::ZoomOut() { scale(0.75, 0.75); }

void ScanView::mouseReleaseEvent(QMouseEvent *event) {
  QGraphicsView::mouseReleaseEvent(event);
}

void ScanView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Plus:
    if (event->modifiers() & Qt::ControlModifier)
      ZoomIn();
    break;
  case Qt::Key_Minus:
    if (event->modifiers() & Qt::ControlModifier)
      ZoomOut();
    break;

  case Qt::Key_0:
    if (event->modifiers() & Qt::ControlModifier)
      ZoomOriginal();
    break;
  default:
    break;
  }

  QGraphicsView::keyPressEvent(event);
}

void ShowTargetDialogBox::CreateContextMenu() {
  m_settings = new QAction("Настройки отображения");
  // connect(m_countRange, SIGNAL(triggered()), this, SLOT(OpenCountRange()));
  // connect(m_settings, SIGNAL(triggered()), this, SLOT(OpenSettings()));
  m_view->addAction(m_settings);
  m_view->setContextMenuPolicy(Qt::ActionsContextMenu);

  connect(m_settings, SIGNAL(triggered()), this, SLOT(OpenDialogSetting()));
}

void ShowTargetDialogBox::CreateScene() {
  m_view = new ScanView();
  m_scene = new QGraphicsScene(-m_margin, -m_margin, m_sizeRect + 2 * m_margin,
                               m_sizeRect + 2 * m_margin, this);
  m_view->setScene(m_scene);
  //    m_view->setBackgroundBrush(QBrush(Qt::white));
  setStyleSheet("background-color: #C0C0C0;");
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
}

DialogSettingTarget::DialogSettingTarget(int currentVal,
                                         int currentValDynamic) {

  auto *mainLayout = new QHBoxLayout;
  mainLayout->addWidget(new QLabel("Максимальное заначение амплитуды:"));
  m_maxVal = new QSpinBox();
  mainLayout->addWidget(m_maxVal);
  m_maxVal->setRange(0, 100000);
  m_maxVal->setFixedWidth(100);
  m_maxVal->setValue(currentVal);
  mainLayout->addWidget(new QLabel("СДЦ:"));
  m_maxValDynamic = new QSpinBox();
  mainLayout->addWidget(m_maxValDynamic);
  m_maxValDynamic->setRange(0, 100000);
  m_maxValDynamic->setFixedWidth(100);
  m_maxValDynamic->setValue(currentValDynamic);

  QPushButton *applay = new QPushButton("Применить");
  mainLayout->addWidget(applay);
  setLayout(mainLayout);
  connect(applay, SIGNAL(clicked()), this, SLOT(accept()));
}

int DialogSettingTarget::GetMaxVal() { return m_maxVal->value(); }

int DialogSettingTarget::GetMaxValDynamic() { return m_maxValDynamic->value(); }
