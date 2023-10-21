#include "schemaview.h"
#include <QMenu>

SchemaView::SchemaView(const QString &name) { CreateView(name); }

SchemaView::~SchemaView() {
  delete m_schemaController;
  m_schemaController = nullptr;
}

void SchemaView::CreateView(const QString &name) {
  m_viewOfSchema = new QGraphicsView();
  m_viewOfSchema->setObjectName(name);
  m_viewOfSchema->setAcceptDrops(true);
  m_viewOfSchema->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_schemaController = new SchemticsController(m_viewOfSchema);
  m_viewOfSchema->setScene(m_schemaController);
  m_viewOfSchema->setDragMode(QGraphicsView::RubberBandDrag);
  m_viewOfSchema->setRenderHints(QPainter::Antialiasing |
                                 QPainter::TextAntialiasing);
}

const SchemticsController *SchemaView::GetSchemaController() const {
  return m_schemaController;
}

void SchemaView::SetOperationManager(AbstractOperationManager *manager) {
  m_schemaController->SetOperationManager(manager);
}

void SchemaView::CreateContextMenu(QList<QAction *> itemsMenu) {
  foreach (QAction *item, itemsMenu) {
    m_viewOfSchema->addAction(item);
  }
  m_viewOfSchema->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void SchemaView::ZoomInScheme() { m_viewOfSchema->scale(1.5, 1.5); }

void SchemaView::ZoomOriginalScheme() { m_viewOfSchema->resetTransform(); }

void SchemaView::ZoomOutScheme() { m_viewOfSchema->scale(0.75, 0.75); }

void SchemaView::OpenScheme(const QString &path) {
  m_schemaController->OpenScheme(path);
}

void SchemaView::SaveScheme(const QString &path) {
  m_schemaController->SaveScheme(path);
}

void SchemaView::DelBlock() { m_schemaController->DelBlock(); }

void SchemaView::CutBlock() { m_schemaController->CutBlock(); }

void SchemaView::CopyBlock() { m_schemaController->CopyBlock(); }

void SchemaView::PasteBlock() { m_schemaController->PasteBlock(); }

void SchemaView::PropertiesBlock() { m_schemaController->PropertiesBlock(); }

void SchemaView::SelecteAll() { m_schemaController->SelecteAll(); }

void SchemaView::VerticalAligning() { m_schemaController->VerticalAligning(); }

void SchemaView::HorizontalAligning() {
  m_schemaController->HorizontalAligning();
}

void SchemaView::Start() { m_schemaController->Start(); }

void SchemaView::Pause() { m_schemaController->Pause(); }

void SchemaView::Stop() { m_schemaController->Stop(); }

void SchemaView::ShowTimer() { m_schemaController->ShowSettingTimer(); }

bool SchemaView::RenameScheme(const QString &nameSchema) {
  return m_schemaController->RenameScheme(nameSchema);
}

bool SchemaView::IsStart() const { return m_schemaController->IsStart(); }

bool SchemaView::HasSelection() const {
  return m_schemaController->HasSelection();
}

bool SchemaView::IsSelecteBlock() const {
  return m_schemaController->IsSelecteBlock();
}

bool SchemaView::IsSelecteBlocks() const {
  return m_schemaController->IsSelecteBlocks();
}

bool SchemaView::IsPaste() const { return m_schemaController->IsPaste(); }

bool SchemaView::IsEmpty() const { return m_schemaController->IsEmpty(); }

QGraphicsView *SchemaView::GetViewOfSchema() const { return m_viewOfSchema; }
