#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <QWidget>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QGraphicsTextItem>
#include <QDir>
#include <QPluginLoader>
#include <QList>
#include <QVBoxLayout>
#include "databaseblocks.h"


// класс диалогового окна для выбора элементов
class ItemsDialogWidget : public QWidget
{
    Q_OBJECT
public:

    ItemsDialogWidget(QWidget *parent = nullptr);
    ~ItemsDialogWidget() override;

    void SetStyleSheet(QString style);

    void ReloudBlocks();

public slots:
    void show();

protected:
    void resizeEvent(QResizeEvent *) override;
private:
    // создание Sink блоков
    void CreateSinkBlocks();
    // создание IO блоков
    void CreateIOBlocks();
    // создание Source блоков
    void CreateSourceBlocks();
    // создание области схемы
    void CreateSchematicsBlock();
    // настройка расположения блоков на сцене выбора
    void SetupItem(AbstractBlock* block);
    // настройка расположения блоков на сцене выбора
    void SetupItems(QList<AbstractBlock*> blocks);
    // добавление надписи "категория блока"
    void SetupTextItem(QGraphicsTextItem *text);
    // сброс положения исходных блоков
    void ResetItem();
    // выполняет загрузку блоков из папки
    void LoudBlocks();
    // выполняет установку размеров блока
    void InitSize(AbstractBlock* block);
    // установка положения названия блоков
    void SetupTitlePos(QGraphicsTextItem *item) const;
    // получение текущей позиции по Х для нового блока
    qreal GetXPos() const;
    // получение текущей позиции по Y для нового блока
    qreal GetYPos() const;

    void SetupSourceBlocks();

    void SetupIOBlocks();

    void SetupSinkBlocks();

    // для хранения исходных блоков
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    // размер блока
    QSizeF m_rectSize;
    // номер для последовательного расположения блоков
    int m_seqNumberItem;
    // количество Блоков в строке
    int m_quantityToLineItem;
    // сдвиг относительно блоков
    static int m_margin;
    // Выполняет загрузку блоков
    DatabaseBlocks* m_databaseBlocks = nullptr;

    QGraphicsTextItem* m_titleSorce = nullptr;

    QGraphicsTextItem* m_titleIO = nullptr;

    QGraphicsTextItem* m_titleSink = nullptr;
};

#endif // SELECTPANEL_H
