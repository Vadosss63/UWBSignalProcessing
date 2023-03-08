#ifndef SCHEMTICSSCENE_H
#define SCHEMTICSSCENE_H

#include <QGraphicsScene>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include "abstractblock.h"
#include "abstractoperationmanager.h"
#include "timerevent.h"
#include "timer.h"
#include "saveschematic.h"
#include "databaseblocks.h"
#include "lineconnect.h"

class TimerRun: public TimerListEvents
{
public:

    TimerRun();
    ~TimerRun();
    void Start(uint32_t msec);
    void Stop();

private:
    Timer<std::chrono::milliseconds>  m_timer;
};

class DilogTimer: public QDialog
{
    Q_OBJECT
public:
    DilogTimer(QWidget* parent = nullptr);
    ~DilogTimer();
signals:
    void ChangeTimer(int time);

private:
    QSpinBox* m_spinBox;
    QLabel* m_label;
};


// Класс хранение и обработки действий графических элементов
// выполняет обработку перемещений элементов с вспомогательной сцены,
// а также отвечет за отрисовку, перемещения и соединения элементов
class SchemticsController : public QGraphicsScene, public EventOperationManager
{
    Q_OBJECT
public:
    explicit SchemticsController(QObject *parent = nullptr);
    ~SchemticsController() override;
    SchemticsController(const SchemticsController& rhs) = delete;
    SchemticsController& operator=(const SchemticsController& rhs) = delete;

    // добавление блока и настройка значений по умочанию
    void AddBlock(AbstractBlock* block, const QPointF &pos);
    void SetOperationManager(AbstractOperationManager* manager);

    // состояние сцены
    bool IsStart() const;
    //  Есть ли выделеные элементы
    bool HasSelection() const;
    //  Есть ли выделеный блок
    bool IsSelecteBlock() const;
    //  Есть ли выделеные блоки
    bool IsSelecteBlocks() const;
    // Есть ли элементы для вставки
    bool IsPaste() const;
    // схема пустая?
    bool IsEmpty() const;

    // пауза схемы
    void Pause();

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
    // снятие выделения всех элементов схемы
    void UnSelecteAll();

    // выравнивание блоков
    void VerticalAligning();
    void HorizontalAligning();

    //переименовать схему
    bool RenameScheme(const QString&);

    void SaveScheme(const QString& path);
    void OpenScheme(const QString& path);

    void ShowTimer();

    // запуск схемы
    void Start();
    // остановка схемы
    void Stop();
    // открытие окна настроек для таймера
    void ShowSettingTimer();

    void StartEvent() override;
    void StopEvent() override;

signals:
    void StopOperation();
    void StartOperation();

protected:

    // действия выполняемые при перенесении элементов с другой сцены
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

public slots:
    // установка таймера
    void SetTimer(int time);

private:

    void BlocksIsMovable(bool isMovable);
    // получаем выделеный блок
    AbstractBlock* SelectedBlock() const;
    // получем выделеные блоки
    QList<AbstractBlock*> SelectedBlocks() const;
    // добавляет блок на сцену
    AbstractBlock* AddBlock(const BlockStruct& blockStruct);
    // создает линии соединения
    void CreateLink(const QList<LinkStruct>& connects, const QMap<size_t, AbstractBlock*>& abstructBlocks);
    // выполняет создания схемы из файла
    void CreateScem(SchemeReader& reader);

    // обьекты для копирования
    static QVector<BlockStruct> m_blockStructs;
    static QList<LinkStruct> m_linkStructs;

    // менеджер команд
    AbstractOperationManager* m_operationManager = nullptr;
    // таймер для запуска схемы
    TimerRun m_timer;
    // период таймера
    int m_time;
    //персональние id для блоков
    size_t m_id = 0;
    DilogTimer* m_dilogTimer;
    // Запущенна ли схема
    volatile bool m_isStart = false;
};

#endif // SCHEMTICSSCENE_H
