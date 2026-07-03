#ifndef SPLASHWIDGET_H
#define SPLASHWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>

class SplashWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SplashWidget();
    void fadeOutAndQuit();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void loadLogo();
    QPixmap m_logo;
    QString m_osDisplay;
    QTimer m_spinnerTimer;
    int m_spinnerAngle = 0;
};

#endif
