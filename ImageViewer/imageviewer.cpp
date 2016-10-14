#include "imageviewer.h"
#include "ui_imageviewer.h"
#include<QDebug>
#include<QDialog>
#include<QMessageBox>
#include<QPixmap>
#include<QFileDialog>
#include<QScrollBar>
#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include<QLabel>
#include<QDrag>
#include<QMimeData>

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);

    //ui->label_image->setBackgroundRole(QPalette::Base);
    ui->label_image->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    ui->label_image->setScaledContents(true);  // if !true then only label will increase (while zoom in) not the pixmap
    ui->label_image->setAcceptDrops(true);

    ui->scrollArea = new QScrollArea;
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(ui->label_image);

    setCentralWidget(ui->scrollArea);
    ui->scrollArea->setAcceptDrops(true);

    setWindowTitle(tr("Image Viewer"));
    qDebug()<<"All set up now choose file";
    resize(600, 500);

}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::on_openAct_triggered()
{
    qDebug()<<"open() trigerred";

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
             QImage image(fileName);
             if (image.isNull())
             {
                 QMessageBox::information(this, tr("Image Viewer"),
                                          tr("Cannot load %1.").arg(fileName));
                 qDebug()<<"cannot displayed this picture";

                 return;
             }
             ui->label_image->setPixmap(QPixmap::fromImage(image));
             scaleFactor = 1.0;

             ui->printAct->setEnabled(true);
             ui->fitToWindowAct->setEnabled(true);
             updateActions();

             if (!ui->fitToWindowAct->isChecked())
                 ui->label_image->adjustSize();

             qDebug()<<"picture displayed";
        }
}

void ImageViewer::on_zoomInAct_triggered()
{
    scaleImage(1.25);
}

void ImageViewer::on_zoomOutAct_triggered()
{
    scaleImage(0.8);
}

void ImageViewer::on_fitToWindowAct_triggered()
{
    bool fitToWindow=ui->fitToWindowAct->isChecked();
    ui->scrollArea->setWidgetResizable(fitToWindow);
    if(!fitToWindow)
    {
        normalSize();
    }
    updateActions();

}

void ImageViewer::on_normalSizeAct_triggered()
{
    ui->label_image->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::normalSize()
{
    ui->label_image->adjustSize();
    scaleFactor = 1.0;
}


void ImageViewer::updateActions()
{
    ui->zoomInAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->zoomOutAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->normalSizeAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->rotClockwiseAct->setEnabled(!ui->fitToWindowAct->isChecked());
    ui->rotAntiClockwiseAct->setEnabled(!ui->fitToWindowAct->isChecked());

}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(ui->label_image->pixmap());
    scaleFactor *= factor;
    ui->label_image->resize(scaleFactor * ui->label_image->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);

    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    ui->zoomInAct->setEnabled(scaleFactor < 3.0);   //can control the zooming limit
    ui->zoomOutAct->setEnabled(scaleFactor > 0.333);//can control the zoomout limit
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::on_quitAct_triggered()
{
    qApp->quit();
}

void ImageViewer::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"drag enter event in action";
    event->acceptProposedAction();
    ui->label_image->setText(tr("<drop content here>"));
   // ui->label_image->setBackgroundRole(QPalette::Highlight);
    emit changed(event->mimeData());
//    ui->label_image->setText("Drop image here");

}


void ImageViewer::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug()<<"drag move event in action";
    //ui->label_image->setText("Drop image here");
    event->acceptProposedAction();
    emit changed(event->mimeData());
}
void ImageViewer::dropEvent(QDropEvent *event)
{
    qDebug()<<"drop event in action";
    //ui->label_image->acceptDrops(true);

    ui->label_image->setText("");

    const QMimeData *mimeData=event->mimeData();
    if(event->mimeData()->hasImage())
    {
        event->accept();
        qDebug()<<"inside here";
        //QPixmap pix= QLabel::setPixmap(mimeData->imageData());
        QImage image = qvariant_cast<QImage>(event->mimeData()->imageData());
        //ui->label_image->setPixmap(QPixmap::fromImage(image));
         //setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
        ui->label_image->setPixmap(qvariant_cast<QPixmap>(event->mimeData()->imageData()));
        ui->label_image->show();

        event->acceptProposedAction();
    }
    else
    {
        qDebug()<<"cannot drop this file";
    }

}


void ImageViewer::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug()<<"drag leave event in action";
    event->accept();
}



void ImageViewer::on_printAct_triggered()
{

}

void ImageViewer::on_rotClockwiseAct_triggered()
{
    QPixmap pixmap(*ui->label_image->pixmap());
    QTransform rm;
    rm.rotate(90);
    pixmap = pixmap.transformed(rm);
    ui->label_image->setPixmap(pixmap);
}

void ImageViewer::on_rotAntiClockwiseAct_triggered()
{
    QPixmap pixmap(*ui->label_image->pixmap());
    QTransform rm;
    rm.rotate(-1*90);
    pixmap = pixmap.transformed(rm);
    ui->label_image->setPixmap(pixmap);
}

void ImageViewer::on_actionAbout_Software_triggered()
{
   // QString link="www.google.com";
    QMessageBox::information(this,tr("About this software"),tr("Developed by Ravi with QT 5.5"));
}
