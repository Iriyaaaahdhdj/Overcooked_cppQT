#pragma once

#include <QColor>
#include <QPainter>
#include <QRectF>
#include <QString>

#include "Theme.h"

enum CarryItemType : int {
    NoItem = 0,
    TomatoItem,
    ChoppedTomatoItem,
    EggItem,
    PlateItem,
    SoupPlateItem
};

inline QString carryItemLabel(CarryItemType item)
{
    switch (item) {
    case TomatoItem:
        return QStringLiteral("Tomato");
    case ChoppedTomatoItem:
        return QStringLiteral("Chopped Tomato");
    case EggItem:
        return QStringLiteral("Egg");
    case PlateItem:
        return QStringLiteral("Plate");
    case SoupPlateItem:
        return QStringLiteral("Tomato Egg Soup");
    case NoItem:
    default:
        return QStringLiteral("Empty");
    }
}

inline void drawCarryItemIcon(QPainter *painter, CarryItemType item, const QRectF &rect)
{
    if (item == NoItem) {
        return;
    }

    painter->setPen(Theme::outlinePen());

    if (item == PlateItem || item == SoupPlateItem) {
        painter->setBrush(QColor(252, 252, 252));
        painter->drawEllipse(rect);
        painter->setBrush(QColor(230, 240, 250));
        painter->drawEllipse(rect.adjusted(rect.width() * 0.18,
                                           rect.height() * 0.18,
                                           -rect.width() * 0.18,
                                           -rect.height() * 0.18));
    }

    if (item == TomatoItem) {
        painter->setBrush(Theme::tomatoRed());
        painter->drawEllipse(rect.adjusted(rect.width() * 0.1,
                                           rect.height() * 0.12,
                                           -rect.width() * 0.1,
                                           -rect.height() * 0.08));
        painter->setBrush(Theme::leafGreen());
        painter->drawEllipse(QRectF(rect.center().x() - rect.width() * 0.09,
                                    rect.top() + rect.height() * 0.1,
                                    rect.width() * 0.18,
                                    rect.height() * 0.18));
        return;
    }

    if (item == ChoppedTomatoItem) {
        painter->setBrush(Theme::tomatoRed().lighter(105));
        const qreal piece = rect.width() * 0.28;
        painter->drawRoundedRect(QRectF(rect.left() + rect.width() * 0.14,
                                        rect.top() + rect.height() * 0.22,
                                        piece,
                                        piece),
                                 3.0,
                                 3.0);
        painter->drawRoundedRect(QRectF(rect.left() + rect.width() * 0.48,
                                        rect.top() + rect.height() * 0.2,
                                        piece,
                                        piece),
                                 3.0,
                                 3.0);
        painter->drawRoundedRect(QRectF(rect.left() + rect.width() * 0.3,
                                        rect.top() + rect.height() * 0.52,
                                        piece,
                                        piece),
                                 3.0,
                                 3.0);
        return;
    }

    if (item == EggItem) {
        painter->setBrush(QColor(254, 250, 233));
        painter->drawEllipse(rect.adjusted(rect.width() * 0.18,
                                           rect.height() * 0.12,
                                           -rect.width() * 0.18,
                                           -rect.height() * 0.08));
        painter->setBrush(QColor(250, 196, 64));
        painter->drawEllipse(QRectF(rect.center().x() - rect.width() * 0.11,
                                    rect.center().y() - rect.height() * 0.1,
                                    rect.width() * 0.22,
                                    rect.height() * 0.22));
        return;
    }

    if (item == SoupPlateItem) {
        painter->setBrush(QColor(255, 190, 92));
        painter->drawEllipse(rect.adjusted(rect.width() * 0.22,
                                           rect.height() * 0.24,
                                           -rect.width() * 0.22,
                                           -rect.height() * 0.22));
        painter->setBrush(Theme::leafGreen());
        painter->drawEllipse(QRectF(rect.center().x() - rect.width() * 0.06,
                                    rect.top() + rect.height() * 0.3,
                                    rect.width() * 0.12,
                                    rect.height() * 0.12));
        painter->setBrush(QColor(255, 244, 211));
        painter->drawEllipse(QRectF(rect.center().x() - rect.width() * 0.12,
                                    rect.center().y() - rect.height() * 0.03,
                                    rect.width() * 0.24,
                                    rect.height() * 0.16));
    }
}
