#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QPixmap>
#include <QWidget>

namespace Ui {
    class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QPixmap pixmap, QWidget *parent = 0);
    ~ImageWidget();

    void setPixmap(QPixmap pixmap);

private:
    Ui::ImageWidget *ui;
    QPixmap pixmap;
};

#endif // IMAGEWIDGET_H
