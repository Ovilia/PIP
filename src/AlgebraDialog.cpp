#include <QFileDialog>

#include "AlgebraDialog.h"
#include "ui_AlgebraDialog.h"

#include "Algebra.h"
#include "ImagePolicy.h"

AlgebraDialog::AlgebraDialog(MainWindow* mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlgebraDialog),
    mainWindow(mainWindow),
    maskImage(0)
{
    ui->setupUi(this);
}

AlgebraDialog::~AlgebraDialog()
{
    delete ui;
    if (maskImage) {
        delete maskImage;
    }
}

void AlgebraDialog::on_openButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this, tr("Open an Image"), QDir::currentPath(),
                tr("Image files(*.bmp *.jpeg *.jpg *.png *.gif *.tif);;"\
                   "All files (*.*)"));
    if (!imagePath.isNull()) {
        ui->applyButton->setEnabled(true);
        ui->pathLabel->setText(imagePath);
    }
}

void AlgebraDialog::on_applyButton_clicked()
{
    ImagePolicy::ScalePolicy scalePolicy;
    if (ui->nearRadio->isChecked()) {
        scalePolicy = ImagePolicy::SP_NEAREST;
    } else {
        scalePolicy = ImagePolicy::SP_BILINEAR;
    }

    ImagePolicy::AlgebraPolicy algePolicy;
    if (ui->addRadio->isChecked()) {
        algePolicy = ImagePolicy::AP_ADD;
    } else if (ui->minusRadio->isChecked()) {
        algePolicy = ImagePolicy::AP_MINUS;
    } else if (ui->multiplyRadio->isChecked()) {
        algePolicy = ImagePolicy::AP_MULTIPLY;
    } else {
        algePolicy = ImagePolicy::AP_DIVIDE;
    }

    if (maskImage) {
        delete maskImage;
    }
    maskImage = new QImage(ui->pathLabel->text());

    QImage* origin = mainWindow->getImageProcessor()->getOriginImage();
    QImage* result = Algebra::getAlgebraImage(origin, maskImage,
                                              algePolicy, scalePolicy);
    delete maskImage;
    maskImage = 0;

    mainWindow->setAlgebraImage(result);
}
