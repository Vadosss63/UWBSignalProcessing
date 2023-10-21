#include "schemewindow.h"

void SchemeWindow::DEBAG() {
  SchemaView *schemaView = GetCurrentSchema();
  QString path("/home/vadosss63/run.uwbs");
  schemaView->OpenScheme(path);
  RenameScheme(QDir(path).dirName());
  StartShemaScript();
}

SchemeWindow::SchemeWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Схема");
  setWindowState(Qt::WindowMaximized);

  CreateActionsMenu();
  CreateMenu();
  CreateEditToolBars();
  CreateActionBars();
  CreateStatusBar();
  CreateSelectPanel();

  CreateCentralWidget();
  ReadSettingsWindow();
  SetStyleSchemeWindow();
  DisableActionMenu();
}

SchemeWindow::~SchemeWindow() {
  WriteSettingsWindow();
  qDeleteAll(m_scematics);
  delete m_operationmanager;
}

void SchemeWindow::SetOperationManager(AbstractOperationManager *manager) {
  m_operationmanager = manager;
}

SchemaView *SchemeWindow::GetCurrentSchema() {
  if (!m_scematics.size())
    return nullptr;

  int currentSchema = m_switchCurrentSchema->currentIndex();
  SchemaView *schemaView = m_scematics.at(currentSchema);

  return schemaView;
}

void SchemeWindow::StartShemaScript() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->Start();
  UpdateActionsMenu();
  m_itemDockWidget->setHidden(true);
}

void SchemeWindow::PauseShemaScript() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->Pause();
}

void SchemeWindow::StopShemaScript() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->Stop();
  m_itemDockWidget->setHidden(false);
  UpdateActionsMenu();
}

void SchemeWindow::ShowTimer() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->ShowTimer();
}

void SchemeWindow::LockActionsMenu() {
  m_selectionPanelElementsButton->setEnabled(false);
  m_startShemaScriptButton->setEnabled(false);
  m_openFileSchemeButton->setEnabled(false);
  m_stopShemaScriptButton->setEnabled(true);
  m_statusWorkShema->setDisabled(true);

  m_zoomInSchemaButton->setDisabled(true);
  m_zoomOriginalSchemaButton->setDisabled(true);
  m_zoomOutSchemaButton->setDisabled(true);

  m_removeSchemaButton->setDisabled(true);
  m_addSchemeButton->setDisabled(true);
  SetEnabledTab(true);
}

void SchemeWindow::UnlockActionsMenu() {
  if (m_scematics.size() > 0) {
    m_selectionPanelElementsButton->setEnabled(true);
    m_openFileSchemeButton->setEnabled(true);
    m_stopShemaScriptButton->setEnabled(false);
    m_startShemaScriptButton->setEnabled(true);
    m_statusWorkShema->setDisabled(false);

    m_zoomInSchemaButton->setEnabled(true);
    m_zoomOriginalSchemaButton->setEnabled(true);
    m_zoomOutSchemaButton->setEnabled(true);

    m_removeSchemaButton->setDisabled(false);
    m_addSchemeButton->setDisabled(false);
    SetEnabledTab(false);
  }
}

void SchemeWindow::ShowSelectPanel() {
  // получаем текущее состояние и инвертируем его
  bool currentState = m_itemDockWidget->isHidden();
  m_itemDockWidget->setHidden(!currentState);
}

void SchemeWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_A:
    if (event->modifiers() & Qt::ControlModifier)
      SelecteAll();
    break;

  case Qt::Key_F5:
    StartShemaScript();
    break;
  case Qt::Key_F4:
    StopShemaScript();
    break;
  default:
    break;
  }
  QWidget::keyPressEvent(event);
}

void SchemeWindow::OpenScheme() {
  QString path =
      QFileDialog::getOpenFileName(this, "Открытие файла", "", "uwbs(*.uwbs)");

  if (path.isEmpty())
    return;

  SchemaView *schemaView = GetCurrentSchema();
  schemaView->OpenScheme(path);
  RenameScheme(QDir(path).dirName());
}

void SchemeWindow::SaveScheme() {

  QString pathName =
      m_switchCurrentSchema->tabText(m_switchCurrentSchema->currentIndex());

  QString path = QFileDialog::getSaveFileName(this, "Сохранение файла",
                                              pathName, "uwbs(*.uwbs)");
  path = path.trimmed();
  if (!path.endsWith(".uwbs")) {
    path += QString(".uwbs");
  }

  SchemaView *schemaView = GetCurrentSchema();
  schemaView->SaveScheme(path);
}

void SchemeWindow::AddScheme() {
  LockActionsMenu();
  m_stopShemaScriptButton->setEnabled(false);

  NameSchemeDialog renameSchemeDialog("Задайте название схемы");
  renameSchemeDialog.setStyleSheet(styleSheet());
  renameSchemeDialog.SetTextLine("");
  if (renameSchemeDialog.exec())
    AddViewOfSchema(renameSchemeDialog.GetTextLine());
  else
    UnlockActionsMenu();
}

void SchemeWindow::RemoveScheme() {
  int index = m_switchCurrentSchema->currentIndex();
  int sizeSchemes = m_scematics.size();
  if (index <
      sizeSchemes) // для проверки, что это не кнопка добавления новой схемы
    RemoveSchemeScene(index);
}

void SchemeWindow::RemoveSchemeScene(int index) {
  SchemaView *schemaView = m_scematics.at(index);

  if (index < m_switchCurrentSchema->count())
    m_switchCurrentSchema->removeTab(index);

  delete schemaView;
  m_scematics.removeAt(index);

  if (m_scematics.size() == 0) {
    LockActionsMenu();
    m_stopShemaScriptButton->setEnabled(false);
    m_statusWorkShema->setDisabled(false);
    m_zoomOutSchemaButton->setDisabled(true);
    m_zoomInSchemaButton->setDisabled(true);
    m_synchronizationTimerBlocksButton->setDisabled(true);
    m_addSchemeButton->setDisabled(false);
  }
  int currInd = m_switchCurrentSchema
                    ->currentIndex(); //для установки соседней схемы, но не
                                      //затрагивая кнопку добавления сигнала.
  if (currInd == m_switchCurrentSchema->count() - 1) {
    if (currInd > 0)
      m_switchCurrentSchema->setCurrentIndex(currInd - 1);
  }
}

void SchemeWindow::DelBlock() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->DelBlock();
}

void SchemeWindow::CutBlock() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->CopyBlock();
  schemaView->DelBlock();
}

void SchemeWindow::CopyBlock() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->CopyBlock();
  UpdateActionsMenu();
}

void SchemeWindow::PasteBlock() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->PasteBlock();
}

void SchemeWindow::UndoBlock() {
  //    SchemaView* schemaView = GetCurrentSchema();
  //    schemaView->PasteBlock();
}

void SchemeWindow::RedoBlock() {
  //    SchemaView* schemaView = GetCurrentSchema();
  //    schemaView->PasteBlock();
}

void SchemeWindow::PropertiesBlock() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->PropertiesBlock();
}

void SchemeWindow::SelecteAll() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->SelecteAll();
}

void SchemeWindow::ZoomInScheme() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->ZoomInScheme();
}

void SchemeWindow::ZoomOriginalScheme() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->ZoomOriginalScheme();
}

void SchemeWindow::ZoomOutScheme() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->ZoomOutScheme();
}

void SchemeWindow::UpdateActionsMenu() {
  SchemaView *currentSchemaView = GetCurrentSchema();
  if (!currentSchemaView) {
    DisableActionMenu();
    return;
  }

  if (currentSchemaView->IsStart()) {
    LockActionsMenu();
    m_propertiesBlockButton->setEnabled(currentSchemaView->IsSelecteBlock());
    return;
  } else {
    UnlockActionsMenu();
  }

  m_propertiesBlockButton->setEnabled(currentSchemaView->IsSelecteBlock());
  m_deleteBlockButton->setEnabled(currentSchemaView->HasSelection());
  m_cutBlockButton->setEnabled(currentSchemaView->IsSelecteBlocks());
  m_copyBlockButton->setEnabled(currentSchemaView->IsSelecteBlocks());

  bool isPaste = false;
  for (auto &scema : m_scematics)
    isPaste = isPaste || scema->IsPaste();

  m_pasteBlockButton->setEnabled(isPaste);

  m_saveFileSchemaButton->setEnabled(!(currentSchemaView->IsEmpty()));
}

void SchemeWindow::DisableActionMenu() {
  m_deleteBlockButton->setEnabled(false);
  m_cutBlockButton->setEnabled(false);
  m_copyBlockButton->setEnabled(false);
  m_propertiesBlockButton->setEnabled(false);
  m_pasteBlockButton->setEnabled(false);
  m_saveFileSchemaButton->setEnabled(false);
}

void SchemeWindow::OpenCSS() {
  QString path =
      QFileDialog::getOpenFileName(this, "Файл стиля", "", "qss(*.qss)");
  if (!path.isEmpty())
    SetStyleSchemeWindow(path);
}

void SchemeWindow::AddViewOfSchema(const QString &nameSchema) {
  QString name = nameSchema;

  if (name.isEmpty())
    name = "Новая схема" + QString::number(m_scematics.size());

  SchemaView *schemaView = new SchemaView(name);
  schemaView->SetOperationManager(m_operationmanager->Clone());
  connect(schemaView->GetSchemaController(), SIGNAL(selectionChanged()), this,
          SLOT(UpdateActionsMenu()), Qt::QueuedConnection);

  connect(schemaView->GetSchemaController(), SIGNAL(StopOperation()), this,
          SLOT(StopShemaScript()), Qt::QueuedConnection);

  connect(schemaView->GetSchemaController(), SIGNAL(StartOperation()), this,
          SLOT(UpdateActionsMenu()), Qt::QueuedConnection);

  m_scematics.prepend(schemaView);
  m_switchCurrentSchema->insertTab(0, schemaView->GetViewOfSchema(), name);
  m_switchCurrentSchema->setCurrentIndex(0);
  CreateContextMenu(schemaView);
  if (m_scematics.size() == 1) {
    UnlockActionsMenu();
    m_statusWorkShema->setDisabled(true);
    m_zoomOutSchemaButton->setDisabled(false);
    m_zoomInSchemaButton->setDisabled(false);
    m_synchronizationTimerBlocksButton->setDisabled(false);
  }
}

void SchemeWindow::AddSchemeFromTab(int index) {
  int i = m_switchCurrentSchema->count() - 1;
  if (index == i)
    AddScheme();
}

void SchemeWindow::RenameTabDialog(int index) {
  int i = m_switchCurrentSchema->count() - 1;
  if ((index != i) && m_selectionPanelElementsButton->isEnabled()) {
    NameSchemeDialog renameSchemeDialog;
    renameSchemeDialog.setStyleSheet(styleSheet());
    renameSchemeDialog.SetTextLine(
        m_switchCurrentSchema->tabText(m_switchCurrentSchema->currentIndex()));
    if (renameSchemeDialog.exec())
      RenameScheme(renameSchemeDialog.GetTextLine());
  }
}

void SchemeWindow::RenameScheme(const QString &nameSchema) {
  if (nameSchema.isEmpty())
    return;
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->RenameScheme(nameSchema);
  m_switchCurrentSchema->setTabText(m_switchCurrentSchema->currentIndex(),
                                    nameSchema);
  GetCurrentSchema()->GetViewOfSchema()->setObjectName(nameSchema);
}

void SchemeWindow::VerticalAligning() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->VerticalAligning();
}

void SchemeWindow::HorizontalAligning() {
  SchemaView *schemaView = GetCurrentSchema();
  schemaView->HorizontalAligning();
}

void SchemeWindow::ReloudBlocks() { m_itemsDialogWidget->ReloudBlocks(); }

void SchemeWindow::WriteSettingsWindow() {
  QSettings settings("Radar MMS", "UWBSignalProcessing");
  settings.beginGroup("SchematicsWindow");
  settings.setValue("geometry", saveGeometry());
  settings.setValue("state", saveState());
  settings.endGroup();
}

void SchemeWindow::ReadSettingsWindow() {
  QSettings settings("Radar MMS", "UWBSignalProcessing");
  settings.beginGroup("SchematicsWindow");
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("state").toByteArray());
  settings.endGroup();
}

void SchemeWindow::CreateActionsMenu() {
  m_openFileSchemeButton =
      new QAction(QIcon(":/images/document-open.svg"), "Открыть", this);
  m_openFileSchemeButton->setShortcut(QKeySequence::Open);
  m_openFileSchemeButton->setStatusTip("Открыть схему");
  connect(m_openFileSchemeButton, SIGNAL(triggered()), this,
          SLOT(OpenScheme()));

  m_saveFileSchemaButton =
      new QAction(QIcon(":/images/document-save.svg"), "Сохранить", this);
  m_saveFileSchemaButton->setShortcut(QKeySequence::Save);
  m_saveFileSchemaButton->setStatusTip("Сохранить схему");
  connect(m_saveFileSchemaButton, SIGNAL(triggered()), this,
          SLOT(SaveScheme()));

  m_addSchemeButton =
      new QAction(QIcon(":/images/window-new.svg"), "Добавить схему", this);
  m_addSchemeButton->setStatusTip("Добавляет схему в рабочую область");
  connect(m_addSchemeButton, SIGNAL(triggered()), this, SLOT(AddScheme()));

  m_removeSchemaButton =
      new QAction(QIcon(":/images/window-close.svg"), "Удалить схему", this);
  m_removeSchemaButton->setStatusTip("Удаляет схему из рабочей области");
  connect(m_removeSchemaButton, SIGNAL(triggered()), this,
          SLOT(RemoveScheme()));

  m_cutBlockButton =
      new QAction(QIcon(":/images/edit-cut.svg"), "Вырезать", this);
  m_cutBlockButton->setShortcut(QKeySequence::Cut);
  m_cutBlockButton->setStatusTip("Вырезать блок");
  connect(m_cutBlockButton, SIGNAL(triggered()), this, SLOT(CutBlock()));

  m_copyBlockButton =
      new QAction(QIcon(":/images/edit-copy.svg"), "Копировать", this);
  m_copyBlockButton->setShortcut(QKeySequence::Copy);
  m_copyBlockButton->setStatusTip("Копировать блок");
  connect(m_copyBlockButton, SIGNAL(triggered()), this, SLOT(CopyBlock()));

  m_deleteBlockButton =
      new QAction(QIcon(":/images/edit-delete.svg"), "Удалить", this);
  m_deleteBlockButton->setShortcut(QKeySequence::Delete);
  m_deleteBlockButton->setStatusTip("Удалить блок");
  connect(m_deleteBlockButton, SIGNAL(triggered()), this, SLOT(DelBlock()));

  m_pasteBlockButton =
      new QAction(QIcon(":/images/edit-paste.svg"), "Вставить", this);
  m_pasteBlockButton->setShortcut(QKeySequence::Paste);
  m_pasteBlockButton->setStatusTip("Вставить блок");
  connect(m_pasteBlockButton, SIGNAL(triggered()), this, SLOT(PasteBlock()));

  m_propertiesBlockButton =
      new QAction(QIcon(":/images/preferences-desktop.svg"), "Свойства", this);
  m_propertiesBlockButton->setStatusTip("Параметры блока");
  connect(m_propertiesBlockButton, SIGNAL(triggered()), this,
          SLOT(PropertiesBlock()));

  m_undoBlockButton =
      new QAction(QIcon(":/images/edit-undo.svg"), "Отменить", this);
  m_undoBlockButton->setShortcut(QKeySequence::Undo);
  m_undoBlockButton->setStatusTip("Отменить действие");
  connect(m_undoBlockButton, SIGNAL(triggered()), this, SLOT(UndoBlock()));

  m_redoBlockButton =
      new QAction(QIcon(":/images/edit-redo.svg"), "Повторить", this);
  m_redoBlockButton->setShortcut(QKeySequence::Redo);
  m_redoBlockButton->setStatusTip("Повторить отменненое действие");
  connect(m_redoBlockButton, SIGNAL(triggered()), this, SLOT(RedoBlock()));

  m_zoomOutSchemaButton =
      new QAction(QIcon(":/images/zoom-out.svg"), "Уменьшить Ctrl-", this);
  m_zoomOutSchemaButton->setShortcut(QKeySequence::ZoomOut);
  m_zoomOutSchemaButton->setStatusTip("Уменьшить схему");
  connect(m_zoomOutSchemaButton, SIGNAL(triggered()), this,
          SLOT(ZoomOutScheme()));

  m_zoomOriginalSchemaButton =
      new QAction(QIcon(":/images/zoom-original.svg"), "Сбросить зум", this);
  m_zoomOriginalSchemaButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
  m_zoomOriginalSchemaButton->setStatusTip("Исходный марштаб");
  connect(m_zoomOriginalSchemaButton, SIGNAL(triggered()), this,
          SLOT(ZoomOriginalScheme()));

  m_zoomInSchemaButton =
      new QAction(QIcon(":/images/zoom-in.svg"), "Увеличить Ctrl+", this);
  m_zoomInSchemaButton->setShortcut(QKeySequence::ZoomIn);
  m_zoomInSchemaButton->setStatusTip("Увеличить схему");
  connect(m_zoomInSchemaButton, SIGNAL(triggered()), this,
          SLOT(ZoomInScheme()));

  m_startShemaScriptButton =
      new QAction(QIcon(":/images/media-playback-start.svg"), "Старт", this);
  m_startShemaScriptButton->setStatusTip("Запустить схему");
  connect(m_startShemaScriptButton, SIGNAL(triggered()), this,
          SLOT(StartShemaScript()));

  m_pauseShemaScriptButton = new QAction("Пауза", this);
  m_pauseShemaScriptButton->setIcon(
      style()->standardIcon(QStyle::SP_MediaPause));

  m_stopShemaScriptButton =
      new QAction(QIcon(":/images/media-playback-stop.svg"), "Стоп", this);
  m_stopShemaScriptButton->setStatusTip("Остановить схему");
  connect(m_stopShemaScriptButton, SIGNAL(triggered()), this,
          SLOT(StopShemaScript()));

  m_synchronizationTimerBlocksButton =
      new QAction(QIcon(":/images/timer.png"), "Таймер", this);
  m_synchronizationTimerBlocksButton->setStatusTip(
      "Настройки таймера на схеме");
  connect(m_synchronizationTimerBlocksButton, SIGNAL(triggered()), this,
          SLOT(ShowTimer()));

  m_selectionPanelElementsButton =
      new QAction(QIcon(":/images/format-indent-more.svg"),
                  "Добавить элементы на схему", this);
  m_selectionPanelElementsButton->setStatusTip("Добавть блоки на схему");
  connect(m_selectionPanelElementsButton, SIGNAL(triggered()), this,
          SLOT(ShowSelectPanel()));

  m_openFileCSS = new QAction("Стиль", this);
  m_openFileCSS->setStatusTip("Выбрать файл стиля программы");
  connect(m_openFileCSS, SIGNAL(triggered()), this, SLOT(OpenCSS()));

  m_verticalAligning = new QAction(QIcon(":/images/vertical_alig.png"),
                                   "Выровнять блоки по вертикали", this);
  m_verticalAligning->setStatusTip("Выровнять блоки по вертикали");
  connect(m_verticalAligning, SIGNAL(triggered()), this,
          SLOT(VerticalAligning()));

  m_horizontalAligning = new QAction(QIcon(":/images/horizontal_alig.png"),
                                     "Выровнять блоки по горизонтали", this);
  m_horizontalAligning->setStatusTip("Выровнять блоки по горизонтали");
  connect(m_horizontalAligning, SIGNAL(triggered()), this,
          SLOT(HorizontalAligning()));

  m_reloadItemsDialogWidget = new QAction("Перегрузить блоки", this);
  connect(m_reloadItemsDialogWidget, SIGNAL(triggered()), this,
          SLOT(ReloudBlocks()));

  m_stopShemaScriptButton->setEnabled(false);
}

void SchemeWindow::CreateStatusBar() {
  m_statusWorkShema = new QPushButton;
  m_statusWorkShema->setProperty("name", "statusWorkShema");
  statusBar()->addWidget(m_statusWorkShema);
}

void SchemeWindow::CreateMenu() {
  m_fileMenu = menuBar()->addMenu("Файл");
  m_fileMenu->addAction(m_openFileSchemeButton);
  m_fileMenu->addAction(m_saveFileSchemaButton);
  m_fileMenu->addAction(m_addSchemeButton);
  m_fileMenu->addAction(m_removeSchemaButton);

  m_editMenu = menuBar()->addMenu("Правка");
  m_editMenu->addAction(m_cutBlockButton);
  m_editMenu->addAction(m_copyBlockButton);
  m_editMenu->addAction(m_pasteBlockButton);
  m_editMenu->addAction(m_deleteBlockButton);
  m_editMenu->addAction(m_propertiesBlockButton);
  m_editMenu->addSeparator();
  m_editMenu->addAction(m_undoBlockButton);
  m_editMenu->addAction(m_redoBlockButton);
  /// TODO отключить
  m_undoBlockButton->setEnabled(false);
  m_redoBlockButton->setEnabled(false);
  m_toolMenu = menuBar()->addMenu("Схема");
  m_toolMenu->addAction(m_startShemaScriptButton);
  m_toolMenu->addAction(m_stopShemaScriptButton);
  m_toolMenu->addAction(m_selectionPanelElementsButton);
  m_optionsMenu = menuBar()->addMenu("Опции");
  m_optionsMenu->addAction(m_synchronizationTimerBlocksButton);
  m_optionsMenu->addAction(m_openFileCSS);
  m_optionsMenu->addAction(m_reloadItemsDialogWidget);

  menuBar()->addSeparator();
  m_helpMenu = menuBar()->addMenu("Справка");
}

void SchemeWindow::CreateEditToolBars() {
  m_editToolBar = addToolBar("Панель инструментов");
  m_editToolBar->setObjectName("m_editToolBar");
  m_editToolBar->addAction(m_openFileSchemeButton);
  m_editToolBar->addAction(m_saveFileSchemaButton);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_cutBlockButton);
  m_editToolBar->addAction(m_copyBlockButton);
  m_editToolBar->addAction(m_pasteBlockButton);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_verticalAligning);
  m_editToolBar->addAction(m_horizontalAligning);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_deleteBlockButton);
  m_editToolBar->addAction(m_propertiesBlockButton);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_undoBlockButton);
  m_editToolBar->addAction(m_redoBlockButton);
  m_editToolBar->addSeparator();
  m_editToolBar->addAction(m_zoomOutSchemaButton);
  m_editToolBar->addAction(m_zoomOriginalSchemaButton);
  m_editToolBar->addAction(m_zoomInSchemaButton);
}

void SchemeWindow::CreateActionBars() {
  m_actionToolBar = addToolBar("Панель действий");
  m_actionToolBar->setObjectName("m_actionToolBar");
  m_actionToolBar->addAction(m_startShemaScriptButton);
  m_actionToolBar->addAction(m_stopShemaScriptButton);
  m_actionToolBar->addSeparator();
  m_actionToolBar->addAction(m_synchronizationTimerBlocksButton);
  m_actionToolBar->addSeparator();
  m_actionToolBar->addAction(m_selectionPanelElementsButton);
}

void SchemeWindow::CreateCentralWidget() {
  m_switchCurrentSchema = new QTabWidget;
  setCentralWidget(m_switchCurrentSchema);
  QWidget *widgetPlusScheme = new QWidget;
  widgetPlusScheme->setProperty("name", "widgetPlusScheme");
  m_switchCurrentSchema->insertTab(0, widgetPlusScheme, "+");
  /// TODO Есть возможность добавить кнопки
  //    QWidget* addW = new QWidget;
  //    addW->addAction(m_addSchemeButton);
  //    addW->setFixedHeight(25);
  //    addW->setFixedWidth(15);
  //    m_switchCurrentSchema->tabBar()->setTabButton(0, QTabBar::LeftSide,
  //    addW);

  m_switchCurrentSchema->tabBar()->connect(m_switchCurrentSchema,
                                           SIGNAL(tabBarClicked(int)), this,
                                           SLOT(AddSchemeFromTab(int)));
  connect(m_switchCurrentSchema, SIGNAL(tabBarDoubleClicked(int)), this,
          SLOT(RenameTabDialog(int)));
  connect(m_switchCurrentSchema, SIGNAL(currentChanged(int)), this,
          SLOT(UpdateActionsMenu()));
}

void SchemeWindow::CreateContextMenu(SchemaView *schemaView) {
  QList<QAction *> itemsMenu;
  itemsMenu.append(m_cutBlockButton);
  itemsMenu.append(m_copyBlockButton);
  itemsMenu.append(m_pasteBlockButton);
  itemsMenu.append(m_deleteBlockButton);
  itemsMenu.append(m_propertiesBlockButton);
  schemaView->CreateContextMenu(itemsMenu);
}

void SchemeWindow::SetStyleSchemeWindow(QString path) {
  // Функция позволяет подгружать дополнительные стили для MainWindow
  // Файл должен распологаться в корнейвой папке StyleSheet/
  // Если файл не обнаружен, то выбирается файл по умолчанию
  QFile file(path); // чтения файла CSS
  if (!file.exists()) {
    file.setFileName(":/StyleSheet/MainWindow.qss");
  }

  file.open(QFile::ReadOnly);
  QString mainWindowStyleSheet = QLatin1String(file.readAll());
  qApp->setStyleSheet(mainWindowStyleSheet);
  m_itemsDialogWidget->SetStyleSheet(mainWindowStyleSheet);
  file.close();
}

bool SchemeWindow::DiagnosisConnectionScheme() { return true; }

void SchemeWindow::CreateSelectPanel() {
  m_itemDockWidget = new QDockWidget(tr("Выбор элементов"));
  m_itemDockWidget->setObjectName(tr("Выбор элементов"));
  m_itemsDialogWidget = new ItemsDialogWidget;
  m_itemDockWidget->setWidget(m_itemsDialogWidget);
  addDockWidget(Qt::LeftDockWidgetArea, m_itemDockWidget);
  m_itemDockWidget->show();
}

void SchemeWindow::SetEnabledTab(bool flag) {
  if (flag) {
    int curIndex = m_switchCurrentSchema->currentIndex();
    for (int i = 0; i <= m_scematics.size(); ++i) {
      if (i != curIndex)
        m_switchCurrentSchema->setTabEnabled(i, false);
    }
  } else {
    for (int i = 0; i <= m_scematics.size(); ++i) {
      m_switchCurrentSchema->setTabEnabled(i, true);
    }
  }
}

NameSchemeDialog::NameSchemeDialog(const QString &msg, QWidget *wparent)
    : QDialog(wparent) {
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(new QLabel(msg), 0, 0);
  QPushButton *okButton = new QPushButton("&Ок");
  okButton->setDefault(true);
  QPushButton *cancelButton = new QPushButton("О&тменить");

  m_textLine = new QLineEdit;
  m_textLine->setFixedWidth(500);
  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  layout->addWidget(m_textLine, 1, 0);
  layout->addLayout(buttonLayout, 1, 1);
  setLayout(layout);
  okButton->setFixedWidth(120);
  cancelButton->setFixedWidth(120);

  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString NameSchemeDialog::GetTextLine() const { return m_textLine->text(); }

void NameSchemeDialog::SetTextLine(const QString &textLine) {
  m_textLine->setText(textLine);
}

CloseSchemeDialog::CloseSchemeDialog(QWidget *wparent) : QDialog(wparent) {
  //    слой для кнопок деяйствия
  QHBoxLayout *buttonLayout;
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(new QLabel("Вы действительно хотите завершить работу?"), 0,
                    0, 1, 2);

  QPushButton *okButton = new QPushButton("&Да");
  okButton->setDefault(true);
  QPushButton *cancelButton = new QPushButton("О&тменить");

  okButton->setFixedWidth(120);
  cancelButton->setFixedWidth(120);

  buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  layout->addLayout(buttonLayout, 1, 1);
  setLayout(layout);

  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void SchemeWindow::closeEvent(QCloseEvent *event) {
  CloseSchemeDialog closeDialog;
  closeDialog.setStyleSheet(styleSheet());
  if (closeDialog.exec()) {
    // корректное завершение программы
    StopShemaScript();
    QCoreApplication::quit();
    return;
  }
  event->ignore();
}
