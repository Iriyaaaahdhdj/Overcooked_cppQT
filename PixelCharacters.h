#pragma once

#include <QColor>
#include <QPainter>
#include <QStringList>

namespace PixelCharacters {
inline void drawSprite(QPainter *painter,
                       const QPointF &topLeft,
                       int pixelSize,
                       const QStringList &rows,
                       const QHash<QChar, QColor> &palette)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    for (int y = 0; y < rows.size(); ++y) {
        const QString &row = rows.at(y);
        for (int x = 0; x < row.size(); ++x) {
            const QChar key = row.at(x);
            if (key == QLatin1Char('.') || !palette.contains(key)) {
                continue;
            }

            painter->setBrush(palette.value(key));
            painter->drawRect(QRectF(topLeft.x() + x * pixelSize,
                                     topLeft.y() + y * pixelSize,
                                     pixelSize,
                                     pixelSize));
        }
    }
    painter->restore();
}

inline void drawHelloKitty(QPainter *painter, const QPointF &topLeft, int pixelSize)
{
    static const QStringList rows = {
        "......OOOO......",
        ".....OWWWO......",
        "...OOOWWWOOO....",
        "..OWWWWWWWWWO...",
        "..OWWWWWWWWWOOO.",
        "..OWWWWWWWWWWWO.",
        ".OWWWWWWWWWWWWO.",
        ".OWKWWWWWWKWWWO.",
        ".OWWWWWYWWWWWWO.",
        "OOWWWWWWWWWWWWOO",
        "OWWWWWWWWWWWWWWO",
        "OWWWWWWWWWWWWWWO",
        ".OWWWWWWWWWWWWO.",
        ".OOOWWWWWWWOOO..",
        "..OPPOWWWPPP....",
        ".OPPPPOPPPPPO...",
        ".OPPPPPPPPPPPO..",
        ".OWWWWWWWWWWWO..",
        "OWWBLBLBLBLWWO..",
        "OWWWWWWWWWWWWO..",
        "OWWWWWWWWWWWWO..",
        ".OWWWOWWOWWWO...",
        "..OOO.OO.OOO...."
    };

    static const QHash<QChar, QColor> palette = {
        {QLatin1Char('O'), QColor(48, 40, 68)},
        {QLatin1Char('W'), QColor(255, 251, 252)},
        {QLatin1Char('K'), QColor(48, 40, 68)},
        {QLatin1Char('Y'), QColor(255, 215, 98)},
        {QLatin1Char('P'), QColor(244, 140, 182)},
        {QLatin1Char('B'), QColor(180, 215, 255)},
        {QLatin1Char('L'), QColor(255, 255, 255)}
    };

    drawSprite(painter, topLeft, pixelSize, rows, palette);
}

inline void drawCinnamoroll(QPainter *painter, const QPointF &topLeft, int pixelSize)
{
    static const QStringList rows = {
        "......OOOOOO....",
        "....OOWWWWWOO...",
        "...OOWWWWWWWOO..",
        "..OOWWWWWWWWWOO.",
        ".OOOWWWWWWWWWWOO",
        ".OWWWWWWWWWWWWWO",
        "OOWWWWWWWWWWWWWO",
        "OWWWWWWWWWWWWWWO",
        "OWKWLWWWWLWKWWWO",
        "OWWWWWWWWWWWWWWO",
        "OWWWWWWWWWWWWWWO",
        ".OWWWWWWWWWWWWO.",
        "..OWWWWWWWWWWO..",
        "..OOPPOWWOPPO...",
        ".OOBBBBBBBBBBO..",
        ".OWBLBLBLBLBWO..",
        "OWWBBBBBBBBWWO..",
        "OWWWWWWWWWWWWO..",
        "OWWWOWWWOWWWWO..",
        ".OWO.OOOO.OWO..."
    };

    static const QHash<QChar, QColor> palette = {
        {QLatin1Char('O'), QColor(64, 46, 62)},
        {QLatin1Char('W'), QColor(252, 252, 255)},
        {QLatin1Char('K'), QColor(64, 46, 62)},
        {QLatin1Char('L'), QColor(153, 216, 255)},
        {QLatin1Char('P'), QColor(244, 166, 198)},
        {QLatin1Char('B'), QColor(162, 214, 255)}
    };

    drawSprite(painter, topLeft, pixelSize, rows, palette);
}
}
