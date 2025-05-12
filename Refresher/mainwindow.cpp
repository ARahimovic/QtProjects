#include "mainwindow.h"
#include "./ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(500,500);

    myButton = new QPushButton("init Text", this);
    myButton->setCheckable(true);
    myButton->setGeometry(10,10,120,50);
    QFont myFont("Courier");
    myButton->setFont(myFont);

    myBar = new QProgressBar(this);
    myBar->setRange(0,100);
    myBar->setValue(0);
    myBar->setGeometry(10,100, 180, 30);

    mySlider = new QSlider(this);
    mySlider->setRange(0,100);
    mySlider->setOrientation(Qt::Horizontal);
    mySlider->setValue(0);
    mySlider->setGeometry(10,250,180,30);


    connect(myButton, SIGNAL (clicked(bool)), this, SLOT (windowButtonClicked(bool)));
    connect(this, SIGNAL (counterReached()), QApplication::instance(), SLOT (quit()));
    connect(mySlider, SIGNAL (valueChanged(int)), myBar, SLOT (setValue(int)) );
}

void MainWindow::windowButtonClicked(bool checked)
{
    if(checked)
    {
        myButton->setText("Checked");
    }
    else
        myButton->setText("HelloWorld");

    counter ++;
    qDebug() << "coutner value : " << counter;
    if(counter >= 10)
    {
        qDebug() << "Emitting counterReached signal";
        emit counterReached();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
