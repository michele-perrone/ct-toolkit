#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openUncompressedImage();
    void openCompressedImage();
    void createAboutCTDialog();
    void createAboutMeDialog();
    void createAboutQtDialog();
    void exitProgram();

private:
    Ui::MainWindow *ui;

    void setupMenuBar();

    uint16_t IMG [512][512];
    unsigned char LUT [256][3];
    short int compression_parameter;
    QString IMG_filename;
    QString LUT_filename;
    QImage loadedIMG;
};

#endif // MAINWINDOW_H
