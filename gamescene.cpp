#include "GameScene.h"

#include <cmath>
#include <QColor>
#include <QCoreApplication>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QKeyEvent>
#include <QLineF>
#include <QLinearGradient>
#include <QPainterPath>
#include <QDir>
#include <QPainter>
#include <QPixmap>
#include <QtMath>

#include "DialogOverlayItem.h"
#include "GameTypes.h"
#include "HudOverlayItem.h"
#include "KitchenItem.h"
#include "MenuOverlayItem.h"
#include "PlayerItem.h"
#include "ResultOverlayItem.h"
#include "TutorialOverlayItem.h"

namespace {
QPixmap loadProjectPixmap(const QString &fileName)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir(appDir).absoluteFilePath(QStringLiteral("../") + fileName),
        QDir(appDir).absoluteFilePath(QStringLiteral("../../") + fileName),
        QDir::current().absoluteFilePath(fileName),
        QDir(QStringLiteral("C:/Users/Iriya/Desktop/Overcooked")).absoluteFilePath(fileName)
    };

    for (const QString &path : candidates) {
        QPixmap pixmap(path);
        if (!pixmap.isNull()) {
            return pixmap;
        }
    }

    return {};
}

class KitchenBackdropItem final : public QGraphicsItem
{
public:
    KitchenBackdropItem(QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
        , m_pixmap(loadProjectPixmap(QStringLiteral("厨房场景.png")))
        , m_targetRect(0.0, 0.0, GameScene::WindowWidth, GameScene::WindowHeight)
    {
        setZValue(0.0);
        setAcceptedMouseButtons(Qt::NoButton);
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        if (!m_pixmap.isNull()) {
            const QSizeF sourceSize = m_pixmap.size();
            const qreal scale = qMin(GameScene::WindowWidth / sourceSize.width(),
                                     GameScene::WindowHeight / sourceSize.height());
            const QSizeF targetSize(sourceSize.width() * scale, sourceSize.height() * scale);
            m_targetRect = QRectF((GameScene::WindowWidth - targetSize.width()) / 2.0,
                                  (GameScene::WindowHeight - targetSize.height()) / 2.0,
                                  targetSize.width(),
                                  targetSize.height());
        }
    }

    QRectF boundingRect() const override
    {
        return QRectF(0.0, 0.0, GameScene::WindowWidth, GameScene::WindowHeight);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
        painter->fillRect(boundingRect(), QColor(244, 225, 187));

        if (m_pixmap.isNull()) {
            painter->setPen(QPen(QColor(80, 70, 60), 2.0));
            painter->drawText(boundingRect(), Qt::AlignCenter, QStringLiteral("Missing 厨房场景.png"));
            return;
        }

        painter->drawPixmap(m_targetRect, m_pixmap, QRectF(QPointF(0.0, 0.0), m_pixmap.size()));
    }

private:
    QPixmap m_pixmap;
    QRectF m_targetRect;
};

class ModeledKitchenSceneItem final : public QGraphicsItem
{
public:
    explicit ModeledKitchenSceneItem(QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
    {
        setZValue(0.0);
        setAcceptedMouseButtons(Qt::NoButton);
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    }

    QRectF boundingRect() const override
    {
        return QRectF(0.0, 0.0, GameScene::WindowWidth, GameScene::WindowHeight);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::TextAntialiasing, true);
        painter->fillRect(boundingRect(), QColor(244, 226, 190));
        drawBackground(painter);
        drawFloor(painter);
        drawCounters(painter);
        drawStations(painter);
        drawDecor(painter);
    }

private:
    static void drawTextCentered(QPainter *painter, const QRectF &rect, const QString &text, int pointSize, const QColor &color)
    {
        QFont font = painter->font();
        font.setPointSize(pointSize);
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(color);
        painter->drawText(rect, Qt::AlignCenter, text);
    }

    static void drawPlate(QPainter *painter, const QPointF &center, qreal scale = 1.0)
    {
        painter->setPen(QPen(QColor(145, 137, 124), 1.5));
        painter->setBrush(QColor(246, 244, 235));
        painter->drawEllipse(QRectF(center.x() - 21.0 * scale, center.y() - 12.0 * scale, 42.0 * scale, 24.0 * scale));
        painter->setPen(QPen(QColor(198, 192, 182), 1.2));
        painter->setBrush(QColor(236, 232, 222));
        painter->drawEllipse(QRectF(center.x() - 13.0 * scale, center.y() - 7.0 * scale, 26.0 * scale, 14.0 * scale));
    }

    static void drawCounterRect(QPainter *painter, const QRectF &rect)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(94, 62, 44, 64));
        painter->drawRect(rect.translated(7.0, 8.0));

        QLinearGradient topGradient(rect.topLeft(), rect.bottomLeft());
        topGradient.setColorAt(0.0, QColor(255, 205, 118));
        topGradient.setColorAt(0.58, QColor(238, 174, 87));
        topGradient.setColorAt(1.0, QColor(207, 135, 67));
        painter->setPen(QPen(QColor(112, 81, 53), 1.3));
        painter->setBrush(topGradient);
        painter->drawRect(rect);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 231, 158, 110));
        painter->drawRect(QRectF(rect.left() + 4.0, rect.top() + 4.0, rect.width() - 8.0, 5.0));

        painter->setPen(QPen(QColor(183, 119, 62), 1.0));
        for (qreal x = rect.left() + 54.0; x < rect.right(); x += 54.0) {
            painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
        }
        if (rect.height() > rect.width() * 1.6) {
            for (qreal y = rect.top() + 64.0; y < rect.bottom() - 8.0; y += 64.0) {
                painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
            }
        }
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(196, 128, 66));
        painter->drawRect(QRectF(rect.left(), rect.bottom() - 10.0, rect.width(), 10.0));
        painter->setBrush(QColor(120, 82, 60));
        for (qreal x = rect.left() + 18.0; x < rect.right() - 14.0; x += 56.0) {
            painter->drawRoundedRect(QRectF(x, rect.bottom() - 17.0, 14.0, 4.0), 2.0, 2.0);
        }
    }

    static void drawCounterPoly(QPainter *painter, const QPolygonF &poly)
    {
        QPolygonF shadow = poly.translated(8.0, 10.0);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(94, 62, 44, 58));
        painter->drawPolygon(shadow);

        QPolygonF body = poly.translated(0.0, 24.0);
        painter->setBrush(QColor(190, 122, 58));
        painter->drawPolygon(body);
        for (int i = 0; i < poly.size(); ++i) {
            const QPointF a = poly.at(i);
            const QPointF b = poly.at((i + 1) % poly.size());
            const QPointF c = b + QPointF(0.0, 24.0);
            const QPointF d = a + QPointF(0.0, 24.0);
            if ((a.y() + b.y()) * 0.5 >= poly.boundingRect().center().y() - 2.0) {
                painter->setBrush(QColor(205, 139, 70));
                painter->drawPolygon(QPolygonF{a, b, c, d});
            }
        }

        painter->setPen(QPen(QColor(112, 81, 53), 1.3));
        painter->setBrush(QColor(238, 174, 87));
        painter->drawPolygon(poly);
        painter->setPen(QPen(QColor(255, 226, 146, 130), 2.0));
        if (poly.size() >= 2) {
            painter->drawLine(poly.at(0), poly.at(1));
        }
    }

    static void drawPlatePickupCounter(QPainter *painter)
    {
        const QRectF rect(360.0, 318.0, 76.0, 236.0);
        drawCounterRect(painter, rect);

        // The original level uses three stacked counter modules here:
        // plate, empty table space, plate.
        painter->setPen(QPen(QColor(141, 93, 52), 1.35));
        painter->drawLine(QPointF(rect.left(), rect.top() + 78.0), QPointF(rect.right(), rect.top() + 78.0));
        painter->drawLine(QPointF(rect.left(), rect.top() + 156.0), QPointF(rect.right(), rect.top() + 156.0));
    }

    static void drawIngredientCrate(QPainter *painter, const QRectF &rect, CarryItemType item, const QString &tag)
    {
        painter->setPen(QPen(QColor(104, 75, 52), 1.5));
        painter->setBrush(QColor(177, 126, 88));
        painter->drawRect(rect);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(222, 168, 120));
        painter->drawRect(rect.adjusted(7.0, 7.0, -7.0, -7.0));

        const QPointF plateCenter(rect.center().x(), rect.top() + 19.0);
        painter->setPen(QPen(QColor(142, 132, 118), 1.1));
        painter->setBrush(QColor(250, 248, 238));
        painter->drawEllipse(QRectF(plateCenter.x() - 17.0, plateCenter.y() - 11.0, 34.0, 22.0));
        painter->setPen(QPen(QColor(211, 204, 190), 1.0));
        painter->setBrush(QColor(236, 232, 220));
        painter->drawEllipse(QRectF(plateCenter.x() - 10.0, plateCenter.y() - 6.0, 20.0, 12.0));

        if (item == NoriItem) {
            painter->setPen(QPen(QColor(42, 72, 50), 1.5));
            painter->setBrush(QColor(58, 125, 82));
            QPolygonF nori;
            nori << QPointF(plateCenter.x() - 12.0, plateCenter.y() - 6.0)
                 << QPointF(plateCenter.x() + 12.0, plateCenter.y() - 9.0)
                 << QPointF(plateCenter.x() + 10.0, plateCenter.y() + 8.0)
                 << QPointF(plateCenter.x() - 12.0, plateCenter.y() + 9.0);
            painter->drawPolygon(nori);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(91, 164, 99));
            painter->drawPolygon(QPolygonF{QPointF(plateCenter.x() - 8.0, plateCenter.y() - 4.0),
                                           QPointF(plateCenter.x() + 2.0, plateCenter.y() - 5.0),
                                           QPointF(plateCenter.x() + 1.0, plateCenter.y() + 6.0),
                                           QPointF(plateCenter.x() - 8.0, plateCenter.y() + 7.0)});
            painter->setBrush(QColor(31, 72, 50));
            painter->drawPolygon(QPolygonF{QPointF(plateCenter.x() + 3.0, plateCenter.y() - 5.0),
                                           QPointF(plateCenter.x() + 10.0, plateCenter.y() - 7.0),
                                           QPointF(plateCenter.x() + 9.0, plateCenter.y() + 7.0),
                                           QPointF(plateCenter.x() + 3.0, plateCenter.y() + 7.0)});
        } else if (item == ShrimpItem) {
            painter->setPen(QPen(QColor(150, 103, 82), 1.2));
            painter->setBrush(QColor(235, 153, 128));
            painter->drawEllipse(QRectF(plateCenter.x() - 12.0, plateCenter.y() - 7.0, 24.0, 15.0));
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(255, 214, 190));
            painter->drawEllipse(QRectF(plateCenter.x() - 8.0, plateCenter.y() - 4.0, 8.0, 5.0));
            painter->setBrush(QColor(190, 88, 72));
            painter->drawEllipse(QRectF(plateCenter.x() + 5.0, plateCenter.y() - 1.0, 5.0, 4.0));
        } else if (item == RiceItem) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(225, 220, 203));
            for (int i = 0; i < 12; ++i) {
                const qreal x = plateCenter.x() - 11.0 + (i % 4) * 6.0;
                const qreal y = plateCenter.y() - 6.0 + (i / 4) * 4.0;
                painter->drawEllipse(QRectF(x, y, 4.0, 4.0));
            }
            painter->setBrush(QColor(255, 255, 252));
            painter->drawEllipse(QRectF(plateCenter.x() - 5.0, plateCenter.y() - 6.0, 5.0, 4.0));
            painter->drawEllipse(QRectF(plateCenter.x() + 5.0, plateCenter.y() - 1.0, 5.0, 4.0));
        } else {
            drawCarryItemIcon(painter, item, rect.adjusted(14.0, 8.0, -14.0, -16.0));
        }
        drawTextCentered(painter, QRectF(rect.left(), rect.bottom() - 16.0, rect.width(), 14.0), tag, 6, QColor(80, 55, 48));
    }

    static void drawDeliveryExit(QPainter *painter)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(80, 50, 38, 76));
        painter->drawRect(QRectF(54.0, 432.0, 142.0, 98.0).translated(7.0, 8.0));

        painter->setPen(QPen(QColor(106, 74, 52), 2.0));
        painter->setBrush(QColor(165, 116, 76));
        painter->drawRect(QRectF(54.0, 432.0, 142.0, 98.0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(220, 175, 112));
        painter->drawRect(QRectF(64.0, 442.0, 122.0, 78.0));

        painter->setPen(QPen(QColor(86, 66, 58), 2.0));
        painter->setBrush(QColor(245, 242, 226));
        painter->drawRect(QRectF(122.0, 438.0, 54.0, 82.0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 255, 255, 150));
        painter->drawRect(QRectF(128.0, 444.0, 42.0, 16.0));

        painter->setBrush(QColor(45, 43, 48));
        QPolygonF arrow;
        arrow << QPointF(150.0, 488.0) << QPointF(172.0, 474.0) << QPointF(172.0, 502.0);
        painter->drawPolygon(arrow);

        const auto drawLamp = [&](const QPointF &center) {
            painter->setPen(QPen(QColor(107, 53, 38), 2.0));
            painter->setBrush(QColor(198, 42, 36));
            painter->drawEllipse(QRectF(center.x() - 29.0, center.y() - 22.0, 58.0, 44.0));
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(232, 72, 54));
            painter->drawEllipse(QRectF(center.x() - 20.0, center.y() - 18.0, 32.0, 24.0));
            painter->setBrush(QColor(255, 178, 130, 170));
            painter->drawEllipse(QRectF(center.x() - 13.0, center.y() - 13.0, 12.0, 8.0));
            painter->setBrush(QColor(128, 72, 48));
            painter->drawRoundedRect(QRectF(center.x() - 18.0, center.y() + 14.0, 36.0, 8.0), 4.0, 4.0);
        };

        drawLamp(QPointF(92.0, 466.0));
        drawLamp(QPointF(92.0, 508.0));

        painter->setPen(QPen(QColor(90, 76, 62), 2.0));
        painter->drawLine(QPointF(58.0, 432.0), QPointF(58.0, 530.0));
        painter->drawLine(QPointF(190.0, 432.0), QPointF(190.0, 530.0));
    }

    static void drawBambooPlanter(QPainter *painter, const QPointF &base)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(56, 42, 36, 58));
        painter->drawEllipse(QRectF(base.x() - 34.0, base.y() + 31.0, 68.0, 18.0));

        painter->setPen(QPen(QColor(86, 57, 42), 1.4));
        painter->setBrush(QColor(104, 70, 50));
        QPolygonF potTop;
        potTop << QPointF(base.x() - 26.0, base.y() + 3.0)
               << QPointF(base.x() + 22.0, base.y() - 3.0)
               << QPointF(base.x() + 30.0, base.y() + 10.0)
               << QPointF(base.x() - 18.0, base.y() + 17.0);
        painter->drawPolygon(potTop);

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(70, 55, 42));
        painter->drawPolygon(QPolygonF{QPointF(base.x() - 15.0, base.y() + 6.0),
                                       QPointF(base.x() + 19.0, base.y() + 2.0),
                                       QPointF(base.x() + 22.0, base.y() + 8.0),
                                       QPointF(base.x() - 11.0, base.y() + 13.0)});

        painter->setPen(QPen(QColor(91, 61, 43), 1.2));
        painter->setBrush(QColor(156, 101, 65));
        QPolygonF potFront;
        potFront << QPointF(base.x() - 20.0, base.y() + 14.0)
                 << QPointF(base.x() + 29.0, base.y() + 8.0)
                 << QPointF(base.x() + 27.0, base.y() + 39.0)
                 << QPointF(base.x() - 22.0, base.y() + 39.0);
        painter->drawPolygon(potFront);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(115, 74, 53));
        painter->drawPolygon(QPolygonF{QPointF(base.x() + 12.0, base.y() + 11.0),
                                       QPointF(base.x() + 29.0, base.y() + 8.0),
                                       QPointF(base.x() + 27.0, base.y() + 39.0),
                                       QPointF(base.x() + 12.0, base.y() + 39.0)});
        painter->setBrush(QColor(196, 132, 82));
        painter->drawPolygon(QPolygonF{QPointF(base.x() - 15.0, base.y() + 17.0),
                                       QPointF(base.x() + 8.0, base.y() + 14.0),
                                       QPointF(base.x() + 8.0, base.y() + 21.0),
                                       QPointF(base.x() - 15.0, base.y() + 24.0)});

        const QColor stem(112, 205, 76);
        const QColor stemDark(70, 152, 64);
        const QList<qreal> xs = {-12.0, -4.0, 6.0, 14.0};
        for (int i = 0; i < xs.size(); ++i) {
            const qreal x = base.x() + xs.at(i);
            const qreal height = 42.0 + (i % 2) * 8.0;
            painter->setBrush(stemDark);
            painter->drawRect(QRectF(x - 2.0, base.y() + 2.0 - height, 7.0, height + 4.0));
            painter->setBrush(stem);
            painter->drawRect(QRectF(x, base.y() - height, 4.0, height));
            painter->setBrush(QColor(151, 230, 96));
            for (qreal y = base.y() - height + 10.0; y < base.y() - 8.0; y += 16.0) {
                painter->drawRect(QRectF(x - 2.0, y, 8.0, 3.0));
            }
            painter->setBrush(QColor(103, 190, 78));
            painter->drawEllipse(QRectF(x - 8.0, base.y() - height - 4.0, 14.0, 8.0));
            painter->drawEllipse(QRectF(x + 1.0, base.y() - height - 7.0, 14.0, 8.0));
        }
    }

    void drawBackground(QPainter *painter) const
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(232, 208, 168));
        painter->drawRect(QRectF(0.0, 0.0, GameScene::WindowWidth, 186.0));

        painter->setPen(QPen(QColor(205, 178, 138), 1.0));
        for (int y = 16; y < 178; y += 24) {
            painter->drawLine(QPointF(0.0, y), QPointF(GameScene::WindowWidth, y));
        }
        for (int x = 0; x < GameScene::WindowWidth; x += 64) {
            painter->drawLine(QPointF(x, 0.0), QPointF(x, 186.0));
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(39, 32, 38));
        painter->drawRect(QRectF(330.0, 76.0, 590.0, 42.0));
        painter->drawRect(QRectF(940.0, 76.0, 238.0, 42.0));
        painter->setBrush(QColor(142, 139, 135));
        painter->drawRect(QRectF(350.0, 118.0, 442.0, 64.0));
        painter->drawRect(QRectF(1015.0, 118.0, 150.0, 64.0));
        painter->setBrush(QColor(185, 86, 55));
        painter->drawRect(QRectF(330.0, 116.0, 608.0, 12.0));
        painter->drawRect(QRectF(988.0, 116.0, 190.0, 12.0));
        painter->setBrush(QColor(38, 35, 41));
        painter->drawRect(QRectF(735.0, 112.0, 102.0, 66.0));
        painter->setBrush(QColor(15, 30, 44));
        painter->drawRect(QRectF(1016.0, 92.0, 160.0, 56.0));
        drawTextCentered(painter, QRectF(1024.0, 110.0, 144.0, 22.0), QStringLiteral("NEGI SUSHI"), 11, QColor(31, 219, 255));
        painter->setBrush(QColor(105, 60, 70));
        for (int i = 0; i < 7; ++i) {
            painter->drawRect(QRectF(344.0 + i * 58.0, 96.0, 34.0, 22.0));
        }

        painter->setBrush(QColor(192, 138, 72));
        painter->drawRect(QRectF(48.0, 54.0, 48.0, 124.0));
        painter->setBrush(QColor(232, 180, 116));
        for (int i = 0; i < 5; ++i) {
            painter->drawRect(QRectF(56.0, 64.0 + i * 22.0, 32.0, 12.0));
        }
    }

    void drawFloor(QPainter *painter) const
    {
        const QRectF floorRect(0.0, 186.0, GameScene::WindowWidth, GameScene::WindowHeight - 186.0);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(191, 186, 174));
        painter->drawRect(floorRect);

        painter->setPen(Qt::NoPen);
        const qreal tile = 64.0;
        for (int row = 0; floorRect.top() + row * tile < floorRect.bottom(); ++row) {
            for (int col = 0; col * tile < floorRect.right(); ++col) {
                const QRectF tileRect(col * tile,
                                      floorRect.top() + row * tile,
                                      tile,
                                      tile);
                const QColor tileColor = ((row + col) % 2 == 0)
                                             ? QColor(198, 193, 181)
                                             : QColor(186, 181, 170);
                painter->setBrush(tileColor);
                painter->drawRect(tileRect);

                painter->setBrush(QColor(222, 218, 207, 54));
                painter->drawRect(tileRect.adjusted(8.0, 8.0, -44.0, -44.0));
                painter->setBrush(QColor(126, 119, 108, 38));
                painter->drawRect(tileRect.adjusted(48.0, 48.0, -10.0, -10.0));
            }
        }

        painter->setPen(QPen(QColor(132, 126, 116), 1.0));
        for (qreal x = 0.0; x <= GameScene::WindowWidth; x += tile) {
            painter->drawLine(QPointF(x, floorRect.top()), QPointF(x, floorRect.bottom()));
        }
        for (qreal y = floorRect.top(); y <= floorRect.bottom(); y += tile) {
            painter->drawLine(QPointF(0.0, y), QPointF(GameScene::WindowWidth, y));
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(255, 255, 255, 36));
        painter->drawEllipse(QRectF(438.0, 292.0, 76.0, 28.0));
        painter->drawEllipse(QRectF(818.0, 318.0, 94.0, 30.0));
    }

    void drawCounters(QPainter *painter) const
    {
        drawCounterRect(painter, QRectF(64.0, 214.0, 92.0, 318.0));
        drawCounterRect(painter, QRectF(156.0, 182.0, 486.0, 56.0));
        drawCounterRect(painter, QRectF(154.0, 250.0, 70.0, 190.0));
        drawPlatePickupCounter(painter);
        drawCounterRect(painter, QRectF(574.0, 182.0, 68.0, 426.0));
        drawCounterRect(painter, QRectF(758.0, 146.0, 266.0, 50.0));
        drawCounterRect(painter, QRectF(1012.0, 204.0, 72.0, 270.0));
        drawCounterRect(painter, QRectF(780.0, 498.0, 258.0, 80.0));
    }

    void drawStations(QPainter *painter) const
    {
        painter->setPen(QPen(QColor(110, 88, 68), 1.3));
        painter->setBrush(QColor(245, 240, 230));
        for (const QPointF &p : QList<QPointF>{QPointF(286.0, 210.0), QPointF(426.0, 210.0), QPointF(566.0, 210.0)}) {
            painter->setBrush(QColor(128, 84, 56, 60));
            painter->drawRoundedRect(QRectF(p.x() - 26.0, p.y() - 13.0, 52.0, 34.0).translated(4.0, 5.0), 4.0, 4.0);
            painter->setBrush(QColor(245, 240, 230));
            painter->drawRoundedRect(QRectF(p.x() - 28.0, p.y() - 17.0, 56.0, 34.0), 5.0, 5.0);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(255, 255, 255, 180));
            painter->drawRect(QRectF(p.x() - 21.0, p.y() - 11.0, 36.0, 4.0));
            painter->setPen(QPen(QColor(96, 82, 70), 1.2));
            painter->setBrush(QColor(246, 244, 236));
            QPolygonF blade;
            blade << QPointF(p.x() - 16.0, p.y() - 13.0)
                  << QPointF(p.x() + 7.0, p.y() - 7.0)
                  << QPointF(p.x() + 9.0, p.y() - 3.0)
                  << QPointF(p.x() + 4.0, p.y() + 3.0)
                  << QPointF(p.x() - 17.0, p.y() - 3.0)
                  << QPointF(p.x() - 20.0, p.y() - 8.0);
            painter->drawPolygon(blade);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(214, 214, 208));
            painter->drawPolygon(QPolygonF{QPointF(p.x() - 17.0, p.y() - 5.0),
                                           QPointF(p.x() + 4.0, p.y() - 1.0),
                                           QPointF(p.x() + 2.0, p.y() + 2.0),
                                           QPointF(p.x() - 17.0, p.y() - 2.0)});
            painter->setPen(QPen(QColor(84, 58, 44), 1.1));
            painter->setBrush(QColor(88, 60, 44));
            QPolygonF handle;
            handle << QPointF(p.x() + 7.0, p.y() - 6.0)
                   << QPointF(p.x() + 24.0, p.y() - 2.0)
                   << QPointF(p.x() + 22.0, p.y() + 5.0)
                   << QPointF(p.x() + 5.0, p.y() + 1.0);
            painter->drawPolygon(handle);
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(255, 255, 255, 155));
            painter->drawPolygon(QPolygonF{QPointF(p.x() - 13.0, p.y() - 11.0),
                                           QPointF(p.x() + 2.0, p.y() - 7.0),
                                           QPointF(p.x() + 1.0, p.y() - 5.0),
                                           QPointF(p.x() - 14.0, p.y() - 9.0)});
            painter->setPen(QPen(QColor(110, 88, 68), 1.3));
            painter->setBrush(QColor(245, 240, 230));
        }

        drawPlate(painter, QPointF(398.0, 356.0), 1.22);
        drawPlate(painter, QPointF(398.0, 512.0), 1.22);
        drawPlate(painter, QPointF(110.0, 266.0));
        drawPlate(painter, QPointF(1048.0, 266.0), 1.22);
        drawPlate(painter, QPointF(1048.0, 410.0), 1.22);

        painter->setPen(QPen(QColor(100, 92, 84), 1.4));
        painter->setBrush(QColor(113, 117, 125));
        for (const QPointF &p : QList<QPointF>{QPointF(808.0, 142.0), QPointF(874.0, 142.0), QPointF(940.0, 142.0)}) {
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(68, 52, 46, 75));
            painter->drawEllipse(QRectF(p.x() - 25.0, p.y() + 22.0, 50.0, 16.0));
            painter->setPen(QPen(QColor(100, 92, 84), 1.4));
            painter->setBrush(QColor(113, 117, 125));
            painter->drawEllipse(QRectF(p.x() - 22.0, p.y() - 10.0, 44.0, 20.0));
            painter->drawRect(QRectF(p.x() - 18.0, p.y() - 2.0, 36.0, 32.0));
            painter->setBrush(QColor(162, 169, 176));
            painter->drawEllipse(QRectF(p.x() - 17.0, p.y() - 8.0, 34.0, 14.0));
            painter->setBrush(QColor(230, 79, 48));
            painter->drawRect(QRectF(p.x() - 26.0, p.y() + 26.0, 52.0, 8.0));
            painter->setBrush(QColor(255, 218, 92));
            painter->drawRect(QRectF(p.x() - 9.0, p.y() + 27.0, 18.0, 5.0));
            painter->setBrush(QColor(113, 117, 125));
        }

        painter->setPen(QPen(QColor(100, 82, 66), 1.3));
        painter->setBrush(QColor(223, 205, 177));
        painter->drawRect(QRectF(786.0, 500.0, 118.0, 58.0));
        painter->drawRect(QRectF(904.0, 500.0, 64.0, 58.0));
        painter->drawRect(QRectF(968.0, 500.0, 70.0, 58.0));
        painter->setPen(QPen(QColor(164, 141, 112), 1.0));
        painter->drawLine(QPointF(904.0, 500.0), QPointF(904.0, 558.0));
        painter->drawLine(QPointF(968.0, 500.0), QPointF(968.0, 558.0));
        painter->setBrush(QColor(182, 224, 236));
        painter->drawRect(QRectF(848.0, 506.0, 50.0, 34.0));
        painter->setBrush(QColor(190, 194, 196));
        painter->drawRect(QRectF(798.0, 504.0, 34.0, 38.0));
        painter->setBrush(QColor(255, 255, 255, 120));
        painter->drawRect(QRectF(852.0, 510.0, 30.0, 6.0));
        painter->setPen(QPen(QColor(86, 91, 94), 2.2));
        painter->setBrush(QColor(185, 193, 196));
        painter->drawRoundedRect(QRectF(822.0, 498.0, 18.0, 7.0), 2.0, 2.0);
        painter->drawRect(QRectF(829.0, 482.0, 5.0, 20.0));
        QPainterPath faucet;
        faucet.moveTo(832.0, 484.0);
        faucet.cubicTo(846.0, 478.0, 856.0, 484.0, 856.0, 496.0);
        painter->setPen(QPen(QColor(86, 91, 94), 4.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(faucet);
        painter->setPen(QPen(QColor(230, 237, 239), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(faucet);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(113, 190, 220, 180));
        painter->drawRect(QRectF(854.0, 496.0, 4.0, 10.0));
        painter->setPen(QPen(QColor(150, 130, 114), 1.2));
        painter->setBrush(QColor(245, 242, 232));
        painter->drawEllipse(QRectF(988.0, 518.0, 36.0, 20.0));
        painter->setBrush(QColor(232, 226, 214));
        painter->drawEllipse(QRectF(996.0, 523.0, 20.0, 10.0));
        drawTextCentered(painter, QRectF(976.0, 540.0, 58.0, 12.0), QStringLiteral("DIRTY"), 6, QColor(92, 68, 58));

        drawIngredientCrate(painter, QRectF(160.0, 254.0, 54.0, 38.0), NoriItem, QStringLiteral("NORI"));
        drawIngredientCrate(painter, QRectF(160.0, 312.0, 54.0, 38.0), ShrimpItem, QStringLiteral("FISH"));
        drawIngredientCrate(painter, QRectF(160.0, 370.0, 54.0, 40.0), RiceItem, QStringLiteral("RICE"));
        painter->setPen(QPen(QColor(104, 75, 52), 1.5));
        painter->setBrush(QColor(177, 126, 88));
        painter->drawRect(QRectF(1016.0, 316.0, 62.0, 44.0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(222, 168, 120));
        painter->drawRect(QRectF(1023.0, 323.0, 48.0, 30.0));
        painter->setPen(QPen(QColor(92, 142, 68), 1.4));
        painter->setBrush(QColor(116, 190, 82));
        painter->drawEllipse(QRectF(1029.0, 331.0, 36.0, 16.0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(71, 142, 64));
        painter->drawEllipse(QRectF(1029.0, 335.0, 36.0, 8.0));
        painter->setBrush(QColor(170, 230, 122));
        painter->drawEllipse(QRectF(1035.0, 331.0, 17.0, 5.0));
        drawTextCentered(painter, QRectF(1016.0, 344.0, 62.0, 14.0), QStringLiteral("KYURI"), 6, QColor(80, 55, 48));

        painter->setPen(QPen(QColor(64, 47, 55), 1.5));
        painter->setBrush(QColor(255, 245, 224));
        painter->drawRoundedRect(QRectF(1018.0, 244.0, 60.0, 36.0), 8.0, 8.0);
        drawTextCentered(painter, QRectF(1020.0, 250.0, 56.0, 16.0), QStringLiteral("PLATES"), 7, QColor(78, 58, 70));
    }

    void drawDecor(QPainter *painter) const
    {
        painter->setPen(QPen(QColor(86, 78, 70), 1.3));
        painter->setBrush(QColor(121, 112, 102));
        painter->drawRect(QRectF(850.0, 338.0, 48.0, 32.0));
        for (int i = 0; i < 6; ++i) {
            painter->drawLine(QPointF(856.0 + i * 7.0, 342.0), QPointF(856.0 + i * 7.0, 366.0));
        }
        drawBambooPlanter(painter, QPointF(336.0, 570.0));
        drawBambooPlanter(painter, QPointF(828.0, 620.0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(221, 78, 50));
        painter->drawRect(QRectF(806.0, 268.0, 22.0, 42.0));

        drawDeliveryExit(painter);

        painter->setPen(QPen(QColor(255, 255, 255, 120), 1.2));
        painter->drawLine(QPointF(1018.0, 96.0), QPointF(1172.0, 96.0));
        painter->drawLine(QPointF(1018.0, 146.0), QPointF(1172.0, 146.0));
        painter->setPen(QPen(QColor(48, 34, 44), 1.2));
        painter->setBrush(QColor(244, 202, 118));
        painter->drawRoundedRect(QRectF(664.0, 62.0, 26.0, 34.0), 8.0, 8.0);
        painter->drawRoundedRect(QRectF(704.0, 60.0, 26.0, 36.0), 8.0, 8.0);
        painter->drawRoundedRect(QRectF(744.0, 62.0, 26.0, 34.0), 8.0, 8.0);
    }
};

class GroundGridItem final : public QGraphicsItem
{
public:
    GroundGridItem(int cols, int rows, qreal tileWidth, qreal tileHeight, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
        , m_cols(cols)
        , m_rows(rows)
        , m_tileWidth(tileWidth)
        , m_tileHeight(tileHeight)
        , m_totalWidth((m_cols + m_rows) * (m_tileWidth / 2.0))
        , m_totalHeight((m_cols + m_rows) * (m_tileHeight / 2.0))
    {
        setZValue(0.0);
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        setAcceptedMouseButtons(Qt::NoButton);
    }

    QRectF boundingRect() const override
    {
        return QRectF(0.0, 0.0, m_totalWidth, m_totalHeight);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, false);

        QPen borderPen(QColor(112, 104, 95), 1.0);
        borderPen.setCosmetic(true);
        painter->setPen(borderPen);

        const QColor lightTile(201, 194, 184);
        const QColor darkTile(190, 183, 173);
        const QColor rugTile(108, 57, 67);
        const QColor rugTileAlt(123, 66, 77);

        for (int row = 0; row < m_rows; ++row) {
            for (int col = 0; col < m_cols; ++col) {
                const QPointF top = tileTop(col, row);
                const bool rug = isRugTile(col, row);
                const QColor fillColor = rug
                                             ? (((col + row) % 2 == 0) ? rugTile : rugTileAlt)
                                             : (((col + row) % 2 == 0) ? lightTile : darkTile);

                QPolygonF diamond;
                diamond << QPointF(top.x(), top.y())
                        << QPointF(top.x() + m_tileWidth / 2.0, top.y() + m_tileHeight / 2.0)
                        << QPointF(top.x(), top.y() + m_tileHeight)
                        << QPointF(top.x() - m_tileWidth / 2.0, top.y() + m_tileHeight / 2.0);

                painter->setBrush(fillColor);
                painter->drawPolygon(diamond);
            }
        }
    }

private:
    QPointF tileTop(int col, int row) const
    {
        const qreal x = (col - row) * (m_tileWidth / 2.0) + (m_totalWidth / 2.0);
        const qreal y = (col + row) * (m_tileHeight / 2.0);
        return QPointF(x, y);
    }

    bool isRugTile(int col, int row) const
    {
        return col == 6 && row >= 1 && row <= 3;
    }

    int m_cols;
    int m_rows;
    qreal m_tileWidth;
    qreal m_tileHeight;
    qreal m_totalWidth;
    qreal m_totalHeight;
};

class GridItem : public QGraphicsItem
{
public:
    explicit GridItem(qreal zValue, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
    {
        setPos(sceneGridOffset());
        setZValue(zValue);
        setAcceptedMouseButtons(Qt::NoButton);
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    }

    QRectF boundingRect() const override
    {
        return QRectF(-GameScene::BASE_GRID_WIDTH * 3,
                      -GameScene::BASE_GRID_HEIGHT * 8,
                      sceneGridWidth() + GameScene::BASE_GRID_WIDTH * 6,
                      sceneGridHeight() + GameScene::BASE_GRID_HEIGHT * 12);
    }

protected:
    static int sceneGridWidth()
    {
        return (GameScene::SCENE_COLS + GameScene::SCENE_ROWS) * (GameScene::BASE_GRID_WIDTH / 2);
    }

    static int sceneGridHeight()
    {
        return (GameScene::SCENE_COLS + GameScene::SCENE_ROWS) * (GameScene::BASE_GRID_HEIGHT / 2);
    }

    static QPoint sceneGridOffset()
    {
        return QPoint((GameScene::WindowWidth - sceneGridWidth()) / 2,
                      (GameScene::WindowHeight - sceneGridHeight()) / 2);
    }

    QPoint tileTop(int col, int row) const
    {
        const int x = (col - row) * (GameScene::BASE_GRID_WIDTH / 2) + sceneGridWidth() / 2;
        const int y = (col + row) * (GameScene::BASE_GRID_HEIGHT / 2);
        return QPoint(x, y);
    }

    QPoint tileCenter(int col, int row) const
    {
        const QPoint top = tileTop(col, row);
        return QPoint(top.x(), top.y() + GameScene::BASE_GRID_HEIGHT / 2);
    }

    QPolygon diamond(int col, int row) const
    {
        const QPoint top = tileTop(col, row);
        const int halfW = GameScene::BASE_GRID_WIDTH / 2;
        const int halfH = GameScene::BASE_GRID_HEIGHT / 2;
        QPolygon poly;
        poly << top
             << QPoint(top.x() + halfW, top.y() + halfH)
             << QPoint(top.x(), top.y() + GameScene::BASE_GRID_HEIGHT)
             << QPoint(top.x() - halfW, top.y() + halfH);
        return poly;
    }

    void drawFlatTile(QPainter *painter, int col, int row, const QColor &fill, const QColor &border) const
    {
        QPen pen(border, 1.0);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->setBrush(fill);
        painter->drawPolygon(diamond(col, row));
    }

    void drawRaisedTile(QPainter *painter,
                        int col,
                        int row,
                        const QColor &topColor,
                        const QColor &leftColor,
                        const QColor &rightColor,
                        int depth = 18) const
    {
        const QPolygon topPoly = diamond(col, row);
        const QPoint top = topPoly.at(0);
        const QPoint right = topPoly.at(1);
        const QPoint bottom = topPoly.at(2);
        const QPoint left = topPoly.at(3);

        QPen pen(QColor(88, 71, 60), 1.0);
        pen.setCosmetic(true);
        painter->setPen(pen);

        painter->setBrush(leftColor);
        QPolygon leftFace;
        leftFace << left
                 << bottom
                 << QPoint(bottom.x(), bottom.y() + depth)
                 << QPoint(left.x(), left.y() + depth);
        painter->drawPolygon(leftFace);

        painter->setBrush(rightColor);
        QPolygon rightFace;
        rightFace << bottom
                  << right
                  << QPoint(right.x(), right.y() + depth)
                  << QPoint(bottom.x(), bottom.y() + depth);
        painter->drawPolygon(rightFace);

        painter->setBrush(topColor);
        painter->drawPolygon(topPoly);
    }
};

class StaticElementItem final : public GridItem
{
public:
    enum Kind {
        PlantersKind,
        DrainKind,
        RugEmblemKind,
        BackdropKind,
        LeftServiceZoneKind,
        LeftPrepCountersKind,
        MiddleDividerKind,
        StoveZoneKind,
        SinkZoneKind,
        RightShelfKind,
        SignageKind,
        CustomerWindowKind,
        ServiceMarkerKind,
        LeftCounterItemsKind,
        StoveItemsKind,
        RightShelfItemsKind
    };

    StaticElementItem(Kind kind, qreal zValue, QGraphicsItem *parent = nullptr)
        : GridItem(zValue, parent)
        , m_kind(kind)
    {
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, false);

        switch (m_kind) {
        case PlantersKind:
            drawPlanters(painter);
            break;
        case DrainKind:
            drawDrain(painter);
            break;
        case RugEmblemKind:
            drawRugEmblem(painter);
            break;
        case BackdropKind:
            drawBackdrop(painter);
            break;
        case LeftServiceZoneKind:
            drawLeftServiceZone(painter);
            break;
        case LeftPrepCountersKind:
            drawLeftPrepCounters(painter);
            break;
        case MiddleDividerKind:
            drawMiddleDivider(painter);
            break;
        case StoveZoneKind:
            drawStoveZone(painter);
            break;
        case SinkZoneKind:
            drawSinkZone(painter);
            break;
        case RightShelfKind:
            drawRightShelf(painter);
            break;
        case SignageKind:
            drawSignage(painter);
            break;
        case CustomerWindowKind:
            drawCustomerWindow(painter);
            break;
        case ServiceMarkerKind:
            drawServiceMarker(painter);
            break;
        case LeftCounterItemsKind:
            drawLeftCounterItems(painter);
            break;
        case StoveItemsKind:
            drawStoveItems(painter);
            break;
        case RightShelfItemsKind:
            drawRightShelfItems(painter);
            break;
        }
    }

private:
    void drawPlanterAt(QPainter *painter, int col, int row) const
    {
        const QPoint center = tileCenter(col, row);
        painter->setPen(QPen(QColor(95, 70, 58), 1.0));
        painter->setBrush(QColor(133, 95, 71));
        painter->drawRect(QRect(center.x() - 14, center.y() - 6, 28, 18));

        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(133, 199, 74));
        painter->drawRect(QRect(center.x() - 10, center.y() - 28, 5, 22));
        painter->drawRect(QRect(center.x() - 2, center.y() - 34, 5, 28));
        painter->drawRect(QRect(center.x() + 6, center.y() - 26, 5, 20));
    }

    void drawPlate(QPainter *painter, const QPoint &center) const
    {
        painter->setPen(QPen(QColor(156, 146, 135), 1.0));
        painter->setBrush(QColor(244, 240, 232));
        painter->drawEllipse(QRect(center.x() - 18, center.y() - 11, 36, 22));
        painter->setBrush(QColor(233, 229, 221));
        painter->drawEllipse(QRect(center.x() - 12, center.y() - 6, 24, 12));
    }

    void drawCuttingBoardWithKnife(QPainter *painter, int col, int row) const
    {
        const QPoint center = tileCenter(col, row);
        painter->setPen(QPen(QColor(142, 126, 110), 1.0));
        painter->setBrush(QColor(242, 239, 232));
        painter->drawRect(QRect(center.x() - 18, center.y() - 13, 36, 18));

        painter->setPen(QPen(QColor(88, 71, 60), 1.0));
        painter->setBrush(QColor(191, 197, 204));
        painter->drawRect(QRect(center.x() - 2, center.y() - 11, 14, 4));
        painter->setBrush(QColor(120, 71, 46));
        painter->drawRect(QRect(center.x() + 10, center.y() - 12, 4, 8));
    }

    void drawPlanters(QPainter *painter) const
    {
        drawPlanterAt(painter, 2, 6);
        drawPlanterAt(painter, 9, 6);
    }

    void drawDrain(QPainter *painter) const
    {
        const QPoint center = tileCenter(9, 3);
        painter->setPen(QPen(QColor(96, 90, 82), 1.0));
        painter->setBrush(QColor(130, 123, 115));
        painter->drawRect(QRect(center.x() - 16, center.y() - 10, 32, 20));
        painter->setPen(QPen(QColor(171, 164, 154), 1.0));
        for (int i = -10; i <= 10; i += 5) {
            painter->drawLine(center.x() + i, center.y() - 8, center.x() + i, center.y() + 8);
        }
    }

    void drawRugEmblem(QPainter *painter) const
    {
        const QPoint center = tileCenter(6, 2);
        painter->setPen(QPen(QColor(86, 42, 52), 1.0));
        painter->setBrush(Qt::NoBrush);
        QPolygon onion;
        onion << QPoint(center.x(), center.y() - 14)
              << QPoint(center.x() + 10, center.y() - 2)
              << QPoint(center.x() + 6, center.y() + 12)
              << QPoint(center.x(), center.y() + 18)
              << QPoint(center.x() - 6, center.y() + 12)
              << QPoint(center.x() - 10, center.y() - 2);
        painter->drawPolygon(onion);
    }

    void drawBackdrop(QPainter *painter) const
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(42, 36, 42));
        painter->drawRect(QRect(108, -8, 548, 42));
        painter->drawRect(QRect(694, -8, 244, 42));

        painter->setBrush(QColor(157, 69, 92, 150));
        for (int i = 0; i < 7; ++i) {
            painter->drawRect(QRect(122 + i * 58, 10, 32, 18));
        }

        painter->setBrush(QColor(143, 141, 141));
        painter->drawRect(QRect(124, 36, 406, 52));
        painter->drawRect(QRect(760, 36, 228, 52));

        painter->setBrush(QColor(187, 94, 66));
        painter->drawRect(QRect(112, 26, 552, 10));
        painter->drawRect(QRect(744, 26, 260, 10));
    }

    void drawLeftServiceZone(QPainter *painter) const
    {
        painter->setPen(QPen(QColor(92, 73, 58), 1.0));

        painter->setBrush(QColor(236, 233, 225));
        painter->drawRect(QRect(68, 78, 28, 198));
        painter->setBrush(QColor(246, 243, 235));
        painter->drawRect(QRect(30, 128, 42, 168));

        painter->setBrush(QColor(121, 88, 60));
        painter->drawRect(QRect(82, 108, 64, 120));
        painter->setBrush(QColor(167, 121, 82));
        painter->drawRect(QRect(88, 114, 52, 108));

        painter->setBrush(QColor(203, 56, 44));
        painter->drawEllipse(QRect(38, 234, 34, 26));
        painter->drawEllipse(QRect(44, 252, 30, 24));
    }

    void drawLeftPrepCounters(QPainter *painter) const
    {
        for (int col = 1; col <= 4; ++col) {
            drawRaisedTile(painter, col, 1, QColor(246, 197, 114), QColor(181, 126, 73), QColor(198, 144, 87));
        }
    }

    void drawMiddleDivider(QPainter *painter) const
    {
        for (int row = 1; row <= 5; ++row) {
            drawRaisedTile(painter, 5, row, QColor(243, 196, 117), QColor(180, 123, 72), QColor(195, 141, 86));
        }
        drawRaisedTile(painter, 5, 5, QColor(134, 182, 106), QColor(88, 120, 74), QColor(105, 143, 88), 8);
    }

    void drawStoveZone(QPainter *painter) const
    {
        for (int col = 8; col <= 10; ++col) {
            drawRaisedTile(painter, col, 1, QColor(245, 198, 120), QColor(184, 127, 76), QColor(199, 143, 87));
            const QPoint center = tileCenter(col, 1);
            painter->setPen(QPen(QColor(64, 60, 63), 1.0));
            painter->setBrush(QColor(72, 70, 74));
            painter->drawRect(QRect(center.x() - 22, center.y() - 14, 44, 14));
            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor(228, 92, 59));
            painter->drawRect(QRect(center.x() - 18, center.y() - 4, 36, 4));
        }
    }

    void drawSinkZone(QPainter *painter) const
    {
        drawRaisedTile(painter, 8, 5, QColor(214, 198, 171), QColor(149, 128, 99), QColor(166, 145, 115));
        drawRaisedTile(painter, 9, 5, QColor(214, 198, 171), QColor(149, 128, 99), QColor(166, 145, 115));

        const QPoint center = tileCenter(9, 5);
        painter->setPen(QPen(QColor(88, 82, 82), 1.0));
        painter->setBrush(QColor(184, 227, 237));
        painter->drawRect(QRect(center.x() - 16, center.y() - 10, 34, 18));
        painter->setBrush(QColor(190, 195, 200));
        painter->drawRect(QRect(center.x() - 28, center.y() - 16, 18, 22));
        painter->drawRect(QRect(center.x() - 16, center.y() - 16, 14, 4));
    }

    void drawRightShelf(QPainter *painter) const
    {
        for (int row = 2; row <= 4; ++row) {
            drawRaisedTile(painter, 11, row, QColor(243, 196, 117), QColor(180, 123, 72), QColor(195, 141, 86));
        }
    }

    void drawSignage(QPainter *painter) const
    {
        painter->setPen(QPen(QColor(78, 70, 80), 1.0));
        painter->setBrush(QColor(36, 35, 40));
        painter->drawRect(QRect(560, 20, 82, 58));

        painter->setBrush(QColor(20, 32, 46));
        painter->drawRect(QRect(760, 6, 172, 52));
        painter->setPen(QPen(QColor(27, 212, 255), 1.0));
        painter->drawText(QRect(774, 18, 150, 28), Qt::AlignCenter, QStringLiteral("NEGI SUSHI"));
    }

    void drawCustomerWindow(QPainter *painter) const
    {
        painter->setPen(QPen(QColor(97, 76, 61), 1.0));
        painter->setBrush(QColor(226, 211, 188));
        painter->drawRect(QRect(184, 40, 348, 18));
        painter->drawRect(QRect(784, 40, 176, 18));
    }

    void drawServiceMarker(QPainter *painter) const
    {
        const QPoint markerTop = tileCenter(11, 3);
        painter->setPen(QPen(QColor(241, 239, 232), 1.0));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QRect(markerTop.x() + 16, markerTop.y() - 42, 26, 26));
        painter->drawEllipse(QRect(markerTop.x() + 42, markerTop.y() - 26, 26, 26));
    }

    void drawLeftCounterItems(QPainter *painter) const
    {
        drawCuttingBoardWithKnife(painter, 2, 1);
        drawCuttingBoardWithKnife(painter, 3, 1);
        drawCuttingBoardWithKnife(painter, 4, 1);

        drawPlate(painter, tileCenter(2, 4));
        drawPlate(painter, tileCenter(2, 5));

        const QPoint riceCenter = tileCenter(0, 1);
        painter->setPen(QPen(QColor(145, 133, 114), 1.0));
        painter->setBrush(QColor(224, 221, 209));
        painter->drawEllipse(QRect(riceCenter.x() - 16, riceCenter.y() - 10, 30, 18));

        const QPoint shrimpCenter = tileCenter(0, 2);
        painter->setBrush(QColor(228, 165, 135));
        painter->drawEllipse(QRect(shrimpCenter.x() - 14, shrimpCenter.y() - 8, 26, 16));

        const QPoint noriCenter = tileCenter(0, 3);
        painter->setBrush(QColor(77, 141, 93));
        painter->drawRect(QRect(noriCenter.x() - 12, noriCenter.y() - 8, 22, 14));
    }

    void drawStoveItems(QPainter *painter) const
    {
        for (int col = 8; col <= 10; ++col) {
            const QPoint center = tileCenter(col, 1);
            painter->setPen(QPen(QColor(108, 110, 118), 1.0));
            painter->setBrush(QColor(120, 123, 131));
            painter->drawEllipse(QRect(center.x() - 15, center.y() - 28, 30, 18));
            painter->drawRect(QRect(center.x() - 12, center.y() - 18, 24, 18));

            painter->setPen(QPen(QColor(248, 245, 240), 1.0));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(QRect(center.x() - 10, center.y() - 44, 20, 20));
            painter->drawLine(center.x() - 5, center.y() - 34, center.x() + 5, center.y() - 34);
            painter->drawLine(center.x(), center.y() - 39, center.x(), center.y() - 29);
        }

        const QPoint bottleCenter = tileCenter(7, 1);
        painter->setPen(QPen(QColor(144, 63, 55), 1.0));
        painter->setBrush(QColor(184, 70, 58));
        painter->drawRect(QRect(bottleCenter.x() - 8, bottleCenter.y() - 22, 16, 22));
    }

    void drawRightShelfItems(QPainter *painter) const
    {
        drawPlate(painter, tileCenter(11, 2));
        drawPlate(painter, tileCenter(11, 4));

        const QPoint cucumberCenter = tileCenter(11, 3);
        painter->setPen(QPen(QColor(77, 131, 75), 1.0));
        painter->setBrush(QColor(111, 183, 88));
        painter->drawEllipse(QRect(cucumberCenter.x() - 12, cucumberCenter.y() - 8, 24, 14));
    }

    Kind m_kind;
};

QSizeF tableIconSize(CarryItemType item);

class DynamicEffectsItem final : public QGraphicsItem
{
public:
    DynamicEffectsItem(const qreal *phase, const QList<KitchenItem *> *kitchenItems, QGraphicsItem *parent = nullptr)
        : QGraphicsItem(parent)
        , m_phase(phase)
        , m_kitchenItems(kitchenItems)
    {
        setZValue(2.6);
        setAcceptedMouseButtons(Qt::NoButton);
        setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    }

    QRectF boundingRect() const override
    {
        return QRectF(0.0, 0.0, GameScene::WindowWidth, GameScene::WindowHeight);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

        const qreal phase = m_phase == nullptr ? 0.0 : *m_phase;

        painter->setPen(Qt::NoPen);
        if (m_kitchenItems == nullptr) {
            return;
        }

        const QRectF beltRect(124.0, 438.0, 50.0, 78.0);
        painter->setBrush(QColor(225, 224, 216));
        const int beltOffset = static_cast<int>(phase * 42.0) % 22;
        for (int y = static_cast<int>(beltRect.top()) - 22 + beltOffset;
             y < beltRect.bottom();
             y += 22) {
            painter->drawRect(QRectF(beltRect.left() + 4.0, y, beltRect.width() - 8.0, 8.0));
        }
        painter->setBrush(QColor(35, 34, 39));
        QPolygonF beltArrow;
        beltArrow << QPointF(148.0, 488.0) << QPointF(168.0, 476.0) << QPointF(168.0, 500.0);
        painter->drawPolygon(beltArrow);

        for (KitchenItem *item : *m_kitchenItems) {
            if (item == nullptr || item->itemKind() != KitchenItem::StoveKind) {
                continue;
            }

            if (item->storedItem() == NoItem && !item->soupReady() && !item->isBurnt()) {
                continue;
            }

            const QRectF stoveRect = item->mapToScene(item->boundingRect()).boundingRect();
            const int wobble = static_cast<int>((std::sin(phase * 8.0 + stoveRect.left() * 0.02) + 1.0) * 2.0);

            painter->setBrush(item->isBurnt() ? QColor(66, 66, 66) : QColor(255, 120, 62));
            painter->drawRect(QRectF(stoveRect.left() + 12.0, stoveRect.bottom() - 14.0 - wobble, 8.0, 10.0 + wobble));
            painter->drawRect(QRectF(stoveRect.left() + 24.0, stoveRect.bottom() - 18.0 - wobble, 10.0, 14.0 + wobble));
            painter->drawRect(QRectF(stoveRect.left() + 38.0, stoveRect.bottom() - 14.0 - wobble, 8.0, 10.0 + wobble));

            painter->setBrush(item->isBurnt() ? QColor(34, 34, 34) : QColor(255, 214, 96));
            painter->drawRect(QRectF(stoveRect.left() + 16.0, stoveRect.bottom() - 12.0 - wobble, 4.0, 6.0 + wobble));
            painter->drawRect(QRectF(stoveRect.left() + 28.0, stoveRect.bottom() - 16.0 - wobble, 4.0, 8.0 + wobble));
            painter->drawRect(QRectF(stoveRect.left() + 40.0, stoveRect.bottom() - 12.0 - wobble, 4.0, 6.0 + wobble));
        }

        for (KitchenItem *item : *m_kitchenItems) {
            if (item == nullptr || item->storedItem() == NoItem) {
                continue;
            }

            const QRectF itemRect = item->mapToScene(item->boundingRect()).boundingRect();
            const QPointF iconCenter = item->storedItemSceneCenter();
            const QSizeF iconSize = tableIconSize(item->storedItem());
            const QRectF iconRect(iconCenter.x() - iconSize.width() / 2.0,
                                  iconCenter.y() - iconSize.height() / 2.0,
                                  iconSize.width(),
                                  iconSize.height());
            drawCarryItemIcon(painter, item->storedItem(), iconRect);

            if (item->itemKind() == KitchenItem::SinkStationKind && item->storedItem() == DirtyPlateItem) {
                const QRectF bar(itemRect.left() + 18.0, itemRect.top() - 12.0, itemRect.width() - 36.0, 8.0);
                painter->setPen(QPen(QColor(67, 91, 98), 1.0));
                painter->setBrush(QColor(244, 252, 255));
                painter->drawRect(bar);
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(83, 202, 234));
                painter->drawRect(QRectF(bar.left() + 1.0,
                                         bar.top() + 1.0,
                                         (bar.width() - 2.0) * item->stationProgress(),
                                         bar.height() - 2.0));
            }

            if (item->itemKind() == KitchenItem::DirtyPlateStationKind && item->storedCount() > 1) {
                QFont font = painter->font();
                font.setPointSize(9);
                font.setBold(true);
                painter->setFont(font);
                painter->setPen(QColor(66, 48, 54));
                painter->drawText(QRectF(itemRect.center().x() + 10.0,
                                         itemRect.top() + 0.0,
                                         28.0,
                                         20.0),
                                  Qt::AlignLeft | Qt::AlignVCenter,
                                  QStringLiteral("x%1").arg(item->storedCount()));
                painter->setPen(Qt::NoPen);
            }
        }
    }

private:
    const qreal *m_phase;
    const QList<KitchenItem *> *m_kitchenItems;
};

bool isLeftStation(const KitchenItem *station)
{
    return station != nullptr && station->gridX() < 20;
}

bool isChoppableItem(CarryItemType item)
{
    return item == CucumberItem || item == ShrimpItem;
}

bool isPlaceableOnCounter(CarryItemType item)
{
    return isPlateCarrierItem(item)
           || ingredientBitForPlate(item) != 0
           || isChoppableItem(item)
           || item == RiceItem
           || item == NoriItem;
}

QSizeF tableIconSize(CarryItemType item)
{
    if (isPlateCarrierItem(item) || item == DirtyPlateItem) {
        return QSizeF(50.0, 42.0);
    }
    if (item == CucumberItem) {
        return QSizeF(48.0, 30.0);
    }
    if (item == NoriItem) {
        return QSizeF(38.0, 38.0);
    }
    if (item == RiceItem || item == CookedRiceItem) {
        return QSizeF(42.0, 34.0);
    }
    if (item == ShrimpItem) {
        return QSizeF(42.0, 34.0);
    }
    return QSizeF(34.0, 34.0);
}

QPointF nearestTableCellCenter(const KitchenItem *station, const QPointF &nearPoint)
{
    if (station == nullptr) {
        return nearPoint;
    }

    const QRectF rect = station->mapToScene(station->boundingRect()).boundingRect();
    if (rect.width() <= 112.0 && rect.height() <= 112.0) {
        return rect.center();
    }

    const qreal cell = 64.0;
    qreal x = rect.center().x();
    qreal y = rect.center().y();
    if (rect.width() >= rect.height()) {
        const qreal minX = rect.left() + cell / 2.0;
        const qreal maxX = rect.right() - cell / 2.0;
        x = minX + std::round((nearPoint.x() - minX) / cell) * cell;
        x = qBound(minX, x, maxX);
        y = rect.center().y();
    } else {
        const qreal minY = rect.top() + cell / 2.0;
        const qreal maxY = rect.bottom() - cell / 2.0;
        y = minY + std::round((nearPoint.y() - minY) / cell) * cell;
        y = qBound(minY, y, maxY);
        x = rect.center().x();
    }
    return QPointF(x, y);
}

CarryItemType choppedVariant(CarryItemType item)
{
    if (item == CucumberItem) {
        return ChoppedCucumberItem;
    }
    if (item == ShrimpItem) {
        return ChoppedShrimpItem;
    }
    return NoItem;
}

CarryItemType sourceItemForStation(const KitchenItem *station)
{
    if (station == nullptr) {
        return NoItem;
    }

    switch (station->itemKind()) {
    case KitchenItem::TomatoStationKind:
        return NoriItem;
    case KitchenItem::EggStationKind:
        return ShrimpItem;
    case KitchenItem::PlateStationKind:
        return isLeftStation(station) ? RiceItem : CucumberItem;
    case KitchenItem::CleanPlateStationKind:
        return CleanPlateItem;
    default:
        return NoItem;
    }
}

bool isDeliverableSushi(CarryItemType item)
{
    if (!isPlatedDish(item)) {
        return false;
    }

    const int bits = platedDishBits(item);
    const bool hasRiceAndNori = (bits & PlateRiceBit) != 0 && (bits & PlateNoriBit) != 0;
    const bool hasFilling = (bits & PlateCucumberBit) != 0 || (bits & PlateFishBit) != 0;
    return hasRiceAndNori && hasFilling;
}

int sushiScoreValue(CarryItemType item)
{
    const int bits = platedDishBits(item);
    const bool hasCucumber = (bits & PlateCucumberBit) != 0;
    const bool hasFish = (bits & PlateFishBit) != 0;
    if (hasCucumber && hasFish) {
        return 32;
    }
    if (hasFish) {
        return 24;
    }
    if (hasCucumber) {
        return 18;
    }
    return 0;
}

CarryItemType addIngredientToPlate(CarryItemType plateItem, CarryItemType ingredient)
{
    const int ingredientBit = ingredientBitForPlate(ingredient);
    if (ingredientBit == 0) {
        return NoItem;
    }

    int bits = 0;
    if (plateItem == CleanPlateItem) {
        bits = 0;
    } else if (isPlatedDish(plateItem)) {
        bits = platedDishBits(plateItem);
    } else {
        return NoItem;
    }

    if ((bits & ingredientBit) != 0) {
        return NoItem;
    }

    return makePlatedDish(bits | ingredientBit);
}

CarryItemType combineCounterItems(CarryItemType placedItem, CarryItemType carriedItem)
{
    const CarryItemType plateFirst = addIngredientToPlate(placedItem, carriedItem);
    if (plateFirst != NoItem) {
        return plateFirst;
    }

    const CarryItemType plateSecond = addIngredientToPlate(carriedItem, placedItem);
    if (plateSecond != NoItem) {
        return plateSecond;
    }
    return NoItem;
}

bool itemCanInteractWithStation(CarryItemType carriedItem, const KitchenItem *station)
{
    if (station == nullptr) {
        return false;
    }

    switch (station->itemKind()) {
    case KitchenItem::TomatoStationKind:
    case KitchenItem::EggStationKind:
    case KitchenItem::PlateStationKind:
        return carriedItem == NoItem;
    case KitchenItem::CleanPlateStationKind:
        if (carriedItem == NoItem) {
            return true;
        }
        if (station->storedItem() == NoItem && carriedItem == CleanPlateItem) {
            return true;
        }
        return addIngredientToPlate(station->storedItem() == NoItem ? CleanPlateItem : station->storedItem(),
                                    carriedItem) != NoItem;
    case KitchenItem::DirtyPlateStationKind:
        return carriedItem == NoItem && station->storedItem() == DirtyPlateItem;
    case KitchenItem::ChoppingStationKind:
        return isChoppableItem(carriedItem) || carriedItem == NoItem;
    case KitchenItem::StoveKind:
        return carriedItem == RiceItem || carriedItem == NoItem;
    case KitchenItem::SinkStationKind:
        return carriedItem == DirtyPlateItem || (carriedItem == NoItem && station->storedItem() == DirtyPlateItem);
    case KitchenItem::DeliveryWindowKind:
        return isDeliverableSushi(carriedItem);
    case KitchenItem::CounterKind:
        if (carriedItem == NoItem) {
            return station->storedItem() != NoItem;
        }
        if (station->storedItem() == NoItem) {
            return isPlaceableOnCounter(carriedItem);
        }
        return combineCounterItems(station->storedItem(), carriedItem) != NoItem;
    case KitchenItem::FloorTileKind:
    case KitchenItem::BoundaryWallKind:
    default:
        return false;
    }
}
}

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_menuOverlay(nullptr)
    , m_dialogOverlay(nullptr)
    , m_tutorialOverlay(nullptr)
    , m_hudOverlay(nullptr)
    , m_resultOverlay(nullptr)
    , m_statusText(nullptr)
    , m_bannerText(nullptr)
    , m_dynamicEffectsItem(nullptr)
    , m_state(StartMenuState)
    , m_previousState(StartMenuState)
    , m_score(0)
    , m_deliveriesCompleted(0)
    , m_targetDeliveries(4)
    , m_starCount(0)
    , m_currentLevel(1)
    , m_maxLevel(1)
    , m_totalPlateCount(4)
    , m_availableCleanPlates(4)
    , m_dirtyPlateCount(0)
    , m_timeRemainingSeconds(120.0)
    , m_roundDurationSeconds(120.0)
    , m_effectPhase(0.0)
    , m_language(ChineseUi)
    , m_difficulty(Difficulty1)
{
    setSceneRect(0.0, 0.0, WindowWidth, WindowHeight);
    setItemIndexMethod(QGraphicsScene::NoIndex);
    setBackgroundBrush(QColor(247, 231, 198));

    buildScene();

    connect(&m_tickTimer, &QTimer::timeout, this, [this]() { tick(); });
    m_tickTimer.start(16);
    m_elapsedTimer.start();
}

void GameScene::buildScene()
{
    buildKitchen();
    createPlayers();
    createMenu();
    drawUI();
}

void GameScene::buildKitchen()
{
    createFloor();
    drawStaticElements();
    createBoundaryWalls();
    createKitchenFixtures();
    m_dynamicEffectsItem = new DynamicEffectsItem(&m_effectPhase, &m_kitchenItems);
    addItem(m_dynamicEffectsItem);
}

void GameScene::createPlayers()
{
    auto *playerOne = new PlayerItem(QColor(255, 160, 182),
                                     QColor(255, 233, 119),
                                     PlayerItem::Controls{Qt::Key_W, Qt::Key_S, Qt::Key_A, Qt::Key_D, Qt::Key_F});
    playerOne->resetPosition(QPointF(420.0, 518.0));
    playerOne->setVisible(false);
    addItem(playerOne);
    m_players.append(playerOne);

    auto *playerTwo = new PlayerItem(QColor(113, 194, 255),
                                     QColor(143, 232, 164),
                                     PlayerItem::Controls{Qt::Key_Up, Qt::Key_Down, Qt::Key_Left, Qt::Key_Right, Qt::Key_Slash});
    playerTwo->resetPosition(QPointF(840.0, 518.0));
    playerTwo->setVisible(false);
    addItem(playerTwo);
    m_players.append(playerTwo);
}

void GameScene::createMenu()
{
    m_menuOverlay = new MenuOverlayItem(WindowWidth, WindowHeight);
    addItem(m_menuOverlay);

    m_dialogOverlay = new DialogOverlayItem(WindowWidth, WindowHeight);
    m_dialogOverlay->setVisible(false);
    addItem(m_dialogOverlay);

    m_tutorialOverlay = new TutorialOverlayItem(WindowWidth, WindowHeight);
    m_tutorialOverlay->setVisible(false);
    addItem(m_tutorialOverlay);

    m_resultOverlay = new ResultOverlayItem(WindowWidth, WindowHeight);
    m_resultOverlay->setVisible(false);
    addItem(m_resultOverlay);
}

void GameScene::drawUI()
{
    m_hudOverlay = new HudOverlayItem(WindowWidth, WindowHeight);
    m_hudOverlay->setVisible(false);
    addItem(m_hudOverlay);

    QFont helpFont;
    helpFont.setPointSize(10);
    helpFont.setBold(true);

    m_statusText = addSimpleText(QString(), helpFont);
    m_statusText->setBrush(QColor(70, 70, 70));
    m_statusText->setZValue(10.0);
    m_statusText->setPos(24.0, WindowHeight - 56.0);
    m_statusText->setVisible(false);

    QFont bannerFont;
    bannerFont.setPointSize(22);
    bannerFont.setBold(true);

    m_bannerText = addSimpleText(QString(), bannerFont);
    m_bannerText->setBrush(QColor(255, 255, 255));
    m_bannerText->setZValue(10.0);
    m_bannerText->setPos(260.0, 86.0);
    m_bannerText->setVisible(false);
}

void GameScene::createFloor()
{
    auto *modeledKitchen = new ModeledKitchenSceneItem();
    addItem(modeledKitchen);
}

void GameScene::drawGroundGrid()
{
}

void GameScene::drawStaticElements()
{
}

void GameScene::createBoundaryWalls()
{
    for (int column = 0; column < ColumnCount; ++column) {
        addKitchenItem(column, 0, 1, 1, KitchenItem::BoundaryWallKind);
        addKitchenItem(column, RowCount - 1, 1, 1, KitchenItem::BoundaryWallKind);
    }

    for (int row = 1; row < RowCount - 1; ++row) {
        addKitchenItem(0, row, 1, 1, KitchenItem::BoundaryWallKind);
        addKitchenItem(ColumnCount - 1, row, 1, 1, KitchenItem::BoundaryWallKind);
    }
}

void GameScene::createEnvironmentDecor()
{
    auto *topBackdrop = addRect(QRectF(32.0, 32.0, WindowWidth - 64.0, 150.0),
                                QPen(Qt::NoPen),
                                QBrush(QColor(255, 218, 232)));
    topBackdrop->setZValue(1.0);

    auto *topStripe = addRect(QRectF(32.0, 32.0, WindowWidth - 64.0, 32.0),
                              QPen(Qt::NoPen),
                              QBrush(QColor(245, 170, 198)));
    topStripe->setZValue(1.1);

    for (int i = 0; i < 7; ++i) {
        auto *lamp = addRect(QRectF(142.0 + i * 156.0, 48.0, 60.0, 20.0),
                             QPen(Qt::NoPen),
                             QBrush(QColor(255, 246, 214)));
        lamp->setZValue(1.2);
    }

    for (int i = 0; i < 6; ++i) {
        const qreal x = 220.0 + i * 148.0;
        auto *bannerTop = addRect(QRectF(x, 114.0, 48.0, 6.0),
                                  QPen(Qt::NoPen),
                                  QBrush(QColor(248, 230, 236)));
        bannerTop->setZValue(1.3);
        auto *bannerFlag = addRect(QRectF(x + 8.0, 120.0, 32.0, 24.0),
                                   QPen(Qt::NoPen),
                                   QBrush(i % 2 == 0 ? QColor(255, 179, 200) : QColor(169, 222, 255)));
        bannerFlag->setZValue(1.3);
    }

    auto *stageBoard = addRect(QRectF(210.0, 78.0, 860.0, 96.0),
                               QPen(Qt::NoPen),
                               QBrush(QColor(255, 247, 252, 220)));
    stageBoard->setZValue(1.25);

    auto *title = addText(QStringLiteral("PIXEL SWEET KITCHEN"));
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setDefaultTextColor(QColor(214, 92, 138));
    title->setPos(430.0, 96.0);
    title->setZValue(1.4);

    auto *subtitle = addText(QStringLiteral("Pixel Co-op Sushi Score Attack"));
    QFont subFont;
    subFont.setPointSize(10);
    subFont.setBold(true);
    subtitle->setFont(subFont);
    subtitle->setDefaultTextColor(QColor(132, 118, 148));
    subtitle->setPos(446.0, 126.0);
    subtitle->setZValue(1.4);

    for (int i = 0; i < 5; ++i) {
        auto *pillar = addRect(QRectF(96.0 + i * 248.0, 186.0, 24.0, 82.0),
                               QPen(Qt::NoPen),
                               QBrush(QColor(236, 206, 220, 130)));
        pillar->setZValue(1.05);
    }

    auto *runway = addRect(QRectF(236.0, 520.0, 808.0, 82.0),
                           QPen(Qt::NoPen),
                           QBrush(QColor(255, 224, 235, 110)));
    runway->setZValue(1.0);

    auto *centerRug = addRect(QRectF(506.0, 514.0, 268.0, 96.0),
                              QPen(Qt::NoPen),
                              QBrush(QColor(255, 243, 248, 150)));
    centerRug->setZValue(1.01);

    for (int i = 0; i < 4; ++i) {
        auto *plantPot = addRect(QRectF(74.0 + i * 344.0, 612.0, 24.0, 18.0),
                                 QPen(Qt::NoPen),
                                 QBrush(QColor(164, 118, 88)));
        plantPot->setZValue(1.1);
        auto *leafA = addRect(QRectF(70.0 + i * 344.0, 592.0, 12.0, 22.0),
                              QPen(Qt::NoPen),
                              QBrush(QColor(142, 220, 160)));
        leafA->setZValue(1.11);
        auto *leafB = addRect(QRectF(84.0 + i * 344.0, 588.0, 12.0, 26.0),
                              QPen(Qt::NoPen),
                              QBrush(QColor(118, 205, 148)));
        leafB->setZValue(1.11);
        auto *leafC = addRect(QRectF(98.0 + i * 344.0, 594.0, 12.0, 20.0),
                              QPen(Qt::NoPen),
                              QBrush(QColor(162, 229, 174)));
        leafC->setZValue(1.11);
    }
}

void GameScene::createKitchenFixtures()
{
    // Invisible gameplay rectangles are matched to the modeled kitchen art in
    // screen pixels, so the walkable path follows the visible counters closely.
    addKitchenItemRect(QRectF(0.0, 0.0, WindowWidth, 188.0), KitchenItem::BoundaryWallKind);

    addKitchenItemRect(QRectF(64.0, 214.0, 92.0, 318.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(154.0, 250.0, 70.0, 190.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(156.0, 182.0, 486.0, 56.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(360.0, 318.0, 76.0, 236.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(574.0, 182.0, 68.0, 426.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(758.0, 146.0, 266.0, 50.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(1012.0, 204.0, 72.0, 270.0), KitchenItem::BoundaryWallKind);
    addKitchenItemRect(QRectF(780.0, 498.0, 258.0, 80.0), KitchenItem::BoundaryWallKind);

    // Sushi ingredient chain: rice -> stove, shrimp/cucumber -> chop, nori + rice + filling -> deliver.
    addKitchenItemRect(QRectF(150.0, 244.0, 90.0, 58.0), KitchenItem::TomatoStationKind);
    addKitchenItemRect(QRectF(150.0, 302.0, 90.0, 58.0), KitchenItem::EggStationKind);
    addKitchenItemRect(QRectF(150.0, 360.0, 90.0, 58.0), KitchenItem::PlateStationKind);
    addKitchenItemRect(QRectF(996.0, 306.0, 92.0, 74.0), KitchenItem::PlateStationKind);
    addKitchenItemRect(QRectF(1018.0, 238.0, 60.0, 56.0), KitchenItem::CleanPlateStationKind);

    addKitchenItemRect(QRectF(244.0, 174.0, 84.0, 70.0), KitchenItem::ChoppingStationKind);
    addKitchenItemRect(QRectF(382.0, 174.0, 82.0, 70.0), KitchenItem::ChoppingStationKind);
    addKitchenItemRect(QRectF(524.0, 174.0, 84.0, 70.0), KitchenItem::ChoppingStationKind);

    addKitchenItemRect(QRectF(770.0, 118.0, 68.0, 82.0), KitchenItem::StoveKind);
    addKitchenItemRect(QRectF(838.0, 118.0, 68.0, 82.0), KitchenItem::StoveKind);
    addKitchenItemRect(QRectF(902.0, 118.0, 68.0, 82.0), KitchenItem::StoveKind);

    addKitchenItemRect(QRectF(350.0, 306.0, 98.0, 92.0), KitchenItem::CleanPlateStationKind);
    addKitchenItemRect(QRectF(342.0, 430.0, 104.0, 112.0), KitchenItem::CleanPlateStationKind);
    addKitchenItemRect(QRectF(570.0, 312.0, 78.0, 168.0), KitchenItem::CounterKind);
    addKitchenItemRect(QRectF(790.0, 488.0, 126.0, 82.0), KitchenItem::SinkStationKind);
    addKitchenItemRect(QRectF(944.0, 494.0, 86.0, 90.0), KitchenItem::DirtyPlateStationKind);
    addKitchenItemRect(QRectF(780.0, 498.0, 258.0, 80.0), KitchenItem::CounterKind);

    addKitchenItemRect(QRectF(62.0, 378.0, 112.0, 104.0), KitchenItem::DeliveryWindowKind);
}

void GameScene::addKitchenItem(int gridX, int gridY, int gridWidth, int gridHeight, int kind)
{
    auto *item = new KitchenItem(static_cast<KitchenItem::Kind>(kind), gridX, gridY, gridWidth, gridHeight);
    item->setOpacity(0.0);
    item->setZValue(2.0);
    addItem(item);
    m_kitchenItems.append(item);
}

void GameScene::addKitchenItemRect(const QRectF &rect, int kind)
{
    auto *item = new KitchenItem(static_cast<KitchenItem::Kind>(kind), rect);
    item->setOpacity(0.0);
    item->setZValue(2.0);
    addItem(item);
    m_kitchenItems.append(item);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        m_pressedKeys.insert(event->key());
    }

    if (m_state == StartMenuState) {
        if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
            m_menuOverlay->setSelectedIndex((m_menuOverlay->selectedIndex() + MenuOverlayItem::MenuCount - 1)
                                            % MenuOverlayItem::MenuCount);
        } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
            m_menuOverlay->setSelectedIndex((m_menuOverlay->selectedIndex() + 1) % MenuOverlayItem::MenuCount);
        } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
            if (m_menuOverlay->selectedIndex() == 0) {
                m_menuOverlay->setLanguageOption((m_menuOverlay->languageOption() + 1) % 2);
            } else if (m_menuOverlay->selectedIndex() == 1) {
                m_menuOverlay->setDifficultyOption((m_menuOverlay->difficultyOption() + 2) % 3);
            }
        } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
            if (m_menuOverlay->selectedIndex() == 0) {
                m_menuOverlay->setLanguageOption((m_menuOverlay->languageOption() + 1) % 2);
            } else if (m_menuOverlay->selectedIndex() == 1) {
                m_menuOverlay->setDifficultyOption((m_menuOverlay->difficultyOption() + 1) % 3);
            }
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
                   || event->key() == Qt::Key_Slash) {
            activateMenuSelection();
        } else if (event->key() == Qt::Key_Escape) {
            openExitDialog();
        }
    } else if (m_state == TutorialState) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
            || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
            || event->key() == Qt::Key_Slash) {
            beginRound();
        } else if (event->key() == Qt::Key_Escape) {
            showMenu();
        }
    } else if (m_state == PlayingState) {
        if (event->key() == Qt::Key_Escape) {
            openExitDialog();
        } else if (!event->isAutoRepeat()) {
            for (PlayerItem *player : m_players) {
                if (event->key() == player->controls().interact) {
                    tryInteract(player);
                    break;
                }
            }
        }
    } else if (m_state == ExitDialogState) {
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A
            || event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
            m_dialogOverlay->setSelectedButton(0);
        } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D
                   || event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
            m_dialogOverlay->setSelectedButton(1);
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
                   || event->key() == Qt::Key_Slash) {
            confirmDialogSelection();
        } else if (event->key() == Qt::Key_Escape) {
            closeExitDialog();
        }
    } else if (m_state == RoundEndState) {
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A
            || event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
            m_resultOverlay->setSelectedButton(ResultOverlayItem::RestartButton);
        } else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D
                   || event->key() == Qt::Key_Down || event->key() == Qt::Key_S) {
            m_resultOverlay->setSelectedButton(ResultOverlayItem::NextLevelButton);
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter
                   || event->key() == Qt::Key_Space || event->key() == Qt::Key_F
                   || event->key() == Qt::Key_Slash) {
            confirmResultSelection();
        } else if (event->key() == Qt::Key_Escape) {
            showMenu();
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        m_pressedKeys.remove(event->key());
    }

    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::tick()
{
    qreal deltaSeconds = m_elapsedTimer.restart() / 1000.0;
    if (deltaSeconds > 0.05) {
        deltaSeconds = 0.05;
    }

    if (m_state == StartMenuState && m_menuOverlay != nullptr) {
        m_menuOverlay->advanceAnimation(deltaSeconds);
    }

    if (m_state != PlayingState) {
        return;
    }

    m_effectPhase += deltaSeconds;

    if (isTimedMode()) {
        m_timeRemainingSeconds -= deltaSeconds;
        if (m_timeRemainingSeconds <= 0.0) {
            m_timeRemainingSeconds = 0.0;
            finishRound(true);
            return;
        }
    }

    const QRectF bounds = playArea();
    for (int i = m_pendingDirtyPlateTimers.size() - 1; i >= 0; --i) {
        m_pendingDirtyPlateTimers[i] -= deltaSeconds;
        if (m_pendingDirtyPlateTimers.at(i) <= 0.0) {
            ++m_dirtyPlateCount;
            m_pendingDirtyPlateTimers.removeAt(i);
        }
    }

    for (KitchenItem *item : m_kitchenItems) {
        item->tick(deltaSeconds, isBurnMode());
        if (item->itemKind() == KitchenItem::DeliveryWindowKind && isDeliverableSushi(item->storedItem())) {
            item->setStationProgress(item->stationProgress() + deltaSeconds / 1.5);
            if (item->stationProgress() >= 1.0) {
                m_score += sushiScoreValue(item->storedItem());
                ++m_deliveriesCompleted;
                m_pendingDirtyPlateTimers.append(15.0);
                item->resetState();
            }
        }
        if (item->itemKind() == KitchenItem::SinkStationKind && item->storedItem() == DirtyPlateItem) {
            item->setStationProgress(item->stationProgress() + deltaSeconds / 3.0);
            if (item->stationProgress() >= 1.0) {
                item->resetState();
                m_availableCleanPlates = qMin(m_totalPlateCount, m_availableCleanPlates + 1);
            }
        }
    }
    refreshDishStationVisuals();

    for (PlayerItem *player : m_players) {
        player->tick(m_pressedKeys, deltaSeconds, bounds);
    }

    if (m_dynamicEffectsItem != nullptr) {
        m_dynamicEffectsItem->update();
    }

    updateHud();
}

void GameScene::startGame()
{
    m_currentLevel = 1;
    restartLevel();
}

void GameScene::beginRound()
{
    m_state = PlayingState;
    m_tutorialOverlay->setVisible(false);
    m_resultOverlay->setVisible(false);
    m_bannerText->setVisible(false);
    for (PlayerItem *player : m_players) {
        player->setVisible(true);
    }
    updateHud();
}

void GameScene::showMenu()
{
    m_state = StartMenuState;
    m_menuOverlay->setVisible(true);
    m_dialogOverlay->setVisible(false);
    m_tutorialOverlay->setVisible(false);
    m_resultOverlay->setVisible(false);
    m_pressedKeys.clear();
    for (PlayerItem *player : m_players) {
        player->setCarriedItem(NoItem);
        player->setVisible(false);
    }

    m_hudOverlay->setVisible(false);
    m_statusText->setVisible(false);
    m_bannerText->setVisible(false);
}

void GameScene::resetKitchenState()
{
    m_effectPhase = 0.0;
    m_availableCleanPlates = m_totalPlateCount;
    m_dirtyPlateCount = 0;
    m_pendingDirtyPlateTimers.clear();
    for (KitchenItem *item : m_kitchenItems) {
        item->resetState();
    }
    refreshDishStationVisuals();
}

void GameScene::refreshDishStationVisuals()
{
    for (KitchenItem *item : m_kitchenItems) {
        if (item->itemKind() != KitchenItem::DirtyPlateStationKind) {
            continue;
        }
        item->setStoredItem(m_dirtyPlateCount > 0 ? DirtyPlateItem : NoItem);
        item->setStoredCount(m_dirtyPlateCount);
    }
}

void GameScene::updateHud()
{
    if (m_state != PlayingState && m_state != TutorialState && m_state != RoundEndState) {
        return;
    }

    const int wholeSeconds = isTimedMode() ? qMax(0, static_cast<int>(std::ceil(m_timeRemainingSeconds))) : -1;
    m_hudOverlay->setStats(m_score,
                           wholeSeconds,
                           m_deliveriesCompleted,
                           m_targetDeliveries,
                           trText(QStringLiteral("寿司"), QStringLiteral("Sushi")),
                           m_language == ChineseUi);
    if (m_players.size() < 2) {
        return;
    }

    const QString playerOne = carryItemLabel(m_players.at(0)->carriedItem());
    const QString playerTwo = carryItemLabel(m_players.at(1)->carriedItem());
    m_statusText->setText(trText(QStringLiteral("P1[F]: %1    P2[/]: %2"),
                                 QStringLiteral("P1[F]: %1    P2[/]: %2"))
                              .arg(playerOne, playerTwo));
    m_statusText->setVisible(false);
}

void GameScene::finishRound(bool success)
{
    m_state = RoundEndState;
    for (PlayerItem *player : m_players) {
        player->setVisible(true);
    }

    m_starCount = success ? calculateStarCount() : 0;
    const bool unlocked = !hasNextLevel() || (success && m_starCount >= 1);

    m_resultOverlay->setLanguageChinese(m_language == ChineseUi);
    m_resultOverlay->setResultData(success,
                                   m_currentLevel,
                                   m_starCount,
                                   m_deliveriesCompleted,
                                   m_targetDeliveries,
                                   m_score,
                                   unlocked,
                                   hasNextLevel());
    m_resultOverlay->setVisible(true);
    m_bannerText->setVisible(false);
    updateHud();
}

void GameScene::tryInteract(PlayerItem *player)
{
    KitchenItem *item = findInteractiveItem(player);
    if (item == nullptr) {
        return;
    }

    switch (item->itemKind()) {
    case KitchenItem::TomatoStationKind:
    case KitchenItem::EggStationKind:
    case KitchenItem::PlateStationKind:
        if (player->isHandsEmpty()) {
            player->setCarriedItem(sourceItemForStation(item));
        }
        break;
    case KitchenItem::CleanPlateStationKind:
        if (player->isHandsEmpty()) {
            if (item->storedItem() != NoItem) {
                player->setCarriedItem(item->storedItem());
                item->resetState();
            } else if (m_availableCleanPlates > 0) {
                player->setCarriedItem(CleanPlateItem);
                --m_availableCleanPlates;
            }
        } else {
            if (item->storedItem() == NoItem && player->carriedItem() == CleanPlateItem) {
                item->setStoredItem(CleanPlateItem);
                item->setStoredItemSceneCenter(item->mapToScene(item->boundingRect().center()));
                item->setStoredCount(1);
                player->setCarriedItem(NoItem);
            } else {
                const CarryItemType plateOnTable = item->storedItem() == NoItem ? CleanPlateItem : item->storedItem();
                const CarryItemType combined = addIngredientToPlate(plateOnTable, player->carriedItem());
                if (combined != NoItem && (item->storedItem() != NoItem || m_availableCleanPlates > 0)) {
                    if (item->storedItem() == NoItem) {
                        --m_availableCleanPlates;
                    }
                    item->setStoredItem(combined);
                    item->setStoredItemSceneCenter(item->mapToScene(item->boundingRect().center()));
                    item->setStoredCount(1);
                    player->setCarriedItem(NoItem);
                }
            }
        }
        break;
    case KitchenItem::DirtyPlateStationKind:
        if (player->isHandsEmpty() && m_dirtyPlateCount > 0) {
            --m_dirtyPlateCount;
            player->setCarriedItem(DirtyPlateItem);
            refreshDishStationVisuals();
        }
        break;
    case KitchenItem::SinkStationKind:
        if (player->carriedItem() == DirtyPlateItem && item->storedItem() == NoItem) {
            item->setStoredItem(DirtyPlateItem);
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        }
        break;
    case KitchenItem::CounterKind:
        if (player->isHandsEmpty() && item->storedItem() != NoItem) {
            player->setCarriedItem(item->storedItem());
            item->resetState();
        } else if (!player->isHandsEmpty() && item->storedItem() == NoItem && isPlaceableOnCounter(player->carriedItem())) {
            item->setStoredItem(player->carriedItem());
            item->setStoredItemSceneCenter(nearestTableCellCenter(item, player->scenePos()));
            item->setStoredCount(1);
            player->setCarriedItem(NoItem);
        } else if (!player->isHandsEmpty() && item->storedItem() != NoItem) {
            const CarryItemType combined = combineCounterItems(item->storedItem(), player->carriedItem());
            if (combined != NoItem) {
                item->setStoredItem(combined);
                item->setStoredCount(1);
                player->setCarriedItem(NoItem);
            }
        }
        break;
    case KitchenItem::ChoppingStationKind:
        if (isChoppableItem(player->carriedItem()) && item->storedItem() == NoItem) {
            item->setStoredItem(player->carriedItem());
            item->setStoredCount(1);
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        } else if (player->isHandsEmpty() && isChoppableItem(item->storedItem())) {
            item->setStationProgress(item->stationProgress() + 0.5);
            if (item->stationProgress() >= 1.0) {
                item->setStoredItem(choppedVariant(item->storedItem()));
                item->setStoredCount(1);
            }
        } else if (player->isHandsEmpty()
                   && (item->storedItem() == ChoppedCucumberItem || item->storedItem() == ChoppedShrimpItem)) {
            player->setCarriedItem(item->storedItem());
            item->resetState();
        }
        break;
    case KitchenItem::StoveKind:
        if (item->isBurnt() && player->isHandsEmpty()) {
            item->resetState();
            break;
        }
        if (player->carriedItem() == RiceItem && !item->soupReady() && item->storedItem() == NoItem) {
            item->setStoredItem(RiceItem);
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        } else if (item->soupReady() && player->isHandsEmpty()) {
            player->setCarriedItem(CookedRiceItem);
            item->resetState();
        }
        break;
    case KitchenItem::DeliveryWindowKind:
        if (isDeliverableSushi(player->carriedItem()) && item->storedItem() == NoItem) {
            item->setStoredItem(player->carriedItem());
            item->setStationProgress(0.0);
            player->setCarriedItem(NoItem);
        }
        break;
    case KitchenItem::FloorTileKind:
    case KitchenItem::BoundaryWallKind:
        break;
    }

    updateHud();
}

KitchenItem *GameScene::findInteractiveItem(const PlayerItem *player) const
{
    const QPointF origin = player->scenePos() + QPointF(0.0, 6.0);
    const QPointF facing = player->facingVector();
    const QPointF probeCenter = origin + facing * 54.0;
    const QRectF probeRect(probeCenter.x() - 46.0, probeCenter.y() - 42.0, 92.0, 84.0);
    const CarryItemType carriedItem = player->carriedItem();

    KitchenItem *bestItem = nullptr;
    qreal bestScore = 1e9;
    for (KitchenItem *item : m_kitchenItems) {
        if (item->itemKind() == KitchenItem::FloorTileKind || item->itemKind() == KitchenItem::BoundaryWallKind) {
            continue;
        }

        const QRectF sceneRect = item->mapToScene(item->boundingRect()).boundingRect();
        if (!sceneRect.intersects(probeRect)) {
            continue;
        }

        const qreal distance = QLineF(origin, sceneRect.center()).length();
        const qreal compatibilityPenalty = itemCanInteractWithStation(carriedItem, item) ? 0.0 : 1000.0;
        qreal stationPriority = 0.0;
        if (item->itemKind() == KitchenItem::CleanPlateStationKind
            && (carriedItem == NoItem || carriedItem == CleanPlateItem || ingredientBitForPlate(carriedItem) != 0)) {
            stationPriority = -35.0;
        }
        const qreal score = distance + compatibilityPenalty + stationPriority;
        if (score < bestScore) {
            bestScore = score;
            bestItem = item;
        }
    }

    return bestItem;
}

void GameScene::openExitDialog()
{
    m_previousState = m_state;
    m_state = ExitDialogState;
    m_dialogOverlay->setSelectedButton(1);
    m_dialogOverlay->setVisible(true);
    if (m_statusText != nullptr && m_state == ExitDialogState) {
        m_statusText->setText(QStringLiteral("Game paused"));
    }
}

void GameScene::closeExitDialog()
{
    m_state = m_previousState;
    m_dialogOverlay->setVisible(false);
    updateHud();
}

void GameScene::activateMenuSelection()
{
    const int selected = m_menuOverlay->selectedIndex();
    if (selected == 2) {
        m_language = m_menuOverlay->languageOption() == MenuOverlayItem::ChineseLanguage ? ChineseUi : EnglishUi;
        m_difficulty = static_cast<DifficultyMode>(m_menuOverlay->difficultyOption());
        startGame();
    } else if (selected == 3) {
        openExitDialog();
    }
}

void GameScene::confirmDialogSelection()
{
    if (m_dialogOverlay->selectedButton() == 0) {
        QCoreApplication::quit();
        return;
    }

    closeExitDialog();
}

void GameScene::confirmResultSelection()
{
    if (m_resultOverlay->selectedButton() == ResultOverlayItem::RestartButton) {
        restartLevel();
        return;
    }

    if (!hasNextLevel()) {
        showMenu();
        return;
    }

    if (m_starCount >= 1) {
        goToNextLevel();
    }
}

QRectF GameScene::playArea() const
{
    return QRectF(72.0, 92.0, 1060.0, 548.0);
}

void GameScene::applyDifficultySettings()
{
    if (m_difficulty == Difficulty1) {
        m_targetDeliveries = 6;
        m_roundDurationSeconds = -1.0;
    } else if (m_difficulty == Difficulty2) {
        m_targetDeliveries = 8;
        m_roundDurationSeconds = 180.0;
    } else {
        m_targetDeliveries = 10;
        m_roundDurationSeconds = 150.0;
    }
}

QString GameScene::trText(const QString &zh, const QString &en) const
{
    return m_language == ChineseUi ? zh : en;
}

bool GameScene::isTimedMode() const
{
    return m_difficulty != Difficulty1;
}

bool GameScene::isBurnMode() const
{
    return m_difficulty == Difficulty3;
}

bool GameScene::hasNextLevel() const
{
    return false;
}

int GameScene::calculateStarCount() const
{
    if (m_targetDeliveries <= 0 || m_deliveriesCompleted <= 0) {
        return 0;
    }

    if (m_deliveriesCompleted >= m_targetDeliveries) {
        return 3;
    }
    if (m_deliveriesCompleted >= qCeil(m_targetDeliveries * 0.67)) {
        return 2;
    }
    if (m_deliveriesCompleted >= qCeil(m_targetDeliveries * 0.34)) {
        return 1;
    }
    return 0;
}

void GameScene::restartLevel()
{
    m_state = TutorialState;
    m_menuOverlay->setVisible(false);
    m_dialogOverlay->setVisible(false);
    m_tutorialOverlay->setVisible(true);
    m_resultOverlay->setVisible(false);
    m_pressedKeys.clear();

    if (m_players.size() >= 2) {
        m_players.at(0)->resetPosition(QPointF(480.0, 300.0));
        m_players.at(1)->resetPosition(QPointF(860.0, 300.0));
    }

    m_score = 0;
    m_deliveriesCompleted = 0;
    m_starCount = 0;
    applyDifficultySettings();
    m_timeRemainingSeconds = isTimedMode() ? m_roundDurationSeconds : -1.0;
    resetKitchenState();
    for (PlayerItem *player : m_players) {
        player->setCarriedItem(NoItem);
        player->setVisible(false);
    }

    m_hudOverlay->setVisible(true);
    m_statusText->setVisible(false);
    m_bannerText->setVisible(false);
    m_tutorialOverlay->setLanguageChinese(m_language == ChineseUi);
    m_tutorialOverlay->setDifficultyMode(static_cast<int>(m_difficulty));
    updateHud();
}

void GameScene::goToNextLevel()
{
    showMenu();
}
