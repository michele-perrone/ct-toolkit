#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../app/imgtools.hpp"
#include "../app/quadtree.hpp"

#include <QFileDialog>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QScreen>
#include <QImage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Main window GUI setup
    ui->setupUi(this);
    this->resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 4);
    this->setWindowTitle("Computer tomography toolkit");
    setupMenuBar();

    LUT_filename = "../BarvnePalete/32_colors.lut";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenuBar()
{
    // File
    connect(ui->actionLoad_uncompressed_image, &QAction::triggered, this, &MainWindow::openUncompressedImage);
    connect(ui->actionLoad_compressed_image, &QAction::triggered, this, &MainWindow::openCompressedImage);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitProgram);

    // Help
    connect(ui->actionAbout_CT_Toolkit, &QAction::triggered, this, &MainWindow::createAboutCTDialog);
    connect(ui->actionAbout_me, &QAction::triggered, this, &MainWindow::createAboutMeDialog);
    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::createAboutQtDialog);
}

void MainWindow::openUncompressedImage()
{
    IMG_filename = QFileDialog::getOpenFileName(this);
    if (!IMG_filename.isEmpty())
    {
        ui->textBrowser->setText(IMG_filename);

        // Create a grayscale BitMap from the IMG matrix
        IMGTools::load_IMG(IMG_filename.toStdString().c_str(), &IMG);
        QImage myIMG((uint8_t*)IMG,
                     512, 512, 512*sizeof(uint16_t),
                     QImage::Format_Grayscale16);

        // Store the LUT color matrix
        IMGTools::load_LUT(LUT_filename.toStdString().c_str(), &LUT);

        // Convert the IMG into a color bitmap
        QImage myColorIMG((uint8_t*)IMGTools::IMG_to_color_BMP(&IMG, &LUT).pixel,
                          512, 512, 512*sizeof(uint8_t)*3,
                          QImage::Format_RGB666);

        // Display the color image
        ui->imageLabel->setPixmap(QPixmap::fromImage(myColorIMG));
    }
    return;

}

void MainWindow::openCompressedImage()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        ui->textBrowser->setText(fileName);
}

void MainWindow::createAboutCTDialog()
{
    QMessageBox aboutCTDialog(this);

    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = QMessageBox::tr(
        "<h3>About CT toolkit</h3>");
    QString translatedTextAboutQtText = QMessageBox::tr(
        "<p>This program provides a basic computer tomography toolkit.</p>"
        "<p>It is intended to be used free of charge.</p>");
    aboutCTDialog.setWindowTitle("About CT toolkit");
    aboutCTDialog.setText(translatedTextAboutQtCaption);
    aboutCTDialog.setInformativeText(translatedTextAboutQtText);
    //QPixmap pm(QLatin1String(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
    //if (!pm.isNull())
    //    aboutCTDialog.setIconPixmap(pm);

    aboutCTDialog.exec();
}

void MainWindow::createAboutMeDialog()
{
    QMessageBox aboutMeDialog(this);

    QString translatedTextAboutQtCaption;
    translatedTextAboutQtCaption = QMessageBox::tr(
        "<h3>About me</h3>");
    QString translatedTextAboutQtText = QMessageBox::tr(
        "<p>My name is Michele Perrone.</p>"
        "<p>I am a computer engineer.</p>"
        "<p>This is my first desktop app, in which I am tring"
        " to adhere to the best coding and user interface practices.</p>");
    aboutMeDialog.setWindowTitle("About me");
    aboutMeDialog.setText(translatedTextAboutQtCaption);
    aboutMeDialog.setInformativeText(translatedTextAboutQtText);
    //QPixmap pm(QLatin1String(":/qt-project.org/qmessagebox/images/qtlogo-64.png"));
    //if (!pm.isNull())
    //    aboutCTDialog.setIconPixmap(pm);

    aboutMeDialog.exec();
}

void MainWindow::createAboutQtDialog()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::exitProgram()
{
    exit(0);
}
