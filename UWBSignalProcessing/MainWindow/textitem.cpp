#include <QFont>
#include <QPainter>
#include "textitem.h"

TextItem::TextItem(const QString& text, const QRectF& rectF): m_text(text)
{
    setRect(rectF);
    setFlags(ItemIsSelectable);
}

TextItem::~TextItem(){}

void TextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QFont f("Times", 20);
    f.setUnderline(true);
    f.setStyle(QFont::StyleItalic);
    painter->setFont(f);
    painter->drawText(rect(), Qt::AlignCenter, m_text);
    painter->setFont(QFont());
}
