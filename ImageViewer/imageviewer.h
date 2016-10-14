#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include<QScrollBar>

#include<QDragEnterEvent>
#include<QDragLeaveEvent>
#include<QDragMoveEvent>
#include<QDropEvent>
#include<QLabel>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
signals:
    void changed(const QMimeData *mimeData = 0);

private slots:
    void on_openAct_triggered();

    void on_zoomInAct_triggered();

    void on_zoomOutAct_triggered();

    void on_fitToWindowAct_triggered();

    void on_normalSizeAct_triggered();

    void on_quitAct_triggered();

    void on_printAct_triggered();

    void on_rotClockwiseAct_triggered();

    void on_rotAntiClockwiseAct_triggered();

    void on_actionAbout_Software_triggered();

private:

    Ui::ImageViewer *ui;
    double scaleFactor;
    void updateActions();
    void scaleImage(double);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void normalSize();

    /* drag and  drop events definations */

    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event)  Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event)  Q_DECL_OVERRIDE;


};

#endif // IMAGEVIEWER_H
