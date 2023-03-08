#ifndef BILDERBLOCKS_H
#define BILDERBLOCKS_H

#include <QtCore/QtPlugin>

class AbstractBlock;

class BilderBlocks
{
public:
    virtual ~BilderBlocks() = default;
    virtual AbstractBlock* LoudBlock() const = 0;
};
Q_DECLARE_INTERFACE(BilderBlocks, "Radar.UWBSignalProcessing.BilderBlocks")

#endif // BILDERBLOCKS_H
