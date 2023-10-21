#ifndef TRANSMITTERUDPBLOCK_H
#define TRANSMITTERUDPBLOCK_H

#include "abstractblock.h"
#include "interfaces.h"
#include "transmitterudpmodule.h"

class PluginBlock : public QObject, public BilderBlocks {
  Q_OBJECT
  Q_INTERFACES(BilderBlocks)
  Q_PLUGIN_METADATA(IID "Radar.UWBSignalProcessing.BilderBlocks" NAME_FILE)

public:
  PluginBlock(QObject *parent = nullptr);
  // Создает и настраевает блок
  AbstractBlock *LoudBlock() const override;
};

// Класс осуществяющий взаимодействие графического блока и TransmitterUDPModule
// Содержит в себе мотоды отрисовка графического блока

class TransmitterUDPBlock : public SinkBlock {
  Q_OBJECT
public:
  // создаем объект
  TransmitterUDPBlock();
  // освобождаем ресурсы
  ~TransmitterUDPBlock() override;
  TransmitterUDPBlock(const TransmitterUDPBlock &rhs) = delete;
  TransmitterUDPBlock &operator=(const TransmitterUDPBlock &rhs) = delete;
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
  void InitBlock(QWidget *) override;
  // Указатель на модуль
  dspm::SenderUDPModule *m_module;
};

#endif // TRANSMITTERUDPBLOCK_H
