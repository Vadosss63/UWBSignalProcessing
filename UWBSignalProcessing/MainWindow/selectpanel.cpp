#include "selectpanel.h"

int ItemsDialogWidget::m_margin = 0;

ItemsDialogWidget::ItemsDialogWidget(QWidget *parent)
    : QWidget(parent), m_seqNumberItem(0), m_quantityToLineItem(2) {
  m_scene = new QGraphicsScene(this);
  m_view = new QGraphicsView();
  m_view->setStyleSheet(styleSheet() +
                        "border-radius: 10px; padding: 6px 6px;");
  m_view->setScene(m_scene);
  // установка сглаживания отрисовки
  m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  // установка привязки при увеличении размеров окна
  m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
  LoudBlocks();
  ResetItem();
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(m_view);
  setLayout(layout);
}

ItemsDialogWidget::~ItemsDialogWidget() {}

void ItemsDialogWidget::SetStyleSheet(QString style) {
  QRegExp rx;
  QString color;

  // получаем QGraphicsTextItem
  rx.setPattern(
      "QGraphicsTextItem.*\\{.*color:\\s*(#[A-fa-f0-9]{6}){1}\\s*;.*\\}");
  if (rx.indexIn(style) != -1) {
    color = rx.cap(1);
    m_titleSorce->setDefaultTextColor(color);
    m_titleIO->setDefaultTextColor(color);
    m_titleSink->setDefaultTextColor(color);
  }
}

void ItemsDialogWidget::ReloudBlocks() {
  m_databaseBlocks->ReloudBlock();
  LoudBlocks();
  ResetItem();
}

void ItemsDialogWidget::show() {
  LoudBlocks();
  ResetItem();
  if (isHidden())
    QWidget::show();
  else {
    raise();
    activateWindow();
  }
}

void ItemsDialogWidget::resizeEvent(QResizeEvent *) {
  qreal widthwin = m_view->width();

  int quantityToLineItem =
      static_cast<int>((widthwin - m_margin) / (m_rectSize.width() - m_margin));

  if (quantityToLineItem > 1 && quantityToLineItem != m_quantityToLineItem) {
    m_quantityToLineItem = quantityToLineItem;
    ResetItem();
  }
}

void ItemsDialogWidget::CreateSinkBlocks() {
  SetupTextItem(m_titleSink);
  SetupItems(m_databaseBlocks->SinkBlocks());
}

void ItemsDialogWidget::CreateIOBlocks() {
  SetupTextItem(m_titleIO);
  SetupItems(m_databaseBlocks->IOBlocks());
}

void ItemsDialogWidget::CreateSourceBlocks() {
  SetupTextItem(m_titleSorce);
  SetupItems(m_databaseBlocks->SourceBlocks());
}

void ItemsDialogWidget::CreateSchematicsBlock() {
  // источники
  CreateSourceBlocks();
  // модули
  CreateIOBlocks();
  // приемники
  CreateSinkBlocks();
}

void ItemsDialogWidget::InitSize(AbstractBlock *block) {
  m_rectSize = block->boundingRect().size();
  //    m_margin = block->Padding();
  m_margin = 30;
  // смещение с учетом скролбара
  const qreal shift = 2;
  setMinimumWidth(static_cast<int>(2 * m_rectSize.width() + m_margin + shift));
  setMinimumHeight(static_cast<int>(2 * m_rectSize.width()));
}

qreal ItemsDialogWidget::GetXPos() const {
  qreal x = m_margin + (m_rectSize.width() - m_margin) *
                           (m_seqNumberItem % (m_quantityToLineItem));

  return x;
}

qreal ItemsDialogWidget::GetYPos() const {
  qreal y = m_margin +
            (m_rectSize.height()) * (m_seqNumberItem / (m_quantityToLineItem));
  return y;
}

void ItemsDialogWidget::SetupItem(AbstractBlock *block) {
  qreal x = GetXPos();
  qreal y = GetYPos();
  block->setPos(x, y);
  if (!block->scene())
    m_scene->addItem(block);
  m_seqNumberItem++;
}

void ItemsDialogWidget::SetupItems(QList<AbstractBlock *> blocks) {
  foreach (auto block, blocks)
    SetupItem(block);
}

void ItemsDialogWidget::SetupTextItem(QGraphicsTextItem *textitem) {
  // расчет количества элементов в строке
  while (m_seqNumberItem % m_quantityToLineItem)
    m_seqNumberItem++;

  qreal y = GetYPos();
  textitem->setY(y);

  QFont f = font();

  f.setPixelSize(m_margin);

  textitem->setFont(f);
  m_seqNumberItem += m_quantityToLineItem;
}

void ItemsDialogWidget::SetupTitlePos(QGraphicsTextItem *item) const {
  qreal x = m_scene->itemsBoundingRect().center().x();
  qreal dx = item->boundingRect().width() / 2.0;
  qreal dy = (m_rectSize.height() / 2.0) - item->boundingRect().height() / 2.0;
  item->setPos(x - dx, item->y() + dy);
}

void ItemsDialogWidget::ResetItem() {
  m_seqNumberItem = 0;

  CreateSchematicsBlock();
  SetupTitlePos(m_titleSorce);
  SetupTitlePos(m_titleIO);
  SetupTitlePos(m_titleSink);

  m_scene->setSceneRect(m_scene->itemsBoundingRect());
}

void ItemsDialogWidget::SetupSourceBlocks() {
  m_titleSorce = m_scene->addText("Источники");
}

void ItemsDialogWidget::SetupIOBlocks() {
  m_titleIO = m_scene->addText("Модули");
}

void ItemsDialogWidget::SetupSinkBlocks() {
  m_titleSink = m_scene->addText("Приемники");
}

void ItemsDialogWidget::LoudBlocks() {
  m_scene->clear();

  m_databaseBlocks = DatabaseBlocks::Instance();

  SetupSourceBlocks();

  SetupIOBlocks();

  SetupSinkBlocks();

  if (!m_databaseBlocks->SourceBlocks().empty()) {
    InitSize(m_databaseBlocks->SourceBlocks().first());
  } else if (!m_databaseBlocks->IOBlocks().empty()) {
    InitSize(m_databaseBlocks->IOBlocks().first());
  } else if (!m_databaseBlocks->SinkBlocks().empty()) {
    InitSize(m_databaseBlocks->SinkBlocks().first());
  }
}
