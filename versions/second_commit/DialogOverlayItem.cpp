#include "DialogOverlayItem.h"

#include <QPainter>

#include "Theme.h"

DialogOverlayItem::DialogOverlayItem(qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_rect(0.0, 0.0, width, height)
    , m_selectedButton(1)
{
    setZValue(100.0);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF DialogOverlayItem::boundingRect() const
{
    return m_rect;
}

void DialogOverlayItem::setSelectedButton(int index)
{
    if (index == m_selectedButton) {
        return;
    }

    m_selectedButton = index;
    update();
}

int DialogOverlayItem::selectedButton() const
{
    return m_selectedButton;
}

void DialogOverlayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->fillRect(m_rect, QColor(0, 0, 0, 140));

    Theme::roundedBox(painter, QRectF(128.0, 184.0, 1024.0, 340.0), Theme::panelBlueLight());
    Theme::roundedBox(painter, QRectF(128.0, 184.0, 512.0, 76.0), Theme::panelBlue());
    Theme::text(painter, QRectF(164.0, 196.0, 438.0, 48.0), QStringLiteral("退出"), 34, true, QColor(255, 255, 255));
    Theme::text(painter, QRectF(350.0, 306.0, 580.0, 40.0), QStringLiteral("真的要退出游戏吗？"), 24, true, QColor(85, 121, 148));

    const QColor leftFill = m_selectedButton == 0 ? QColor(255, 171, 64) : Theme::panelBlue();
    const QColor rightFill = m_selectedButton == 1 ? QColor(255, 171, 64) : Theme::panelBlue();
    Theme::roundedBox(painter, QRectF(350.0, 392.0, 256.0, 74.0), leftFill);
    Theme::roundedBox(painter, QRectF(674.0, 392.0, 256.0, 74.0), rightFill);
    Theme::text(painter, QRectF(350.0, 404.0, 256.0, 46.0), QStringLiteral("退出"), 30, true, QColor(255, 255, 255));
    Theme::text(painter, QRectF(674.0, 404.0, 256.0, 46.0), QStringLiteral("取消"), 30, true, QColor(255, 255, 255));
}
