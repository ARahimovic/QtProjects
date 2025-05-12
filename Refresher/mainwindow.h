#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *myButton;
    QProgressBar *myBar;
    QSlider *mySlider;
    int counter = 0;

private slots:
    void windowButtonClicked(bool checked);

signals:
    void counterReached();


};
#endif // MAINWINDOW_H
