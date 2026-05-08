#include "MenuOverlayItem.h"

#include <cmath>

#include <QCoreApplication>
#include <QDir>
#include <QImage>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QStringList>

#include "Theme.h"

namespace {
QImage loadArtwork(const QString &preferred)
{
    const QString appDir = QCoreApplication::applicationDirPath();
    const QStringList candidates = {
        QDir(appDir).absoluteFilePath(QStringLiteral("../") + preferred),
        QDir(appDir).absoluteFilePath(QStringLiteral("../../") + preferred),
        QDir::current().absoluteFilePath(preferred),
        QDir(QStringLiteral("C:/Users/Iriya/Desktop/Overcooked")).absoluteFilePath(preferred)
    };

    for (const QString &path : candidates) {
        QImage image(path);
        if (!image.isNull()) {
            return image;
        }
    }

    return {};
}

QString coverArtworkName()
{
    return QString::fromUtf8("\xE5\x8E\xA8\xE6\x88\xBF\xE5\xB0\x81\xE9\x9D\xA2.png");
}

QImage menuCover()
{
    return loadArtwork(coverArtworkName());
}

QImage menuPreview()
{
    QImage cover = menuCover();
    if (!cover.isNull()) {
        return cover;
    }

    QImage image = loadArtwork(QStringLiteral("菜单厨房.png"));
    if (!image.isNull()) {
        return image;
    }
    return loadArtwork(QStringLiteral("厨房场景.png"));
}

QString difficultyLabel(int languageOption, int difficultyOption)
{
    if (languageOption == MenuOverlayItem::ChineseLanguage) {
        switch (difficultyOption) {
        case MenuOverlayItem::DifficultyOne:
            return QStringLiteral("轻松");
        case MenuOverlayItem::DifficultyTwo:
            return QStringLiteral("限时");
        case MenuOverlayItem::DifficultyThree:
        default:
            return QStringLiteral("糊锅");
        }
    }

    switch (difficultyOption) {
    case MenuOverlayItem::DifficultyOne:
        return QStringLiteral("Casual");
    case MenuOverlayItem::DifficultyTwo:
        return QStringLiteral("Timed");
    case MenuOverlayItem::DifficultyThree:
    default:
        return QStringLiteral("Burn");
    }
}

QString difficultyDescription(int languageOption, int difficultyOption)
{
    if (languageOption == MenuOverlayItem::ChineseLanguage) {
        switch (difficultyOption) {
        case MenuOverlayItem::DifficultyOne:
            return QStringLiteral("无限时，练习寿司出餐和路线。");
        case MenuOverlayItem::DifficultyTwo:
            return QStringLiteral("有限时，尽可能多做寿司。");
        case MenuOverlayItem::DifficultyThree:
        default:
            return QStringLiteral("有限时，米饭放太久会糊锅。");
        }
    }

    switch (difficultyOption) {
    case MenuOverlayItem::DifficultyOne:
        return QStringLiteral("No timer. Practice sushi routing.");
    case MenuOverlayItem::DifficultyTwo:
        return QStringLiteral("Timed sushi score rush.");
    case MenuOverlayItem::DifficultyThree:
    default:
        return QStringLiteral("Timed rush with burning pots.");
    }
}

void drawHardPanel(QPainter *painter, const QRectF &rect, const QColor &fill, const QColor &border)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(76, 52, 37, 58));
    painter->drawRoundedRect(rect.translated(5.0, 6.0), 6.0, 6.0);
    painter->setBrush(border);
    painter->drawRoundedRect(rect, 6.0, 6.0);
    painter->setBrush(fill);
    painter->drawRoundedRect(rect.adjusted(4.0, 4.0, -4.0, -4.0), 4.0, 4.0);
}

void drawSpark(QPainter *painter, const QPointF &center, qreal size, const QColor &color)
{
    painter->save();
    painter->translate(center);
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawRect(QRectF(-size * 0.12, -size * 0.5, size * 0.24, size));
    painter->drawRect(QRectF(-size * 0.5, -size * 0.12, size, size * 0.24));
    painter->restore();
}
}

MenuOverlayItem::MenuOverlayItem(qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , m_rect(0.0, 0.0, width, height)
    , m_selectedIndex(0)
    , m_languageOption(ChineseLanguage)
    , m_difficultyOption(DifficultyOne)
    , m_animationTime(0.0)
{
    setZValue(80.0);
    setCacheMode(QGraphicsItem::NoCache);
    setAcceptedMouseButtons(Qt::NoButton);
}

QRectF MenuOverlayItem::boundingRect() const
{
    return m_rect;
}

void MenuOverlayItem::advanceAnimation(qreal deltaSeconds)
{
    m_animationTime += deltaSeconds;
    if (m_animationTime > 1000.0) {
        m_animationTime = std::fmod(m_animationTime, 1000.0);
    }
    update();
}

void MenuOverlayItem::setSelectedIndex(int index)
{
    if (index == m_selectedIndex) {
        return;
    }
    m_selectedIndex = index;
    update();
}

int MenuOverlayItem::selectedIndex() const
{
    return m_selectedIndex;
}

void MenuOverlayItem::setLanguageOption(int languageOption)
{
    if (m_languageOption == languageOption) {
        return;
    }
    m_languageOption = languageOption;
    update();
}

int MenuOverlayItem::languageOption() const
{
    return m_languageOption;
}

void MenuOverlayItem::setDifficultyOption(int difficultyOption)
{
    if (m_difficultyOption == difficultyOption) {
        return;
    }
    m_difficultyOption = difficultyOption;
    update();
}

int MenuOverlayItem::difficultyOption() const
{
    return m_difficultyOption;
}

void MenuOverlayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    drawSky(painter);
    drawKitchenWindow(painter);
    drawTruck(painter);
    drawStoryPanel(painter);
    drawBottomBar(painter);
}

void MenuOverlayItem::drawSky(QPainter *painter) const
{
    static const QImage cover = menuCover();
    if (!cover.isNull()) {
        QLinearGradient backdrop(0.0, 0.0, 0.0, m_rect.height());
        backdrop.setColorAt(0.0, QColor(255, 239, 247));
        backdrop.setColorAt(0.54, QColor(255, 231, 243));
        backdrop.setColorAt(1.0, QColor(242, 218, 232));
        painter->fillRect(m_rect, backdrop);

        const qreal backgroundScale = qMax(m_rect.width() / cover.width(), m_rect.height() / cover.height());
        const QSizeF backgroundSize(cover.width() * backgroundScale, cover.height() * backgroundScale);
        const QRectF backgroundTarget(m_rect.center().x() - backgroundSize.width() / 2.0,
                                      m_rect.center().y() - backgroundSize.height() / 2.0,
                                      backgroundSize.width(),
                                      backgroundSize.height());
        painter->save();
        painter->setOpacity(0.18);
        painter->drawImage(backgroundTarget, cover);
        painter->restore();

        const QRectF coverArea(82.0, 28.0, m_rect.width() - 164.0, 542.0);
        const qreal scale = qMin(coverArea.width() / cover.width(), coverArea.height() / cover.height());
        const QSizeF targetSize(cover.width() * scale, cover.height() * scale);
        const QRectF target(coverArea.center().x() - targetSize.width() / 2.0,
                            coverArea.center().y() - targetSize.height() / 2.0,
                            targetSize.width(),
                            targetSize.height());

        const QRectF card = target.adjusted(-18.0, -18.0, 18.0, 18.0);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(74, 42, 58, 55));
        painter->drawRoundedRect(card.translated(0.0, 10.0), 24.0, 24.0);
        painter->setBrush(QColor(255, 248, 251, 230));
        painter->drawRoundedRect(card, 24.0, 24.0);

        QPainterPath clipPath;
        clipPath.addRoundedRect(target.adjusted(-2.0, -2.0, 2.0, 2.0), 18.0, 18.0);
        painter->save();
        painter->setClipPath(clipPath);
        painter->drawImage(target, cover);
        QLinearGradient imageGlow(target.left(), target.top(), target.right(), target.bottom());
        imageGlow.setColorAt(0.0, QColor(255, 255, 255, 48));
        imageGlow.setColorAt(0.45, QColor(255, 255, 255, 0));
        imageGlow.setColorAt(1.0, QColor(255, 190, 214, 34));
        painter->fillRect(target, imageGlow);
        painter->restore();

        painter->setPen(QPen(QColor(111, 78, 102, 150), 3.0));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(target.adjusted(-2.0, -2.0, 2.0, 2.0), 18.0, 18.0);

        QLinearGradient shade(0.0, 0.0, 0.0, m_rect.height());
        shade.setColorAt(0.0, QColor(255, 255, 255, 8));
        shade.setColorAt(0.66, QColor(255, 255, 255, 0));
        shade.setColorAt(1.0, QColor(76, 48, 66, 78));
        painter->fillRect(m_rect, shade);
        return;
    }

    QLinearGradient sky(0.0, 0.0, 0.0, m_rect.height());
    sky.setColorAt(0.0, QColor(115, 203, 255));
    sky.setColorAt(0.58, QColor(236, 247, 255));
    sky.setColorAt(1.0, QColor(245, 225, 188));
    painter->fillRect(m_rect, sky);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 190));
    for (int i = 0; i < 5; ++i) {
        const qreal x = 80.0 + i * 260.0 + std::sin(m_animationTime * 0.35 + i) * 8.0;
        const qreal y = 68.0 + (i % 2) * 36.0;
        painter->drawEllipse(QRectF(x, y, 74.0, 30.0));
        painter->drawEllipse(QRectF(x + 36.0, y - 12.0, 82.0, 44.0));
        painter->drawEllipse(QRectF(x + 94.0, y + 2.0, 64.0, 28.0));
    }

    painter->setBrush(QColor(235, 177, 80));
    painter->drawRect(QRectF(0.0, m_rect.height() - 100.0, m_rect.width(), 100.0));
    painter->setBrush(QColor(209, 127, 48));
    painter->drawRect(QRectF(0.0, m_rect.height() - 112.0, m_rect.width(), 14.0));

    drawSpark(painter,
              QPointF(110.0, 165.0 + std::sin(m_animationTime * 1.2) * 4.0),
              22.0,
              QColor(255, 255, 255, 180));
    drawSpark(painter,
              QPointF(m_rect.width() - 126.0, 198.0 + std::cos(m_animationTime * 1.0) * 4.0),
              18.0,
              QColor(255, 244, 180, 160));
}

void MenuOverlayItem::drawKitchenWindow(QPainter *painter) const
{
    static const QImage cover = menuCover();
    if (!cover.isNull()) {
        return;
    }

    static const QImage preview = menuPreview();
    const QRectF frame(194.0, 192.0, 818.0, 388.0);
    drawHardPanel(painter, frame, QColor(248, 233, 198), QColor(93, 64, 48));

    if (preview.isNull()) {
        Theme::text(painter, frame.adjusted(30.0, 40.0, -30.0, -40.0), QStringLiteral("Kitchen Preview"), 24, true, QColor(92, 62, 46));
        return;
    }

    const QRectF inner = frame.adjusted(18.0, 18.0, -18.0, -18.0);
    const qreal scale = qMin(inner.width() / preview.width(), inner.height() / preview.height());
    const QSizeF targetSize(preview.width() * scale, preview.height() * scale);
    const QRectF target(inner.center().x() - targetSize.width() / 2.0,
                        inner.center().y() - targetSize.height() / 2.0 + std::sin(m_animationTime * 0.8) * 2.0,
                        targetSize.width(),
                        targetSize.height());
    painter->drawImage(target, preview);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 255, 255, 34));
    painter->drawRect(QRectF(inner.left(), inner.top(), inner.width(), 38.0));
}

void MenuOverlayItem::drawTruck(QPainter *painter) const
{
    static const QImage cover = menuCover();
    if (!cover.isNull()) {
        return;
    }

    const QRectF logo(218.0, 38.0, 770.0, 128.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(80, 46, 30, 80));
    painter->drawRoundedRect(logo.translated(6.0, 8.0), 12.0, 12.0);
    painter->setBrush(QColor(255, 151, 37));
    painter->drawRoundedRect(logo, 12.0, 12.0);
    painter->setBrush(QColor(219, 76, 28));
    painter->drawRoundedRect(QRectF(logo.left() + 18.0, logo.bottom() - 30.0, logo.width() - 36.0, 18.0), 8.0, 8.0);

    Theme::text(painter,
                QRectF(logo.left() + 36.0, logo.top() + 20.0, logo.width() - 72.0, 54.0),
                QStringLiteral("胡闹厨房 overcooked!"),
                30,
                true,
                QColor(255, 255, 246));
    Theme::text(painter,
                QRectF(logo.left() + 50.0, logo.top() + 78.0, logo.width() - 100.0, 22.0),
                m_languageOption == ChineseLanguage ? QStringLiteral("寿司合作厨房") : QStringLiteral("Sushi Co-op Kitchen"),
                13,
                true,
                QColor(86, 49, 30));
}

void MenuOverlayItem::drawStoryPanel(QPainter *painter) const
{
    const bool zh = m_languageOption == ChineseLanguage;
    const QStringList labels = zh
                                   ? QStringList{QStringLiteral("语言"),
                                                 QStringLiteral("难度"),
                                                 QStringLiteral("开始"),
                                                 QStringLiteral("退出")}
                                   : QStringList{QStringLiteral("Language"),
                                                 QStringLiteral("Difficulty"),
                                                 QStringLiteral("Start"),
                                                 QStringLiteral("Exit")};
    const QStringList values = {
        zh ? QStringLiteral("中文") : QStringLiteral("English"),
        difficultyLabel(m_languageOption, m_difficultyOption),
        zh ? QStringLiteral("开始游戏") : QStringLiteral("Play"),
        zh ? QStringLiteral("退出") : QStringLiteral("Quit")
    };

    const QRectF panel(164.0, 598.0, 878.0, 92.0);
    drawHardPanel(painter, panel, QColor(255, 244, 222, 244), QColor(87, 58, 43));

    for (int i = 0; i < labels.size(); ++i) {
        const QRectF button(panel.left() + 22.0 + i * 211.0, panel.top() + 18.0, 184.0, 42.0);
        const bool selected = i == m_selectedIndex;
        drawHardPanel(painter,
                      button,
                      selected ? QColor(255, 223, 95) : QColor(255, 252, 239),
                      selected ? QColor(181, 74, 35) : QColor(161, 104, 70));
        Theme::text(painter,
                    QRectF(button.left() + 12.0, button.top() + 6.0, 76.0, 12.0),
                    labels.at(i),
                    8,
                    true,
                    QColor(105, 66, 42),
                    Qt::AlignLeft | Qt::AlignVCenter);
        Theme::text(painter,
                    QRectF(button.left() + 72.0, button.top() + 12.0, 96.0, 18.0),
                    values.at(i),
                    11,
                    true,
                    QColor(66, 42, 30),
                    Qt::AlignRight | Qt::AlignVCenter);
    }

    Theme::text(painter,
                QRectF(panel.left() + 40.0, panel.bottom() - 24.0, panel.width() - 80.0, 14.0),
                difficultyDescription(m_languageOption, m_difficultyOption),
                8,
                true,
                QColor(104, 70, 48));
}

void MenuOverlayItem::drawBottomBar(QPainter *painter) const
{
    const QRectF hint(372.0, 704.0, 462.0, 26.0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(80, 48, 34, 150));
    painter->drawRoundedRect(hint, 5.0, 5.0);
    Theme::text(painter,
                hint,
                m_languageOption == ChineseLanguage
                    ? QString::fromUtf8("A/D \xE9\x80\x89\xE6\x8B\xA9   W/S \xE5\x88\x87\xE6\x8D\xA2   Enter \xE7\xA1\xAE\xE8\xAE\xA4   Esc \xE8\xBF\x94\xE5\x9B\x9E")
                    : QStringLiteral("A/D Select   W/S Change   Enter Confirm   Esc Back"),
                8,
                true,
                QColor(255, 246, 226));
    return;

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(80, 48, 34, 150));
    painter->drawRoundedRect(hint, 5.0, 5.0);
    Theme::text(painter,
                hint,
                m_languageOption == ChineseLanguage
                    ? QStringLiteral("W/S 选择   A/D 切换   Enter 确认   Esc 返回")
                    : QStringLiteral("A/D Select   W/S Change   Enter Confirm   Esc Back"),
	                8,
	                true,
	                QColor(255, 246, 226));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(80, 48, 34, 150));
    painter->drawRoundedRect(hint, 5.0, 5.0);
    Theme::text(painter,
                hint,
                m_languageOption == ChineseLanguage
                    ? QStringLiteral("A/D 选择   W/S 切换   Enter 确认   Esc 返回")
                    : QStringLiteral("A/D Select   W/S Change   Enter Confirm   Esc Back"),
                8,
                true,
                QColor(255, 246, 226));
}

void MenuOverlayItem::drawBunting(QPainter *) const
{
}

void MenuOverlayItem::drawDecorFood(QPainter *) const
{
}

void MenuOverlayItem::drawMascot(QPainter *) const
{
}
