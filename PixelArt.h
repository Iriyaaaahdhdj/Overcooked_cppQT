#pragma once

#include <QColor>
#include <QHash>
#include <QPainter>
#include <QPointF>
#include <QStringList>

namespace PixelArt {
using Palette = QHash<QChar, QColor>;

inline void drawSprite(QPainter *painter,
                       const QStringList &rows,
                       const Palette &palette,
                       const QPointF &topLeft,
                       qreal pixelSize)
{
    painter->setPen(Qt::NoPen);
    for (int y = 0; y < rows.size(); ++y) {
        const QString &row = rows.at(y);
        for (int x = 0; x < row.size(); ++x) {
            const QChar key = row.at(x);
            if (key == QChar('.')) {
                continue;
            }
            painter->fillRect(QRectF(topLeft.x() + x * pixelSize,
                                     topLeft.y() + y * pixelSize,
                                     pixelSize,
                                     pixelSize),
                              palette.value(key, QColor(255, 0, 255)));
        }
    }
}

inline Palette helloKittyPalette()
{
    return {
        {QChar('O'), QColor(52, 41, 72)},
        {QChar('W'), QColor(255, 251, 252)},
        {QChar('S'), QColor(236, 242, 255)},
        {QChar('P'), QColor(245, 181, 197)},
        {QChar('R'), QColor(233, 110, 145)},
        {QChar('B'), QColor(187, 215, 255)},
        {QChar('D'), QColor(146, 191, 247)},
        {QChar('L'), QColor(255, 255, 255)},
        {QChar('Y'), QColor(255, 216, 94)},
        {QChar('K'), QColor(36, 36, 36)}
    };
}

inline Palette cinnamorollPalette()
{
    return {
        {QChar('O'), QColor(84, 53, 74)},
        {QChar('W'), QColor(252, 253, 255)},
        {QChar('S'), QColor(235, 241, 255)},
        {QChar('B'), QColor(178, 222, 255)},
        {QChar('D'), QColor(128, 190, 245)},
        {QChar('P'), QColor(246, 186, 208)},
        {QChar('R'), QColor(233, 132, 176)},
        {QChar('Y'), QColor(255, 219, 98)},
        {QChar('K'), QColor(38, 38, 38)}
    };
}

inline QStringList helloKittyChefSprite()
{
    return {
        "...........OOOOOO...........",
        ".........OOOWWWWOOO.........",
        "........OOWWWWWWWWOO........",
        ".......OOWWWWWWWWWWOO.......",
        ".......OWWWWWWWWWWWWO.......",
        "......OOWWWWWWWWWWWWWO......",
        "......OWWWWWWWWWWWWWWWO.....",
        "......OWWWWWWWWWWWWWWWO.....",
        ".....OOWWWWWWWWWWWWWWWWOO...",
        ".....OWWWWWWWWWWWWWWWWWWWO..",
        ".....OWWWWWWWWWWWWWWWWWWWO..",
        "....OOWWWWWWWWWWWWWWWWWWWO..",
        "....OWWWWWWWWWWWWWWWWWWWWWO.",
        "...OOWWWWWWWWWWWWWWWWWWWWWO.",
        "...OWWWKWWWWWWWWWWWWWWKWWWO.",
        "...OWWWWWWWWWWWYYWWWWWWWWWO.",
        "...OWWWWWWWWWWWWWWWWWWWWWWO.",
        "...OOWWWWWWWWWWWWWWWWWWWWOO.",
        "....OWWWWWWWWWWWWWWWWWWWO...",
        "....OOWWWWWWWWWWWWWWWWWOO...",
        "...OOOWWWWWWWWWWWWWWWWWO....",
        "..OOWRRRWWWWWWWWWWWWWWO.....",
        "..OWRRPRRWWWWWWWWWWWWO......",
        "..OWRRRRRWWWWWWWWWWWO.......",
        "..OOPPPPPPPPPPPPPPPPO.......",
        ".OOPPBBBBBBBBBBBBBBPPO......",
        ".OPPBBDBDBDBDBDBDBBBPPO.....",
        ".OPPBBBBBBBBBBBBBBBBBPPO....",
        ".OPPBBDBDBDBDBDBDBBBBPPO....",
        ".OPPBBBBBBBBBBBBBBBBBPPO....",
        "..OPPPPPPPPPPPPPPPPPPO......",
        "...OOOWWOO.....OOWWOO.......",
        "...OOWWWO.......OWWWOO......",
        "....OOO...........OOO......."
    };
}

inline QStringList cinnamorollChefSprite()
{
    return {
        ".............OOOOOOOOO.............",
        "..........OOOOWWWWWWWOOOO..........",
        ".........OOOWWWWWWWWWWWOO..........",
        "........OOWWWWWWWWWWWWWWOO.........",
        ".......OOWWWWWWWWWWWWWWWWOO........",
        "......OOWWWWWWWWWWWWWWWWWWOO.......",
        "......OWWWWWWWWWWWWWWWWWWWWO.......",
        ".....OOWWWWWWWWWWWWWWWWWWWWWOO.....",
        "....OOOWWWWWWWWWWWWWWWWWWWWWWOO....",
        "...OOOWWWWWWWWWWWWWWWWWWWWWWWWOO...",
        "..OOOWWWWWWWWWWWWWWWWWWWWWWWWWWOO..",
        "..OOWWWWWWWWWWWWWWWWWWWWWWWWWWWWOO.",
        "..OWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO.",
        ".OOWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO.",
        ".OWWKWWWWWWWWWWWWWWWWWWWWWWWWKWWWO.",
        ".OWWWWWWWWWBWWWWWWWWWWBWWWWWWWWWWO.",
        ".OWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO.",
        ".OWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO.",
        "..OWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO..",
        "..OOWWWWWWWWWWWWWWWWWWWWWWWWWWWOO..",
        "...OOWWWWWWWRRRWWWWWWRRRWWWWWWWOO..",
        "....OOWWWWRRPRRWWWWRRPRRWWWWWWO....",
        ".....OWWWWRRRRRWWWWRRRRRWWWWWO.....",
        ".....OWWWWPPPPPWWWWPPPPPWWWWWO.....",
        ".....OWWWWPPBBBWWWWBBBPPWWWWWO.....",
        ".....OWWWWPPDBBWWWWBDBPPWWWWWO.....",
        ".....OWWWWPPBBBWWWWBBBPPWWWWWO.....",
        ".....OWWWWPPPPPWWWWPPPPPWWWWWO.....",
        "......OWWWWWWWWWWWWWWWWWWWWWO......",
        "......OWWWWWWWWWWWWWWWWWWWWWO......",
        ".....OOWWWWWWWWWWWWWWWWWWWWWOO.....",
        "....OOWWWWWWWWWWWWWWWWWWWWWWWO.....",
        "...OOWWWWWWWWWWWWWWWWWWWWWWWWWOO...",
        "..OOWWWWWWWWWWWWWWWWWWWWWWWWWWWOO..",
        "..OWWWWWWWWWWWWWWWWWWWWWWWWWWWWWO..",
        ".OOWWWWWWWBBBBBBBBBBBBBBBBWWWWWWO..",
        ".OWWWWWWBBDBDBDBDBDBDBDBBBWWWWWWO..",
        ".OWWWWWWBBBBBBBBBBBBBBBBBBWWWWWWO..",
        ".OWWWWWWBBDBDBDBDBDBDBDBBBWWWWWWO..",
        "..OWWWWWWBBBBBBBBBBBBBBBBWWWWWWO...",
        "...OWWWWWWWWWWWWWWWWWWWWWWWWWWO....",
        "...OOWWWWWWWWWWWWWWWWWWWWWWWWOO....",
        "....OOOWWWWWWWWWWWWWWWWWWWWOO......",
        "......OOWWWWWWWWWWWWWWWWWWOO.......",
        "......OOOWWWWOO....OOWWWWOO........",
        ".......OOWWWO........OWWWO.........",
        "........OOO............OOO........."
    };
}
}
