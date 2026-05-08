#pragma once

#include <QColor>
#include <QPainter>
#include <QPolygonF>
#include <QRectF>
#include <QString>

enum CarryItemType : int {
    NoItem = 0,
    RiceItem,
    CookedRiceItem,
    NoriItem,
    CucumberItem,
    ChoppedCucumberItem,
    ShrimpItem,
    ChoppedShrimpItem,
    SushiBaseItem,
    CucumberRollItem,
    ShrimpRollItem,
    CleanPlateItem,
    DirtyPlateItem,
    BurntRiceItem
};

constexpr int PlateDishMask = 0x100;
constexpr int PlateRiceBit = 0x01;
constexpr int PlateNoriBit = 0x02;
constexpr int PlateCucumberBit = 0x04;
constexpr int PlateFishBit = 0x08;
constexpr int PlateIngredientMask = PlateRiceBit | PlateNoriBit | PlateCucumberBit | PlateFishBit;

inline bool isPlatedDish(CarryItemType item)
{
    const int value = static_cast<int>(item);
    return (value & PlateDishMask) == PlateDishMask && (value & PlateIngredientMask) != 0;
}

inline int platedDishBits(CarryItemType item)
{
    if (!isPlatedDish(item)) {
        return 0;
    }
    return static_cast<int>(item) & PlateIngredientMask;
}

inline CarryItemType makePlatedDish(int bits)
{
    return bits == 0 ? CleanPlateItem : static_cast<CarryItemType>(PlateDishMask | (bits & PlateIngredientMask));
}

inline bool isPlateCarrierItem(CarryItemType item)
{
    return item == CleanPlateItem || isPlatedDish(item);
}

inline int ingredientBitForPlate(CarryItemType item)
{
    switch (item) {
    case CookedRiceItem:
        return PlateRiceBit;
    case NoriItem:
        return PlateNoriBit;
    case ChoppedCucumberItem:
        return PlateCucumberBit;
    case ChoppedShrimpItem:
        return PlateFishBit;
    default:
        return 0;
    }
}

inline QString carryItemLabel(CarryItemType item)
{
    if (isPlatedDish(item)) {
        const int bits = platedDishBits(item);
        QString label = QStringLiteral("Plate");
        if ((bits & PlateRiceBit) != 0) {
            label += QStringLiteral(" + Rice");
        }
        if ((bits & PlateCucumberBit) != 0) {
            label += QStringLiteral(" + Cucumber");
        }
        if ((bits & PlateFishBit) != 0) {
            label += QStringLiteral(" + Fish");
        }
        if ((bits & PlateNoriBit) != 0) {
            label += QStringLiteral(" + Nori");
        }
        return label;
    }

    switch (item) {
    case RiceItem:
        return QStringLiteral("Rice");
    case CookedRiceItem:
        return QStringLiteral("Cooked Rice");
    case NoriItem:
        return QStringLiteral("Nori");
    case CucumberItem:
        return QStringLiteral("Cucumber");
    case ChoppedCucumberItem:
        return QStringLiteral("Chopped Cucumber");
    case ShrimpItem:
        return QStringLiteral("Shrimp");
    case ChoppedShrimpItem:
        return QStringLiteral("Chopped Shrimp");
    case SushiBaseItem:
        return QStringLiteral("Rice + Nori");
    case CucumberRollItem:
        return QStringLiteral("Cucumber Roll");
    case ShrimpRollItem:
        return QStringLiteral("Shrimp Roll");
    case CleanPlateItem:
        return QStringLiteral("Clean Plate");
    case DirtyPlateItem:
        return QStringLiteral("Dirty Plate");
    case BurntRiceItem:
        return QStringLiteral("Burnt Rice");
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

    if (isPlateCarrierItem(item)) {
        const QPointF center = rect.center();
        const qreal plateWidth = rect.width() * 0.92;
        const qreal plateHeight = qMax<qreal>(rect.height() * 0.48, plateWidth * 0.50);
        const QRectF plateRect(center.x() - plateWidth / 2.0,
                               center.y() - plateHeight / 2.0,
                               plateWidth,
                               plateHeight);

        painter->setPen(QPen(QColor(138, 130, 120), qMax<qreal>(1.0, rect.width() / 28.0)));
        painter->setBrush(QColor(255, 252, 244));
        painter->drawEllipse(plateRect);
        painter->setPen(QPen(QColor(207, 199, 187), qMax<qreal>(1.0, rect.width() / 34.0)));
        painter->setBrush(QColor(236, 232, 222));
        painter->drawEllipse(plateRect.adjusted(plateWidth * 0.20,
                                                plateHeight * 0.22,
                                                -plateWidth * 0.20,
                                                -plateHeight * 0.22));

        if (item == CleanPlateItem) {
            return;
        }

        const int bits = platedDishBits(item);
        const bool hasRice = (bits & PlateRiceBit) != 0;
        const bool hasNori = (bits & PlateNoriBit) != 0;
        const bool hasCucumber = (bits & PlateCucumberBit) != 0;
        const bool hasFish = (bits & PlateFishBit) != 0;
        const bool isFinishedSushi = hasRice && hasNori && (hasCucumber || hasFish);

        painter->setPen(Qt::NoPen);

        auto drawRoll = [&](const QPointF &rollCenter, qreal rollWidth, qreal rollHeight, bool cucumber, bool fish) {
            const qreal topHeight = rollHeight * 0.62;
            const qreal sideHeight = rollHeight * 0.34;
            const QRectF side(rollCenter.x() - rollWidth / 2.0,
                              rollCenter.y() - topHeight * 0.12,
                              rollWidth,
                              sideHeight);
            painter->setBrush(QColor(23, 55, 42));
            painter->drawRoundedRect(side, sideHeight * 0.18, sideHeight * 0.18);
            painter->setBrush(QColor(54, 103, 73));
            painter->drawRect(QRectF(side.left() + rollWidth * 0.10,
                                     side.top() + sideHeight * 0.12,
                                     rollWidth * 0.14,
                                     sideHeight * 0.42));

            const QRectF outer(rollCenter.x() - rollWidth / 2.0,
                               rollCenter.y() - topHeight / 2.0,
                               rollWidth,
                               topHeight);
            painter->setBrush(QColor(35, 75, 55));
            painter->drawEllipse(outer);

            const QRectF rice = outer.adjusted(rollWidth * 0.15,
                                               topHeight * 0.16,
                                               -rollWidth * 0.15,
                                               -topHeight * 0.16);
            painter->setBrush(QColor(252, 249, 240));
            painter->drawEllipse(rice);
            painter->setBrush(QColor(232, 226, 210));
            painter->drawEllipse(QRectF(rice.left() + rice.width() * 0.18,
                                        rice.top() + rice.height() * 0.16,
                                        rice.width() * 0.12,
                                        rice.height() * 0.10));

            if (cucumber && fish) {
                painter->setBrush(QColor(102, 190, 88));
                painter->drawEllipse(QRectF(rice.left() + rice.width() * 0.14,
                                            rice.top() + rice.height() * 0.26,
                                            rice.width() * 0.28,
                                            rice.height() * 0.44));
                painter->setBrush(QColor(248, 148, 126));
                painter->drawEllipse(QRectF(rice.left() + rice.width() * 0.52,
                                            rice.top() + rice.height() * 0.25,
                                            rice.width() * 0.30,
                                            rice.height() * 0.46));
            } else if (cucumber) {
                painter->setBrush(QColor(88, 178, 82));
                painter->drawEllipse(QRectF(rice.center().x() - rice.width() * 0.18,
                                            rice.center().y() - rice.height() * 0.22,
                                            rice.width() * 0.36,
                                            rice.height() * 0.44));
                painter->setBrush(QColor(178, 232, 132));
                painter->drawEllipse(QRectF(rice.center().x() - rice.width() * 0.07,
                                            rice.center().y() - rice.height() * 0.08,
                                            rice.width() * 0.14,
                                            rice.height() * 0.16));
            } else if (fish) {
                painter->setBrush(QColor(247, 150, 130));
                painter->drawRoundedRect(QRectF(rice.center().x() - rice.width() * 0.24,
                                               rice.center().y() - rice.height() * 0.20,
                                               rice.width() * 0.48,
                                               rice.height() * 0.40),
                                         rice.height() * 0.08,
                                         rice.height() * 0.08);
                painter->setBrush(QColor(255, 210, 188));
                painter->drawRect(QRectF(rice.center().x() - rice.width() * 0.06,
                                         rice.center().y() - rice.height() * 0.14,
                                         rice.width() * 0.12,
                                         rice.height() * 0.10));
            }

            painter->setBrush(QColor(255, 255, 255, 120));
            painter->drawEllipse(QRectF(outer.left() + rollWidth * 0.20,
                                        outer.top() + topHeight * 0.08,
                                        rollWidth * 0.22,
                                        topHeight * 0.12));
        };

        if (isFinishedSushi) {
            painter->setBrush(QColor(214, 207, 191, 150));
            painter->drawEllipse(QRectF(center.x() - plateWidth * 0.32,
                                        center.y() + plateHeight * 0.06,
                                        plateWidth * 0.64,
                                        plateHeight * 0.18));

            const qreal rollWidth = hasCucumber && hasFish ? plateWidth * 0.25 : plateWidth * 0.30;
            const qreal rollHeight = plateHeight * 0.47;
            if (hasCucumber && hasFish) {
                drawRoll(QPointF(center.x() - plateWidth * 0.21, center.y() - plateHeight * 0.03),
                         rollWidth,
                         rollHeight,
                         true,
                         false);
                drawRoll(QPointF(center.x(), center.y() + plateHeight * 0.06),
                         rollWidth,
                         rollHeight,
                         true,
                         true);
                drawRoll(QPointF(center.x() + plateWidth * 0.21, center.y() - plateHeight * 0.03),
                         rollWidth,
                         rollHeight,
                         false,
                         true);
            } else {
                drawRoll(QPointF(center.x() - plateWidth * 0.15, center.y() - plateHeight * 0.01),
                         rollWidth,
                         rollHeight,
                         hasCucumber,
                         hasFish);
                drawRoll(QPointF(center.x() + plateWidth * 0.18, center.y() + plateHeight * 0.05),
                         rollWidth,
                         rollHeight,
                         hasCucumber,
                         hasFish);
            }
            return;
        }

        if (hasNori) {
            painter->setBrush(QColor(39, 78, 58));
            painter->drawRoundedRect(QRectF(center.x() - plateWidth * 0.34,
                                           center.y() + plateHeight * 0.03,
                                           plateWidth * 0.68,
                                           plateHeight * 0.20),
                                     plateHeight * 0.07,
                                     plateHeight * 0.07);
        }
        if (hasRice) {
            painter->setBrush(QColor(250, 248, 242));
            painter->drawEllipse(QRectF(center.x() - plateWidth * 0.24,
                                        center.y() - plateHeight * 0.18,
                                        plateWidth * 0.48,
                                        plateHeight * 0.34));
            painter->setBrush(QColor(232, 226, 210));
            painter->drawEllipse(QRectF(center.x() + plateWidth * 0.02,
                                        center.y() - plateHeight * 0.14,
                                        plateWidth * 0.10,
                                        plateHeight * 0.12));
        }
        if (hasCucumber) {
            painter->setBrush(QColor(96, 176, 82));
            painter->drawEllipse(QRectF(center.x() - plateWidth * 0.24,
                                        center.y() - plateHeight * 0.09,
                                        plateWidth * 0.22,
                                        plateHeight * 0.20));
            painter->setBrush(QColor(176, 232, 132));
            painter->drawEllipse(QRectF(center.x() - plateWidth * 0.18,
                                        center.y() - plateHeight * 0.05,
                                        plateWidth * 0.09,
                                        plateHeight * 0.08));
        }
        if (hasFish) {
            painter->setBrush(QColor(248, 155, 136));
            painter->drawRoundedRect(QRectF(center.x() + plateWidth * 0.04,
                                           center.y() - plateHeight * 0.13,
                                           plateWidth * 0.27,
                                           plateHeight * 0.22),
                                     plateHeight * 0.07,
                                     plateHeight * 0.07);
            painter->setBrush(QColor(255, 207, 184));
            painter->drawRect(QRectF(center.x() + plateWidth * 0.10,
                                     center.y() - plateHeight * 0.10,
                                     plateWidth * 0.07,
                                     plateHeight * 0.06));
        }
        return;
    }

    if (item == DirtyPlateItem) {
        const QPointF center = rect.center();
        const qreal plateWidth = rect.width() * 0.92;
        const qreal plateHeight = qMax<qreal>(rect.height() * 0.48, plateWidth * 0.50);
        const QRectF plateRect(center.x() - plateWidth / 2.0,
                               center.y() - plateHeight / 2.0,
                               plateWidth,
                               plateHeight);
        painter->setPen(QPen(QColor(138, 130, 120), qMax<qreal>(1.0, rect.width() / 28.0)));
        painter->setBrush(QColor(246, 243, 232));
        painter->drawEllipse(plateRect);
        painter->setPen(QPen(QColor(207, 199, 187), qMax<qreal>(1.0, rect.width() / 34.0)));
        painter->setBrush(QColor(232, 228, 216));
        painter->drawEllipse(plateRect.adjusted(plateWidth * 0.20,
                                                plateHeight * 0.22,
                                                -plateWidth * 0.20,
                                                -plateHeight * 0.22));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(136, 91, 62));
        painter->drawEllipse(QRectF(center.x() - plateWidth * 0.16,
                                    center.y() - plateHeight * 0.13,
                                    plateWidth * 0.20,
                                    plateHeight * 0.16));
        painter->setBrush(QColor(104, 78, 58));
        painter->drawEllipse(QRectF(center.x() + plateWidth * 0.10,
                                    center.y() - plateHeight * 0.02,
                                    plateWidth * 0.13,
                                    plateHeight * 0.12));
        return;
    }

    painter->setPen(Qt::NoPen);
    const qreal px = qMax<qreal>(1.0, rect.width() / 12.0);
    const qreal left = rect.left();
    const qreal top = rect.top();
    const auto pixel = [&](int x, int y, const QColor &c, int w = 1, int h = 1) {
        painter->fillRect(QRectF(left + x * px, top + y * px, w * px, h * px), c);
    };

    const QColor outline(58, 46, 78);

    if (item == RiceItem) {
        pixel(3, 2, outline, 6, 1);
        pixel(2, 3, outline, 8, 1);
        pixel(1, 4, outline, 10, 3);
        pixel(2, 7, outline, 8, 2);
        pixel(3, 3, QColor(250, 247, 238), 6, 1);
        pixel(2, 4, QColor(232, 226, 206), 8, 2);
        pixel(3, 6, QColor(214, 190, 146), 6, 1);
        pixel(4, 4, QColor(255, 255, 252), 1, 1);
        pixel(7, 4, QColor(255, 255, 252), 1, 1);
        return;
    }

    if (item == CookedRiceItem) {
        pixel(2, 3, outline, 8, 1);
        pixel(2, 9, outline, 8, 1);
        pixel(1, 4, outline, 1, 5);
        pixel(10, 4, outline, 1, 5);
        pixel(3, 4, QColor(248, 246, 240), 6, 4);
        pixel(4, 3, QColor(255, 255, 255), 4, 1);
        pixel(3, 8, QColor(224, 217, 201), 6, 1);
        return;
    }

    if (item == NoriItem) {
        pixel(2, 2, outline, 8, 1);
        pixel(2, 9, outline, 8, 1);
        pixel(2, 3, outline, 1, 6);
        pixel(9, 3, outline, 1, 6);
        pixel(3, 3, QColor(39, 78, 58), 6, 6);
        pixel(4, 4, QColor(65, 128, 88), 3, 4);
        pixel(7, 3, QColor(28, 58, 45), 1, 6);
        return;
    }

    if (item == CucumberItem) {
        const QPointF center = rect.center();
        const QRectF cucumberRect(center.x() - rect.width() * 0.42,
                                  center.y() - rect.height() * 0.18,
                                  rect.width() * 0.84,
                                  rect.height() * 0.36);
        painter->setPen(QPen(QColor(92, 142, 68), qMax<qreal>(1.0, rect.width() / 28.0)));
        painter->setBrush(QColor(116, 190, 82));
        painter->drawEllipse(cucumberRect);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(71, 142, 64));
        painter->drawEllipse(cucumberRect.adjusted(0.0,
                                                   cucumberRect.height() * 0.28,
                                                   0.0,
                                                   0.0));
        painter->setBrush(QColor(170, 230, 122));
        painter->drawEllipse(QRectF(cucumberRect.left() + cucumberRect.width() * 0.18,
                                    cucumberRect.top() + cucumberRect.height() * 0.12,
                                    cucumberRect.width() * 0.46,
                                    cucumberRect.height() * 0.28));
        return;
    }

    if (item == ChoppedCucumberItem) {
        pixel(2, 4, QColor(111, 198, 101), 3, 2);
        pixel(7, 4, QColor(98, 182, 88), 3, 2);
        pixel(4, 7, QColor(86, 167, 79), 3, 2);
        pixel(2, 4, outline, 3, 1);
        pixel(7, 4, outline, 3, 1);
        pixel(4, 7, outline, 3, 1);
        return;
    }

    if (item == ShrimpItem) {
        pixel(3, 2, outline, 5, 1);
        pixel(2, 3, outline, 7, 1);
        pixel(1, 4, outline, 8, 1);
        pixel(1, 5, outline, 1, 3);
        pixel(8, 5, outline, 1, 3);
        pixel(2, 8, outline, 6, 1);
        pixel(3, 4, QColor(255, 203, 184), 4, 1);
        pixel(2, 5, QColor(255, 161, 142), 6, 2);
        pixel(3, 7, QColor(235, 112, 92), 4, 1);
        pixel(9, 4, QColor(255, 180, 152), 1, 1);
        pixel(0, 6, QColor(246, 132, 114), 1, 1);
        return;
    }

    if (item == ChoppedShrimpItem) {
        pixel(2, 4, QColor(255, 172, 152), 3, 2);
        pixel(7, 4, QColor(245, 146, 128), 3, 2);
        pixel(4, 7, QColor(235, 126, 110), 3, 2);
        pixel(2, 4, outline, 3, 1);
        pixel(7, 4, outline, 3, 1);
        pixel(4, 7, outline, 3, 1);
        return;
    }

    if (item == SushiBaseItem) {
        pixel(2, 3, outline, 8, 1);
        pixel(2, 9, outline, 8, 1);
        pixel(2, 4, outline, 1, 5);
        pixel(9, 4, outline, 1, 5);
        pixel(3, 4, QColor(248, 246, 240), 6, 2);
        pixel(3, 6, QColor(58, 98, 74), 6, 2);
        pixel(4, 3, QColor(255, 255, 255), 4, 1);
        return;
    }

    if (item == CucumberRollItem) {
        pixel(2, 3, outline, 8, 1);
        pixel(2, 9, outline, 8, 1);
        pixel(2, 4, outline, 1, 5);
        pixel(9, 4, outline, 1, 5);
        pixel(3, 4, QColor(246, 244, 238), 6, 4);
        pixel(3, 8, QColor(57, 95, 72), 6, 1);
        pixel(5, 5, QColor(112, 199, 97), 2, 2);
        return;
    }

    if (item == ShrimpRollItem) {
        pixel(2, 3, outline, 8, 1);
        pixel(2, 9, outline, 8, 1);
        pixel(2, 4, outline, 1, 5);
        pixel(9, 4, outline, 1, 5);
        pixel(3, 4, QColor(246, 244, 238), 6, 4);
        pixel(3, 8, QColor(57, 95, 72), 6, 1);
        pixel(5, 5, QColor(248, 155, 136), 2, 2);
        return;
    }

    if (item == CleanPlateItem) {
        pixel(2, 4, outline, 8, 1);
        pixel(1, 5, outline, 10, 3);
        pixel(2, 8, outline, 8, 1);
        pixel(3, 5, QColor(255, 255, 255), 6, 2);
        pixel(4, 6, QColor(224, 235, 244), 4, 1);
        return;
    }

    if (item == DirtyPlateItem) {
        pixel(2, 4, outline, 8, 1);
        pixel(1, 5, outline, 10, 3);
        pixel(2, 8, outline, 8, 1);
        pixel(3, 5, QColor(232, 229, 218), 6, 2);
        pixel(4, 5, QColor(138, 92, 64), 2, 1);
        pixel(7, 6, QColor(116, 78, 55), 1, 1);
        return;
    }

    if (item == BurntRiceItem) {
        pixel(2, 3, outline, 8, 1);
        pixel(1, 4, outline, 10, 5);
        pixel(2, 9, outline, 8, 1);
        pixel(3, 4, QColor(45, 42, 38), 6, 4);
        pixel(5, 5, QColor(82, 72, 58), 2, 1);
    }
}
