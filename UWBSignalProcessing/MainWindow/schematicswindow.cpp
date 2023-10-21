#include "schematicswindow.h"
#include "lineconnect.h"
#include <QFont>
#include <QPainterPath>
#include <QStatusBar>
#include <QToolBar>

SchematicsWindow::SchematicsWindow(QWidget *parent)
    : QMainWindow(parent), m_scene(new SchemticsController(this)),
      m_nodeBuffer(nullptr) {

  setWindowTitle("Схема");
  setWindowState(Qt::WindowMaximized);

  CreateActions();
  CreateEditToolBars();
  CreateActionBars();

  m_view = new QGraphicsView();
  m_view->setObjectName(tr("Схема"));
  m_view->setStyleSheet(
      "border: 2px groove gray; border-radius: 10px; padding: 6px 6px;");

  m_view->setAcceptDrops(true);
  m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_view->setScene(m_scene);
  m_view->setDragMode(QGraphicsView::RubberBandDrag);
  m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(UpdateActions()));
  UpdateActions();

  setCentralWidget(m_view);
  CreateContextMenu();
  m_itemDockWidget = new QDockWidget(tr("Выбор элементов"));
  m_itemDockWidget->setObjectName(tr("Выбор элементов"));
  m_itemDockWidget->setWidget(m_itemsDialogWidget);
  addDockWidget(Qt::LeftDockWidgetArea, m_itemDockWidget);
  connect(m_switchShowCloseAction, SIGNAL(triggered()), m_itemDockWidget,
          SLOT(show()));
  ReadSettings();
  // установка количества углов по умолчанию
  SetStyleSchemeWindow();
}

SchematicsWindow::~SchematicsWindow() {
  WriteSettings();
  emit m_scene->Stop();
  if (m_nodeBuffer)
    delete m_nodeBuffer;
}

void SchematicsWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_A:
    if (event->modifiers() & Qt::ControlModifier)
      SelecteAll();
    break;
  default:
    break;
  }
  QWidget::keyPressEvent(event);
}

void SchematicsWindow::OpenFile(QString path) {
  SchemeReader reader;

  if (reader.ReadFile(path)) {
    SelecteAll();
    Del();
    CreateScem(reader);
  }
}

void SchematicsWindow::SetOperationManager(AbstractOperationManager *manager) {
  m_scene->SetOperationManager(manager);
}

void SchematicsWindow::Open() {
  QString path =
      QFileDialog::getOpenFileName(this, "Открытие файла", "", "uwbs(*.uwbs)");
  OpenFile(path);
}

void SchematicsWindow::Save() {
  QString path = QFileDialog::getSaveFileName(this, "Сохранение файла", "",
                                              "uwbs(*.uwbs)");

  if (!path.endsWith(".uwbs"))
    path += QString(".uwbs");

  // выделяем все элементы;
  SelecteAll();

  // выполняем поиск блоков на сцене
  SchemeWriter save(path); // добавить путь для записи
  foreach (QGraphicsItem *item, m_scene->selectedItems()) {
    AbstractBlock *abstractBlock = dynamic_cast<AbstractBlock *>(item);
    if (abstractBlock) {
      save.SaveBlock(abstractBlock->GetBlockStruct());
      QVector<LinkStruct> linkStructs = abstractBlock->GetLinkStruct();

      for (auto &linkStruct : linkStructs)
        save.SaveLink(linkStruct);
    }
  }
}

void SchematicsWindow::Del() {
  QList<QGraphicsItem *> itemsDelete;
  // поиск для удаления и отброска мусора
  foreach (QGraphicsItem *item, m_scene->selectedItems())
    if (dynamic_cast<AbstractBlock *>(item))
      itemsDelete.append(item);
  qDeleteAll(itemsDelete);
}

void SchematicsWindow::Properties() {
  AbstractBlock *block = SelectedBlock();
  if (block)
    block->ShowDialog();
}

void SchematicsWindow::Cut() {
  Copy();
  Del();
  UpdateActions();
}

void SchematicsWindow::Copy() {
  if (m_nodeBuffer)
    delete m_nodeBuffer;
  m_nodeBuffer = SelectedBlock()->Clone();
  m_nodeBuffer->setPos(SelectedBlock()->pos());
  UpdateActions();
  if (!m_nodeBuffer)
    return;
}

void SchematicsWindow::Paste() {
  if (m_nodeBuffer) {
    m_scene->AddBlock(
        m_nodeBuffer->Clone(),
        QPointF(m_nodeBuffer->pos().x() + 20, m_nodeBuffer->pos().y() + 10));
    UpdateActions();
  }
}

void SchematicsWindow::UpdateActions() {
  if (IsStart())
    return;

  bool hasSelection = !m_scene->selectedItems().isEmpty();
  bool isBlock = (SelectedBlock() != nullptr);
  bool isPaste = (m_nodeBuffer != nullptr);
  m_cutAction->setEnabled(isBlock);
  m_copyAction->setEnabled(isBlock);
  m_deleteAction->setEnabled(hasSelection);
  m_propertiesAction->setEnabled(isBlock);
  m_pasteAction->setEnabled(isPaste);
}

void SchematicsWindow::ZoomIn() { m_view->scale(1.5, 1.5); }

void SchematicsWindow::ZoomOut() { m_view->scale(0.75, 0.75); }

void SchematicsWindow::SelecteAll() {
  QPainterPath path;
  path.addRect(m_scene->itemsBoundingRect());
  m_scene->setSelectionArea(path);
}

void SchematicsWindow::WriteSettings() {
  QSettings settings("Radar MMS", "UWBSignalProcessing");
  settings.beginGroup("SchematicsWindow");
  settings.setValue("geometry", saveGeometry());
  settings.setValue("state", saveState());
  settings.endGroup();
}

void SchematicsWindow::ReadSettings() {
  QSettings settings("Radar MMS", "UWBSignalProcessing");
  settings.beginGroup("SchematicsWindow");
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("state").toByteArray());
  settings.endGroup();
}

void SchematicsWindow::LockActions() {
  SetIsStart(true);
  m_statusBar->setText("");
  m_switchShowCloseAction->setEnabled(false);
  m_cutAction->setEnabled(false);
  m_copyAction->setEnabled(false);
  m_deleteAction->setEnabled(false);
  m_pasteAction->setEnabled(false);
  m_startAction->setEnabled(false);
  m_openAction->setEnabled(false);
  m_saveAction->setEnabled(false);
  m_stopAction->setEnabled(true);
  m_itemDockWidget->hide();
}

void SchematicsWindow::UnlockActions() {
  SetIsStart(false);
  if (!m_isRelease) {
    m_switchShowCloseAction->setEnabled(true);
    m_itemDockWidget->show();
    m_cutAction->setEnabled(true);
    m_copyAction->setEnabled(true);
    m_deleteAction->setEnabled(true);
    m_pasteAction->setEnabled(true);
    m_openAction->setEnabled(true);
    m_saveAction->setEnabled(true);
  }

  m_stopAction->setEnabled(false);
  m_startAction->setEnabled(true);
}

void SchematicsWindow::Start() { m_scene->Start(); }

void SchematicsWindow::SetIsRelease(bool isRelease) { m_isRelease = isRelease; }

void SchematicsWindow::CreateActions() {
  m_statusBar = new QLabel;
  statusBar()->addWidget(m_statusBar); // строка состояния подсказок

  m_openAction =
      new QAction(QIcon(":/Images/document-open.svg"), "Открыть", this);
  m_openAction->setShortcut(QKeySequence::Open);
  m_openAction->setStatusTip("Открыть схему");
  connect(m_openAction, SIGNAL(triggered()), this, SLOT(Open()));

  m_saveAction =
      new QAction(QIcon(":/Images/document-save.svg"), "Сохранить", this);
  m_saveAction->setShortcut(QKeySequence::Save);
  m_saveAction->setStatusTip("Сохранить схему");
  connect(m_saveAction, SIGNAL(triggered()), this, SLOT(Save()));

  m_cutAction = new QAction(QIcon(":/Images/edit-cut.svg"), "Вырезать", this);
  m_cutAction->setShortcut(QKeySequence::Cut);
  m_cutAction->setStatusTip("Вырезать блок");
  connect(m_cutAction, SIGNAL(triggered()), this, SLOT(Cut()));

  m_copyAction =
      new QAction(QIcon(":/Images/edit-copy.svg"), "Копировать", this);
  m_copyAction->setShortcut(QKeySequence::Copy);
  m_copyAction->setStatusTip("Копировать блок");
  connect(m_copyAction, SIGNAL(triggered()), this, SLOT(Copy()));

  m_deleteAction =
      new QAction(QIcon(":/Images/edit-delete.svg"), "Удалить", this);
  m_deleteAction->setShortcut(QKeySequence::Delete);
  m_deleteAction->setStatusTip("Удалить блок");
  connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(Del()));

  m_pasteAction =
      new QAction(QIcon(":/Images/edit-paste.svg"), "Вставить", this);
  m_pasteAction->setShortcut(QKeySequence::Paste);
  m_pasteAction->setStatusTip("Вставить блок");
  connect(m_pasteAction, SIGNAL(triggered()), this, SLOT(Paste()));

  m_propertiesAction =
      new QAction(QIcon(":/Images/preferences-desktop.svg"), "Свойства", this);
  m_propertiesAction->setStatusTip("Параметры блока");
  connect(m_propertiesAction, SIGNAL(triggered()), this, SLOT(Properties()));

  m_zoomoutAction =
      new QAction(QIcon(":/Images/zoom-out.svg"), "Уменьшить Ctrl-", this);
  m_zoomoutAction->setShortcut(QKeySequence::ZoomOut);
  m_zoomoutAction->setStatusTip("Уменьшить схему");
  connect(m_zoomoutAction, SIGNAL(triggered()), this, SLOT(ZoomOut()));

  m_zoominAction =
      new QAction(QIcon(":/Images/zoom-in.svg"), "Увеличить Ctrl+", this);
  m_zoominAction->setShortcut(QKeySequence::ZoomIn);
  m_zoominAction->setStatusTip("Увеличить схему");
  connect(m_zoominAction, SIGNAL(triggered()), this, SLOT(ZoomIn()));

  m_startAction =
      new QAction(QIcon(":/Images/media-playback-start.svg"), "Старт", this);
  m_startAction->setStatusTip("Запустить схему");
  connect(m_startAction, SIGNAL(triggered()), this, SLOT(LockActions()));
  connect(m_startAction, SIGNAL(triggered()), m_scene, SLOT(Start()));

  m_stopAction =
      new QAction(QIcon(":/Images/media-playback-stop.svg"), "Стоп", this);
  m_stopAction->setStatusTip("Остановить схему");
  connect(m_stopAction, SIGNAL(triggered()), m_scene, SLOT(Stop()));
  connect(m_scene, SIGNAL(StopChem()), this, SLOT(UnlockActions()));

  m_timer = new QAction(QIcon(":/Images/timer.png"), "Таймер", this);
  m_timer->setStatusTip("Настройки таймера на схеме");
  connect(m_timer, SIGNAL(triggered()), m_scene, SLOT(ShowSettingTimer()));

  m_switchShowCloseAction =
      new QAction(QIcon(":/Images/format-indent-more.svg"),
                  "Добавить элементы на схему", this);
  m_switchShowCloseAction->setStatusTip("Добавть блоки на схему");
  m_itemsDialogWidget = new ItemsDialogWidget;

  m_stopAction->setEnabled(false);
}

void SchematicsWindow::CreateMenus() {}

void SchematicsWindow::CreateEditToolBars() {
  m_editToolBar = addToolBar("editToolBar");
  m_editToolBar->setObjectName("m_editToolBar");
  m_editToolBar->addAction(m_openAction);
  m_editToolBar->addAction(m_saveAction);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_cutAction);
  m_editToolBar->addAction(m_copyAction);
  m_editToolBar->addAction(m_pasteAction);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_deleteAction);
  m_editToolBar->addAction(m_propertiesAction);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_zoomoutAction);
  m_editToolBar->addAction(m_zoominAction);
  m_editToolBar->addSeparator();
  m_editToolBar->setFixedWidth(m_editToolBar->sizeHint().width());
}

void SchematicsWindow::CreateActionBars() {
  m_actionToolBar = addToolBar("actionToolBar");
  m_actionToolBar->setObjectName("m_actionToolBar");
  m_actionToolBar->addAction(m_startAction);
  m_actionToolBar->addAction(m_stopAction);
  m_actionToolBar->addAction(m_switchShowCloseAction);
  m_actionToolBar->addAction(m_timer);
  if (m_rs485Action)
    m_actionToolBar->addAction(m_rs485Action);
}

void SchematicsWindow::CreateContextMenu() {
  m_view->addAction(m_cutAction);
  m_view->addAction(m_copyAction);
  m_view->addAction(m_pasteAction);
  m_view->addAction(m_deleteAction);
  m_view->addAction(m_propertiesAction);
  m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void SchematicsWindow::CreateScem(SchemeReader &reader) {
  QMap<size_t, AbstractBlock *> abstructBlocks;
  QMap<size_t, BlockStruct> blockStruct = reader.Blocks();
  DatabaseBlocks *databaseBlocks = DatabaseBlocks::Instance();

  foreach (BlockStruct blockStruct, blockStruct) {
    AbstractBlock *newblock = databaseBlocks->GetBlock(blockStruct.type);
    if (newblock) {
      abstructBlocks.insert(blockStruct.id, newblock);
      m_scene->AddBlock(newblock,
                        QPoint(blockStruct.coordX, blockStruct.coordY));
      newblock->SetSetting(blockStruct.listSetting);
    }
  }

  QList<LinkStruct> connects = reader.Connects();
  foreach (LinkStruct linkStruct, connects) {
    // проверка на существования соединяемых блоков
    size_t idOutBlock = linkStruct.idOutBlock;
    if (!abstructBlocks.contains(idOutBlock))
      continue;

    size_t idInBlock = linkStruct.idInBlock;
    if (!abstructBlocks.contains(idInBlock))
      continue;

    // получение соединяемых блоков
    AbstractBlock *inBlock = abstructBlocks[idInBlock];
    AbstractBlock *outBlock = abstructBlocks[idOutBlock];

    // получение векторов портов
    QVector<IPortBlock *> vectorInPorts = inBlock->GetInPort();
    QVector<OPortBlock *> vectorOutPorts = outBlock->GetOutPort();

    int idInPort = static_cast<int>(linkStruct.idInPort) - 1;
    int idOutPort = static_cast<int>(linkStruct.idOutPort) - 1;

    if (idInPort >= vectorInPorts.size())
      continue;

    if (idOutPort >= vectorOutPorts.size())
      continue;

    OPortBlock *outPortBlock = vectorOutPorts[idOutPort];
    IPortBlock *inPortBlock = vectorInPorts[idInPort];
    LineConnect *link = new LineConnect(outPortBlock);
    link->SetPort(inPortBlock);

    inPortBlock->SetLine(link);
    outPortBlock->SetLine(link);

    link->Connect();
    link->TrackNodes();
  }
}

void SchematicsWindow::SetStyleSchemeWindow(QString path) {
  // Функция позволяет подгружать дополнительные стили для MainWindow
  // Файл должен распологаться в корнейвой папке StyleSheet/
  // Если файл не обнаружен, то выбирается файл по умолчанию
  QFile file(path); // чтения файла CSS
  if (!file.exists()) {
    file.setFileName(":/StyleSheet/style.qss");
  }

  file.open(QFile::ReadOnly);
  QString mainWindowStyleSheet = QLatin1String(file.readAll());
  qApp->setStyleSheet(mainWindowStyleSheet);
  m_itemsDialogWidget->setStyleSheet(mainWindowStyleSheet);
  file.close();
}

AbstractBlock *SchematicsWindow::SelectedBlock() const {
  QList<QGraphicsItem *> items = m_scene->selectedItems();
  if (items.count() == 1)
    return dynamic_cast<AbstractBlock *>(items.first());
  else
    return nullptr;
}

bool SchematicsWindow::IsStart() const { return m_isStart; }

void SchematicsWindow::SetIsStart(bool isStart) { m_isStart = isStart; }

// ItemsDialogWidget::ItemsDialogWidget(QWidget *parent):QWidget(parent),
// m_seqNumberItem(0),
//     m_itemSize(200, 100), m_marginRectSize(50, 30), m_quantityToLineItem(2)
//{
//     m_scene = new QGraphicsScene(this);
//     m_view = new QGraphicsView();
//     m_view->setStyleSheet("border: 2px groove gray; border-radius: 10px;
//     padding: 6px 6px;"); m_view->setScene(m_scene);
//     // установка сглаживания отрисовки
//     m_view->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing);
//     // установка привязки при увеличении размеров окна
//     m_view->setAlignment(Qt::AlignLeft|Qt::AlignTop);
//     CreateSchematicsBlock();
//     QVBoxLayout* layout = new QVBoxLayout;
//     layout->addWidget(m_view);
//     setLayout(layout);
//     setMinimumWidth(2 * m_marginRectSize.width() + m_itemSize.width());
// }

// ItemsDialogWidget::~ItemsDialogWidget(){}

// void ItemsDialogWidget::show()
//{
//     if(isHidden())
//         QWidget::show();
//     else {
//         raise();
//         activateWindow();
//     }
// }

// void ItemsDialogWidget::resizeEvent(QResizeEvent *event)
//{

//    if(event->size().width() <= 2 * m_marginRectSize.width() + m_step)
//        return;

//    size_t quantityToLineItem = (event->size().width() -
//                                 m_marginRectSize.width())/(m_step);

//    if(!quantityToLineItem)
//        return;

//    if(quantityToLineItem == m_quantityToLineItem)
//        return;

//    m_quantityToLineItem = quantityToLineItem;

//    ResetItem();
//    m_scene->setSceneRect(m_scene->itemsBoundingRect());

//}

// void ItemsDialogWidget::CreateSinkBlocks()
//{
//     SetupTextItem("Приемники");
//     QList<AbstractBlock *> blocks = DatabaseBlocks::Instance()->SinkBlocks();
//     for (auto* blocks : blocks)
//     {
//         SetupItem(blocks->Clone());
//     }
// }

// void ItemsDialogWidget::CreateIOBlocks()
//{
//     SetupTextItem("Модули");
//     QList<AbstractBlock *> blocks = DatabaseBlocks::Instance()->IOBlocks();
//     for (auto* blocks : blocks)
//     {
//         SetupItem(blocks->Clone());
//     }
// }

// void ItemsDialogWidget::CreateSourceBlocks()
//{
//     SetupTextItem("Источники");
//     QList<AbstractBlock *> blocks =
//     DatabaseBlocks::Instance()->SourceBlocks(); for (auto* blocks : blocks)
//     {
//         SetupItem(blocks->Clone());
//     }
// }

// void ItemsDialogWidget::CreateSchematicsBlock()
//{
//     // источники
//     CreateSourceBlocks();
//     // модули
//     CreateIOBlocks();
//     // приемники
//     CreateSinkBlocks();
// }

// void ItemsDialogWidget::SetupItem(AbstractBlock* block)
//{
//     block->setPos(QPoint(m_marginRectSize.width() + (m_step *
//     (m_seqNumberItem % m_quantityToLineItem)),
//                          m_marginRectSize.height() + (m_step *
//                          (m_seqNumberItem / m_quantityToLineItem)))
//                   );
//     m_scene->addItem(block);
//     m_seqNumberItem++;
// }

// void ItemsDialogWidget::SetupTextItem(const QString& text)
//{
//     // расчет количества элементов в строке
//     while (m_seqNumberItem % m_quantityToLineItem)
//         m_seqNumberItem ++;

//    TextItem* textItem = new TextItem(text,
//    QRectF(QPointF(m_marginRectSize.width(),
//                                                           m_marginRectSize.height()
//                                                           + (m_step *
//                                                           (m_seqNumberItem /
//                                                           m_quantityToLineItem))),
//                                                   m_itemSize));
//    textItem->setSelected(true);
//    m_scene->addItem(textItem);
//    m_seqNumberItem += m_quantityToLineItem;
//}

// void ItemsDialogWidget::ResetItem()
//{
//     QPainterPath path;
//     path.addRect(m_scene->itemsBoundingRect());
//     m_scene->setSelectionArea(path);
//     QList<QGraphicsItem*> itemsDelete;
//     // поиск для удаления и отброска мусора
//     foreach (QGraphicsItem* item, m_scene->selectedItems())
//         itemsDelete.append(item);

//    qDeleteAll(itemsDelete);
//    m_seqNumberItem = 0;
//    CreateSchematicsBlock();
//}
