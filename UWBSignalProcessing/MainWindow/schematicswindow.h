#ifndef SCHEMATICSWINDOW_H
#define SCHEMATICSWINDOW_H

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLayout>
#include <QMainWindow>
#include <QSettings>
#include <QToolBar>
#include <QTreeWidget>
#include <QWidget>

#include "abstractblock.h"
#include "databaseblocks.h"
#include "saveschematic.h"
#include "schemticsscene.h"
#include "selectpanel.h"
#include "textitem.h"

// Главный класс осуществяющий отображение всех графических интерфейсов
// класс содержит в себе методы копировая, удаления, вставки и др.
// класс содержит в себе две сцены, одна из которых осуществляет представление
// элементов пользователю, а другая выполняет непосредственное отображение схемы
class SchemeReader;

class SchematicsWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit SchematicsWindow(QWidget *parent = nullptr);
  ~SchematicsWindow();
  SchematicsWindow(const SchematicsWindow &rhs) = delete;
  SchematicsWindow &operator=(const SchematicsWindow &rhs) = delete;

  void Start();
  // flag  для рабочий схемы
  void SetIsRelease(bool isRelease);
  // Открытие схемы из файла
  void OpenFile(QString path);

  void SetOperationManager(AbstractOperationManager *manager);

public slots:
  // блокировка схемы
  void LockActions();
  // разблокировка схемы
  void UnlockActions();

protected:
  // действия при нажажатии клавиш
  void keyPressEvent(QKeyEvent *event);

private slots:
  // открытия сохраненной схемы
  void Open();
  // сохранения схемы
  void Save();
  // удаление блока
  void Del();
  // вырезать блок
  void Cut();
  // копировать блок
  void Copy();
  // вставить блок
  void Paste();
  // свойства блока
  void Properties();
  // обновление меню событий
  void UpdateActions();

  // Увеличение схемы
  void ZoomIn();
  // Уменьшение схемы
  void ZoomOut();
  // Выделение всех элементов на схеме
  void SelecteAll();

private:
  bool IsStart() const;

  void SetIsStart(bool isStart);

  // функция сохранения настроек боковой панели
  void WriteSettings();
  // функция чтения настроек боковой панели
  void ReadSettings();
  // создания события
  void CreateActions();
  // создание меню
  void CreateMenus();
  // создание меню инструментов
  void CreateEditToolBars();
  // создание меню действий
  void CreateActionBars();
  // создание контекстного меню
  void CreateContextMenu();
  // создание схемы из сохраненного файла
  void CreateScem(SchemeReader &reader);

  // Настройка стиля главного окна
  void SetStyleSchemeWindow(QString path = QString("StyleSheet/style.qss"));

  QLabel *m_statusBar;
  // выбор блока
  AbstractBlock *SelectedBlock() const;

  // меню инструментов
  QToolBar *m_editToolBar;
  // меню запуска и остановки схемы
  QToolBar *m_actionToolBar;

  // кнопка открыть
  QAction *m_openAction;
  // кнопка сохранить
  QAction *m_saveAction;

  // кнопка вырезать
  QAction *m_cutAction;
  // кнопка копировать
  QAction *m_copyAction;
  // кнопка вставить
  QAction *m_pasteAction;

  // кнопка удаления
  QAction *m_deleteAction;
  // кнопка свойства
  QAction *m_propertiesAction;

  // кнопка уменьшения
  QAction *m_zoomoutAction;
  // кнопка увеличения
  QAction *m_zoominAction;

  // кнопка запуска схемы
  QAction *m_startAction;
  // кнопка остановки схемы
  QAction *m_stopAction;
  // панели выбора элементов
  QAction *m_switchShowCloseAction;
  // панели выбора элементов
  QAction *m_rs485Action = nullptr;
  // кнопка настройки таймера блока
  QAction *m_timer;
  // параметры для отображения блоков
  QGraphicsView *m_view;
  SchemticsController *m_scene;
  // виджет хранения блоков
  ItemsDialogWidget *m_itemsDialogWidget;
  // запуск приложениия в релизе
  mutable bool m_isRelease = false;
  // обьект для копирования
  AbstractBlock *m_nodeBuffer;
  // боковая панель выбора блоков
  QDockWidget *m_itemDockWidget;
  // Запущенна ли схема
  bool m_isStart = false;
};

//// класс диалогового окна для выбора элементов
// class ItemsDialogWidget : public QWidget
//{
//     Q_OBJECT
// public:
//     ItemsDialogWidget(QWidget *parent = nullptr);
//     ~ItemsDialogWidget();

// public slots:
//     void show();

// protected:
//     void resizeEvent(QResizeEvent * event);
// private:
//     // создание Sink блоков
//     void CreateSinkBlocks();
//     // создание IO блоков
//     void CreateIOBlocks();
//     // создание Source блоков
//     void CreateSourceBlocks();
//     // создание области схемы
//     void CreateSchematicsBlock();
//     // настройка расположения блоков на сцене выбора
//     void SetupItem(AbstractBlock* block);
//     // добавление надписи "категория блока"
//     void SetupTextItem(const QString& text);
//     // сброс положения исходных блоков
//     void ResetItem();
//     // для хранения исходных блоков
//     QGraphicsScene* m_scene;
//     QGraphicsView* m_view;
//     // номер для последовательного расположения блоков
//     int m_seqNumberItem;
//     // исходные размеры блока
//     const QSizeF m_itemSize;
//     // отступы от блоков
//     const QSizeF m_marginRectSize;
//     // количество Блоков в строке
//     size_t m_quantityToLineItem;
//     // сдвиг относительно блоков
//     const size_t m_step = 100;
// };

#endif // SCHEMATICSWINDOW_H
