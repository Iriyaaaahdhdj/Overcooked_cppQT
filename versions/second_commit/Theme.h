#pragma once

#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QString>

namespace Theme {
inline QPen outlinePen()
{
    QPen pen(Qt::black, 1.0);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setCapStyle(Qt::RoundCap);
    return pen;
}

inline void roundedBox(QPainter *painter, const QRectF &rect, const QColor &fill)
{
    painter->setPen(outlinePen());
    painter->setBrush(fill);
    painter->drawRoundedRect(rect, 8.0, 8.0);
}

inline void text(QPainter *painter,
                 const QRectF &rect,
                 const QString &content,
                 int pointSize,
                 bool bold,
                 const QColor &color = Qt::black,
                 int alignment = Qt::AlignCenter)
{
    QFont font = painter->font();
    font.setPointSize(pointSize);
    font.setBold(bold);
    painter->setFont(font);
    painter->setPen(color);
    painter->drawText(rect, alignment, content);
}

inline QColor skyBlue() { return QColor(110, 196, 255); }
inline QColor cloudWhite() { return QColor(248, 252, 255); }
inline QColor truckOrange() { return QColor(255, 163, 32); }
inline QColor truckOrangeDark() { return QColor(235, 125, 20); }
inline QColor truckOrangeDeep() { return QColor(204, 92, 12); }
inline QColor panelBlue() { return QColor(104, 186, 245); }
inline QColor panelBlueLight() { return QColor(202, 236, 255); }
inline QColor panelBlueDark() { return QColor(64, 150, 214); }
inline QColor warmGray() { return QColor(90, 84, 80); }
inline QColor boardBrown() { return QColor(102, 66, 56); }
inline QColor tomatoRed() { return QColor(242, 76, 63); }
inline QColor leafGreen() { return QColor(120, 202, 63); }
inline QColor butterYellow() { return QColor(255, 209, 82); }
inline QColor mintGreen() { return QColor(131, 218, 152); }
inline QColor cream() { return QColor(245, 229, 193); }
}
