#include "schemticsscene.h"
#include <QGraphicsView>
#include <QTimer>

QVector<BlockStruct> SchemticsController::m_blockStructs =
    QVector<BlockStruct>();
QList<LinkStruct> SchemticsController::m_linkStructs = QList<LinkStruct>();

SchemticsController::SchemticsController(QObject *parent)
    : QGraphicsScene(parent), m_time(1), m_dilogTimer(new DilogTimer) {
  connect(m_dilogTimer, SIGNAL(ChangeTimer(int)), this, SLOT(SetTimer(int)));
  connect(this, SIGNAL(sceneRectChanged(const QRectF &)),
          SLOT(update(const QRectF &)));
}

SchemticsController::~SchemticsController() {
  if (m_dilogTimer->isVisible())
    m_dilogTimer->close();

  SelecteAll();
  DelBlock();
  delete m_dilogTimer;
  // удаляем менеджер
  delete m_operationManager;
}

void SchemticsController::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
  if (dynamic_cast<const BlockMimeData *>(event->mimeData()))
    event->setAccepted(true);
  else
    event->setAccepted(false);
}

void SchemticsController::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  dragEnterEvent(event);
}

void SchemticsController::AddBlock(AbstractBlock *block, const QPointF &pos) {
  block->setFlag(QGraphicsItem::ItemIsMovable);
  block->setPos(pos);
  block->setAcceptDrops(false);
  clearSelection();
  block->setSelected(true);
  addItem(block);

  // проверка на иницилизацию
  if (block->GetModule())
    return;

  block->InitBlock(qobject_cast<QWidget *>(parent()));
  block->SetId(m_id++);

  if (m_operationManager)
    block->RegistrOperationManager(m_operationManager);

  m_timer.AttachEvent(block->GetModule());
}

void SchemticsController::SetTimer(int time) {
  if (time < 0 && time == m_time)
    return;

  m_time = time;
  m_timer.Start(m_time);
}

void SchemticsController::SetOperationManager(
    AbstractOperationManager *manager) {
  m_operationManager = manager;
  if (m_operationManager)
    m_operationManager->AttachEvent(this);
}

bool SchemticsController::IsStart() const { return m_isStart; }

bool SchemticsController::HasSelection() const {
  return !selectedItems().isEmpty();
}

bool SchemticsController::IsSelecteBlock() const { return SelectedBlock(); }

bool SchemticsController::IsSelecteBlocks() const {
  return SelectedBlocks().count();
}

bool SchemticsController::IsPaste() const { return !m_blockStructs.isEmpty(); }

bool SchemticsController::IsEmpty() const { return items().empty(); }

void SchemticsController::Pause() {}

void SchemticsController::DelBlock() { qDeleteAll(SelectedBlocks()); }

void SchemticsController::CutBlock() {
  CopyBlock();
  DelBlock();
}

void SchemticsController::CopyBlock() {
  m_blockStructs.clear();
  m_linkStructs.clear();

  for (auto *abstractBlock : SelectedBlocks()) {
    m_blockStructs.push_back(abstractBlock->GetBlockStruct());
    m_linkStructs.append(abstractBlock->GetLinkStruct());
  }
}

void SchemticsController::PasteBlock() {
  QMap<size_t, AbstractBlock *> abstructBlocks;

  for (auto &blockStruct : m_blockStructs) {
    // выполняем смещение относительно основной схемы
    blockStruct.coordX += 10;
    blockStruct.coordY += 10;

    AbstractBlock *abstrbl = AddBlock(blockStruct);
    if (abstrbl)
      abstructBlocks.insert(blockStruct.id, abstrbl);
  }

  CreateLink(m_linkStructs, abstructBlocks);

  //выделяем вставленные блоки
  for (auto *it : abstructBlocks)
    it->setSelected(true);
}

void SchemticsController::PropertiesBlock() {
  if (IsSelecteBlock()) {
    AbstractBlock *block = SelectedBlock();
    block->ShowDialog();
  }
}

void SchemticsController::SelecteAll() {
  QPainterPath path;
  path.addRect(itemsBoundingRect());
  setSelectionArea(path);
}

void SchemticsController::UnSelecteAll() { clearSelection(); }

void SchemticsController::VerticalAligning() {
  QList<AbstractBlock *> list = SelectedBlocks();
  if (list.empty())
    return;

  double yMin = list.first()->y();
  double xPos = list.first()->x();
  for (AbstractBlock *block : list) {
    if (yMin > block->y()) {
      yMin = block->y();
      xPos = block->x();
    }
  }

  for (AbstractBlock *abstractBlock : list)
    abstractBlock->setX(xPos);
}

void SchemticsController::HorizontalAligning() {
  QList<AbstractBlock *> list = SelectedBlocks();
  if (list.isEmpty())
    return;

  double yPos = list.first()->y();
  double xMin = list.first()->x();

  for (AbstractBlock *block : list) {
    if (xMin > block->x()) {
      yPos = block->y();
      xMin = block->x();
    }
  }

  for (AbstractBlock *abstractBlock : list)
    abstractBlock->setY(yPos);
}

bool SchemticsController::RenameScheme(const QString &) { return true; }

void SchemticsController::SaveScheme(const QString &path) {
  // выделяем все элементы;
  SelecteAll();
  // выполняем поиск блоков на сцене
  SchemeWriter save(path); // добавить путь для записи

  for (AbstractBlock *abstractBlock : SelectedBlocks()) {
    save.SaveBlock(abstractBlock->GetBlockStruct());
    QList<LinkStruct> linkStructs = abstractBlock->GetLinkStruct();

    for (auto &linkStruct : linkStructs)
      save.SaveLink(linkStruct);
  }
}

void SchemticsController::OpenScheme(const QString &path) {
  SchemeReader reader;

  if (reader.ReadFile(path)) {
    SelecteAll();
    DelBlock();
    CreateScem(reader);
  }
}

void SchemticsController::dropEvent(QGraphicsSceneDragDropEvent *event) {
  const BlockMimeData *mime =
      dynamic_cast<const BlockMimeData *>(event->mimeData());

  if (!mime)
    return;

  if (mime->HasAbstractBlock()) {
    AbstractBlock *block = mime->Block()->Clone();
    AddBlock(block, event->scenePos());
  }
}

void SchemticsController::Start() {
  if (m_operationManager)
    m_operationManager->StartManager();

  m_timer.Start(static_cast<uint32_t>(m_time));
  BlocksIsMovable(false);
}

void SchemticsController::Stop() {
  m_timer.Stop();
  BlocksIsMovable(true);
  if (m_isStart) {
    m_isStart = false;
    if (m_operationManager)
      m_operationManager->StopManager();
  }
}

void SchemticsController::ShowSettingTimer() {
  if (m_dilogTimer->isHidden())
    m_dilogTimer->show();
  else {
    m_dilogTimer->raise();
    m_dilogTimer->activateWindow();
    m_dilogTimer->showNormal();
  }
}

DilogTimer::DilogTimer(QWidget *parent) : QDialog(parent) {
  m_spinBox = new QSpinBox;
  m_label = new QLabel("Таймер, мсек:");
  m_spinBox->setValue(50);
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(m_label);
  layout->addWidget(m_spinBox);
  setLayout(layout);
  m_spinBox->setRange(1, 5000);
  connect(m_spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(ChangeTimer(int)));
}

DilogTimer::~DilogTimer() {}

TimerRun::TimerRun() {
  m_timer.AddCallback([&]() { Timout(); });
}

TimerRun::~TimerRun() { m_timer.Stop(); }

void TimerRun::Start(uint32_t msec) { m_timer.Start(msec); }

void TimerRun::Stop() { m_timer.Stop(); }

void SchemticsController::BlocksIsMovable(bool isMovable) {
  for (QGraphicsItem *block : items())
    block->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
}

void SchemticsController::StartEvent() {
  if (!m_isStart) {
    m_isStart = true;
    emit StartOperation();
  }
}

void SchemticsController::StopEvent() {
  if (m_isStart) {
    m_isStart = false;
    emit StopOperation();
  }
}

AbstractBlock *SchemticsController::SelectedBlock() const {
  QList<QGraphicsItem *> items = selectedItems();
  if (items.count() == 1)
    return dynamic_cast<AbstractBlock *>(items.first());
  else
    return nullptr;
}

QList<AbstractBlock *> SchemticsController::SelectedBlocks() const {
  QList<AbstractBlock *> selectedBlocks;
  for (auto *item : selectedItems()) {
    AbstractBlock *abstractBlock = dynamic_cast<AbstractBlock *>(item);
    if (abstractBlock)
      selectedBlocks.append(abstractBlock);
  }
  return selectedBlocks;
}

AbstractBlock *SchemticsController::AddBlock(const BlockStruct &blockStruct) {
  DatabaseBlocks *databaseBlocks = DatabaseBlocks::Instance();
  AbstractBlock *newblock = databaseBlocks->GetBlock(blockStruct.type);
  if (newblock) {
    AddBlock(newblock, QPoint(blockStruct.coordX, blockStruct.coordY));
    newblock->SetSetting(blockStruct.listSetting);
    return newblock;
  }
  return nullptr;
}

void SchemticsController::CreateLink(
    const QList<LinkStruct> &connects,
    const QMap<size_t, AbstractBlock *> &abstructBlocks) {
  for (auto &linkStruct : connects) {
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

    if (link->Connect())
      link->TrackNodes();
  }
}

void SchemticsController::CreateScem(SchemeReader &reader) {
  QMap<size_t, AbstractBlock *> abstructBlocks;

  for (auto &blockStruct : reader.Blocks()) {
    AbstractBlock *abstrbl = AddBlock(blockStruct);
    if (abstrbl)
      abstructBlocks.insert(blockStruct.id, abstrbl);
  }

  CreateLink(reader.Connects(), abstructBlocks);
}
