#include "splashwidget.h"

#include <QPainter>
#include <QScreen>
#include <QGuiApplication>
#include <QSvgRenderer>
#include <QSettings>
#include <QPropertyAnimation>

SplashWidget::SplashWidget()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setCursor(Qt::BlankCursor);

    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen)
        setGeometry(screen->geometry());

    loadLogo();

    connect(&m_spinnerTimer, &QTimer::timeout, this, [this]() {
        m_spinnerAngle = (m_spinnerAngle + 6) % 360;
        update();
    });
    m_spinnerTimer.start(16);
}

void SplashWidget::loadLogo()
{
    QSettings osRelease("/etc/os-release", QSettings::IniFormat);
    m_osDisplay = osRelease.value("PRETTY_NAME", "Lingmo OS").toString();

    QStringList logoPaths = {
        "/usr/share/lingmo/distribution/distribution_logo.svg",
        "/usr/share/lingmo/distribution/distribution_logo_transparent.svg",
        "/usr/share/icons/hicolor/scalable/apps/deepin-launcher.svg",
    };

    for (const QString &path : logoPaths) {
        QSvgRenderer renderer(path);
        if (!renderer.isValid())
            continue;
        QSize defaultSize = renderer.defaultSize();
        int logoWidth = qMin(defaultSize.width(), 260);
        int logoHeight = defaultSize.height() * logoWidth / defaultSize.width();
        QSize size(logoWidth, logoHeight);
        m_logo = QPixmap(size);
        m_logo.fill(Qt::transparent);
        QPainter p(&m_logo);
        renderer.render(&p, QRect(QPoint(0, 0), size));
        p.end();
        break;
    }

    if (m_logo.isNull())
        m_logo = QPixmap(1, 1);
        m_logo.fill(Qt::transparent);
    }
}

void SplashWidget::fadeOutAndQuit()
{
    m_spinnerTimer.stop();

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity", this);
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);

    connect(anim, &QPropertyAnimation::finished, this, [this]() {
        close();
        qApp->quit();
    });

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void SplashWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#1a1a1a"));

    int cx = width() / 2;
    int cy = height() / 2;

    // logo
    if (!m_logo.isNull()) {
        painter.drawPixmap(cx - m_logo.width() / 2, cy - m_logo.height() / 2 - 40, m_logo);
    }

    // OS name
    QFont font;
    font.setPixelSize(28);
    font.setWeight(QFont::Light);
    painter.setFont(font);
    painter.setPen(QColor("#cccccc"));

    QFontMetrics fm28(font);
    painter.drawText(cx - fm28.horizontalAdvance(m_osDisplay) / 2, cy + 80, m_osDisplay);

    // spinning ring
    QPen pen(QColor("#888888"), 3);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    int ringRadius = 18;
    int ringY = cy + 120;

    painter.drawArc(cx - ringRadius, ringY - ringRadius,
                    ringRadius * 2, ringRadius * 2,
                    m_spinnerAngle * 16, 270 * 16);
}

void SplashWidget::mousePressEvent(QMouseEvent *)
{
    fadeOutAndQuit();
}
