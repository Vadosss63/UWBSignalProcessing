#ifndef RECEIVERUDPBLOCK_H
#define RECEIVERUDPBLOCK_H

#include "abstractblock.h"
#include "interfaces.h"
#include "receiverudpdialogbox.h"
#include "receiverudpmodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  // Создает и настраевает блок
  AbstractBlock *LoudBlock() const override;
};

// Класс осуществяющий взаимодействие графического блока и ReceiverUDPModule
// Содержит в себе мотоды отрисовка графического блока
class ReceiverUDPBlock : public SourceBlock {
  Q_OBJECT
public:
  // создаем объект
  ReceiverUDPBlock();
  // освобождаем ресурсы
  ~ReceiverUDPBlock() override;
  ReceiverUDPBlock(const ReceiverUDPBlock &rhs) = delete;
  ReceiverUDPBlock &operator=(const ReceiverUDPBlock &rhs) = delete;
  AbstractBlock *Clone() const override;
  // доступ к абстракт модулю
  dspm::AbstractModule &GetModule() override;
  QString GetType() override;

protected:
  // событие отрисовки
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *) override;
  // Создание рисунка
  void DrawImage(QPainter *painter) override;

private:
  // Функция иницилизации
  void InitBlock(QWidget *wparent) override;
  // Указатель на модуль
  dspm::ReceiverUDPModule *m_module;
};

#endif // RECEIVERUDPBLOCK_H
