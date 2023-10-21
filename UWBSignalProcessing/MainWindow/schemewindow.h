#ifndef SCHEMATICSWINDOW_H
#define SCHEMATICSWINDOW_H

#include <QAction>
#include <QBoxLayout>
#include <QDockWidget>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

#include "abstractblock.h"
#include "databaseblocks.h"
#include "saveschematic.h"
#include "schemaview.h"
#include "schemticsscene.h"
#include "selectpanel.h"
#include "textitem.h"

// Основное диологовое окно программы.
// Окно предназначено для графического отображения математических схем и
// Предоставляет основные средства работы с ними, такие как добавление,
// удаление, копирование, вставка и выделение элементарных блоков.
// Средвтва управления (Запуск, пауза, остановка) схемы.
class SchemeWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit SchemeWindow(QWidget *parent = nullptr);
  ~SchemeWindow() override;
  // Запрет конструктора копирования и присваевания
  SchemeWindow(const SchemeWindow &rhs) = delete;
  SchemeWindow &operator=(const SchemeWindow &rhs) = delete;

  void SetOperationManager(AbstractOperationManager *manager);
  /// TODO Debag
  void DEBAG();

public slots:
  // Запуск выполнения сценария схемы
  void StartShemaScript();
  // Запуск выполнения сценария схемы
  void PauseShemaScript();
  // Запуск выполнения сценария схемы
  void StopShemaScript();
  void ShowTimer();
  // блокировка схемы
  void LockActionsMenu();
  // разблокировка схемы
  void UnlockActionsMenu();
  // Показать панельвыбора элементов или скрыть
  void ShowSelectPanel();

  void AddViewOfSchema(const QString &nameSchema);

protected:
  // действия при нажажатии клавиш
  void keyPressEvent(QKeyEvent *event) override;
  // действие при закрытии формы
  void closeEvent(QCloseEvent *event) override;

private slots:
  // открытия сохраненной схемы
  void OpenScheme();
  // сохранения схемы
  void SaveScheme();
  // добавляет схему на сцену
  void AddScheme();
  // удаляет схему
  void RemoveScheme();
  // удаляет схему
  void RemoveSchemeScene(int index);
  // удаление блока
  void DelBlock();
  // вырезать блок
  void CutBlock();
  // копировать блок
  void CopyBlock();
  // вставить блок
  void PasteBlock();
  // свойства блока
  void PropertiesBlock();
  // Выделение всех элементов на схеме
  void SelecteAll();

  void UndoBlock();
  void RedoBlock();

  // Увеличение схемы
  void ZoomInScheme();
  // Исходный зум
  void ZoomOriginalScheme();
  // Уменьшение схемы
  void ZoomOutScheme();

  // обновление меню событий
  void UpdateActionsMenu();
  void DisableActionMenu();

  // открытия стиля схемы
  void OpenCSS();

  void AddSchemeFromTab(int index);

  void RenameTabDialog(int index);

  void RenameScheme(const QString &nameSchema);

  void VerticalAligning();

  void HorizontalAligning();

  void ReloudBlocks();

private:
  // функция записи настроек главного окна
  void WriteSettingsWindow();
  // функция чтения настроек главного окна
  void ReadSettingsWindow();
  // создания события
  void CreateActionsMenu();
  // Создание строки состояния
  void CreateStatusBar();
  // создание меню
  void CreateMenu();
  // создание меню инструментов
  void CreateEditToolBars();
  // создание меню действий
  void CreateActionBars();
  // создание Центрального виджета
  void CreateCentralWidget();
  // создание контекстного меню
  void CreateContextMenu(SchemaView *schemaView);
  // Настройка стиля главного окна
  void
  SetStyleSchemeWindow(QString path = QString("StyleSheet/MainWindow.qss"));
  // диагностика соединений между блоками в схеме
  bool DiagnosisConnectionScheme();
  // создание панели выбора блоков
  void CreateSelectPanel();

  void SetEnabledTab(bool flag);

  SchemaView *GetCurrentSchema();

  // меню инструментов
  QToolBar *m_editToolBar = nullptr;
  // меню запуска и остановки схемы
  QToolBar *m_actionToolBar = nullptr;
  // Пункты системы меню
  QMenu *m_fileMenu = nullptr;
  QMenu *m_editMenu = nullptr;
  QMenu *m_toolMenu = nullptr;
  QMenu *m_optionsMenu = nullptr;
  QMenu *m_helpMenu = nullptr;

  // индикатор работы схемы (зеленый - работает; красный - не работает)
  QPushButton *m_statusWorkShema = nullptr;

  // кнопка открыть
  QAction *m_openFileSchemeButton = nullptr;
  // кнопка сохранить
  QAction *m_saveFileSchemaButton = nullptr;

  // кнопка добавить схему
  QAction *m_addSchemeButton = nullptr;
  // кнопка удалить схему
  QAction *m_removeSchemaButton = nullptr;

  // кнопка вырезать
  QAction *m_cutBlockButton = nullptr;
  // кнопка копировать
  QAction *m_copyBlockButton = nullptr;
  // кнопка вставить
  QAction *m_pasteBlockButton = nullptr;
  // кнопка удаления
  QAction *m_deleteBlockButton = nullptr;
  // кнопка свойства
  QAction *m_propertiesBlockButton = nullptr;
  // кнопка отмены
  QAction *m_undoBlockButton = nullptr;
  // кнопка возврата
  QAction *m_redoBlockButton = nullptr;

  // кнопка уменьшения
  QAction *m_zoomOutSchemaButton = nullptr;
  // кнопка сброса зума
  QAction *m_zoomOriginalSchemaButton = nullptr;
  // кнопка увеличения
  QAction *m_zoomInSchemaButton = nullptr;

  // кнопка запуска схемы
  QAction *m_startShemaScriptButton = nullptr;
  // кнопка паузы схемы
  QAction *m_pauseShemaScriptButton = nullptr;
  // кнопка остановки схемы
  QAction *m_stopShemaScriptButton = nullptr;
  // кнопка панели выбора элементов
  QAction *m_selectionPanelElementsButton = nullptr;
  // кнопка настройки главного тамера синхронизации блоков
  QAction *m_synchronizationTimerBlocksButton = nullptr;
  // вертикальное выравнивание блоков
  QAction *m_verticalAligning = nullptr;
  // горизонтальное выравнивание блоков
  QAction *m_horizontalAligning = nullptr;
  // перезагрузка боковой панели
  QAction *m_reloadItemsDialogWidget = nullptr;

  // кнопка открыть
  QAction *m_openFileCSS = nullptr;

  // Объект представления для отображения схемы из блоков
  QList<SchemaView *> m_scematics;

  // боковая панель выбора блоков
  QDockWidget *m_itemDockWidget = nullptr;
  // виджет хранения блоков
  ItemsDialogWidget *m_itemsDialogWidget = nullptr;
  // панель переключения между схемами
  QTabWidget *m_switchCurrentSchema = nullptr;
  AbstractOperationManager *m_operationmanager = nullptr;
};

// класс диалогового окна для переименования схемы
class NameSchemeDialog : public QDialog {
  Q_OBJECT
public:
  NameSchemeDialog(
      const QString &msg = "Введите новое название для даной схемы",
      QWidget *wparent = nullptr);
  QString GetTextLine() const;
  void SetTextLine(const QString &textLine);

private:
  //    текстовое поля названия файла
  QLineEdit *m_textLine;
};

// класс диалогового окна для закрытия приложения
class CloseSchemeDialog : public QDialog {
  Q_OBJECT
public:
  CloseSchemeDialog(QWidget *wparent = nullptr);
};

#endif // SCHEMATICSWINDOW_H
