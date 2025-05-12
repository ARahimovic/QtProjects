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
    bool validateString(const QString& str);
    Ui::MainWindow *ui;
    QList<QPushButton*> myButtons;
    double calcVal = 0;
    bool isRigtparentheses = true;


private slots:
    void inputPressed();
    void clearDisplay();
    void equalPressed();
    void deletePressed();
};
#endif // MAINWINDOW_H
