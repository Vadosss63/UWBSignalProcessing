#ifndef ABSTRACTBLOCK_H
#define ABSTRACTBLOCK_H

#include <QAction>
#include <QApplication>
#include <QBitmap>
#include <QDrag>
#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QMimeData>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

#include "abstractdialogcommand.h"
#include "abstractmodule.h"
#include "changeevent.h"
#include "savestruct.h"

class AbstractBlock;
class IPortBlock;
class OPortBlock;

class Block {

public:
  virtual ~Block();
  // доступ к абстракт модулю
  virtual AbstractModule *GetModule() const = 0;
  // Функция иницилизации
  virtual void InitBlock(QWidget *wparent) = 0;
  // клонирование блока
  virtual AbstractBlock *Clone() const = 0;
  // Получение типа порта
  virtual QString GetType() const = 0;
  virtual void
  RegistrOperationManager(AbstractOperationManager *operationManager) = 0;
  // получения ветора входных
  virtual QVector<IPortBlock *> GetInPort() const = 0;
  // получение векторы выходных портов
  virtual QVector<OPortBlock *> GetOutPort() const = 0;
  // получение настроек
  virtual QList<std::pair<QString, QVariant>> GetSetting() const = 0;
  // установка настроек
  virtual bool SetSetting(QList<std::pair<QString, QVariant>> list) = 0;
  // Вызывает окно описания модуля
  virtual void ShowHelp() = 0;

  virtual BlockStruct GetBlockStruct() = 0;
  virtual QList<LinkStruct> GetLinkStruct() = 0;

protected:
  // отрисовка изображения блока
  virtual void DrawImage(QPainter *painter) = 0;
};

// абстрактный класс для графических блоков, отвечающий за отображения
// начального прямоугольника и настроек отображения
class AbstractBlock : public QGraphicsItem, public Block, public ChangeEvent {

public:
  AbstractBlock(const QString &lable, QGraphicsItem *parent = nullptr);
  ~AbstractBlock() override;
  AbstractBlock(const AbstractBlock &rhs) = delete;
  AbstractBlock &operator=(const AbstractBlock &rhs) = delete;

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *) override;

  void ShowHelp() override;

  // устанока параметров прямоугольника
  void SetRect(QRect &r);
  void SetRectWidth(int w);
  void SetRectHeight(int h);

  // получение параметров прямоугольника
  QRectF &GetborderRect();
  int GetRectWidth() const;
  int GetRectHeight() const;

  //открытие диалогового окна блока
  void ShowDialog();

  size_t GetId() const;
  // Установка Id на схеме
  virtual void SetId(size_t id);

  void SetAbstractDialogCommand(DialogWindow *abstractDialogCommand);

  QList<std::pair<QString, QVariant>> GetSetting() const override;
  bool SetSetting(QList<std::pair<QString, QVariant>> list) override;

  // Событие измениения
  void Change() override;

  BlockStruct GetBlockStruct() override;
  QList<LinkStruct> GetLinkStruct() override;

protected:
  // действия мыши
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  // отрисовка квадрата
  void DrawRect(QPainter *painter, const QStyleOptionGraphicsItem *option);

private:
  // эфекты отрисовки
  QRectF OutlineRect() const;
  // вычисления закругления
  int Roundness(double size) const;
  // название блока
  QString m_lable;
  // цвет текста
  QColor m_textColor;
  // заливка
  QColor m_backgroundColor;
  // цвет обводки квадрата
  QColor m_outLineColor;
  QRectF m_borderRect;
  // персональной номер для блока
  size_t m_id = 0;
  // интерфейс диалогового окна
  DialogWindow *m_dialogWindow = nullptr;
};

// класс для создания временных объектов перемещения по рабочему столу
class BlockMimeData : public QMimeData {
  Q_OBJECT
public:
  explicit BlockMimeData(AbstractBlock *Block);
  ~BlockMimeData() override;
  bool HasAbstractBlock() const;
  AbstractBlock *Block() const;

private:
  AbstractBlock *m_block;
};

#endif // ABSTRACTBLOCK_H
