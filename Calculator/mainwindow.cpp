#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <vector>
#include <stack>

struct Token
{
    enum tokenType {Operand, Operator, Parentheses} type;
    QString tokenContent;
    double val;

    Token(tokenType type, const QString& content):type(type),tokenContent(content),val(0){}
    Token(tokenType type, double value):type(type),tokenContent(QString::number(value)), val(value){}

};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    ui->Display->setText("0");

    myButtons = ui->buttonContainer->findChildren<QPushButton*>();
    qDebug()<<"Button size "<<myButtons.size();
    for(auto* button : myButtons)
    {
        qDebug() << button->objectName();
        if(button == ui->Ac)
        {
            connect(button, SIGNAL(clicked()), this, SLOT(clearDisplay()));
        }
        else if(button == ui->Del)
        {
            connect(button, SIGNAL(clicked()), this, SLOT(deletePressed()));
        }
        else if(button == ui->Equal)
        {
            connect(button, SIGNAL(clicked()), this, SLOT(equalPressed()));
        }
        else
        {
            connect(button, SIGNAL(clicked()), this, SLOT(inputPressed()));
        }

    }

}


void MainWindow::clearDisplay()
{
    ui->Display->setText("0");
}

void MainWindow::inputPressed()
{
    QPushButton* senderButton = reinterpret_cast<QPushButton *>(sender());

    QString buttonTxt = senderButton->text();
    QString displayTxt = ui->Display->text();

    if( displayTxt == "0" || displayTxt == "0.0" || displayTxt.isEmpty())
    {
        ui->Display->setText(buttonTxt);
    }
    else
    {
        displayTxt += buttonTxt;
        ui->Display->setText(displayTxt);
    }
}


bool MainWindow::validateString(const QString& str)
{
    QChar firstChar = str.front();
    if (firstChar == '*' || firstChar == '/' || firstChar == ')')
        return false;
    QChar lastChar = str.back();
    if (lastChar == '*' || lastChar == '/' || lastChar == '(' || lastChar == '-' || lastChar == '+')
        return false;

    //stack to validate parentheses
    std::stack<QChar> myStack;

    for(uint8_t i = 0;  i < str.size() ; i++)
    {
        //consecutive operators not allowed
        if(str.at(i) == '*' || str.at(i) == '-' || str.at(i) == '+' || str.at(i) == '/')
        {
            if ( (i < str.size() - 1) && ( str.at(i + 1) == '*' || str.at(i + 1) == '-' || str.at(i + 1) == '+' || str.at(i + 1) == '/'))
                return false;
        }

        if(str.at(i) == '(')
        {
            //empty brackets not allowed
            if (i < str.size() - 1 && str.at(i + 1) == ')')
                return false;

            myStack.push(str.at(i));
        }
        else if (str.at(i) == ')')
        {
            //closing brackets before opening one
            if(myStack.size() == 0) return false;

            if (myStack.top() == '(')
            {
                myStack.pop();
            }
            else
                return false;
        }
    }

    return myStack.empty();
}

void MainWindow::equalPressed()
{
    qDebug()<<" equal sign pressed , evaluate string";

    /**
     * tokenisation : get a list of chars of the string expression
     * validate the expression : make sure only valid charachters, no consecutive oeprators, no empty brackets, cant start or end with * / , balanced brackets
     * convert to Reverse polish notation (postfix notation)
     * evaluate using stack
     * display value in line edit
     * */

    QString displayTxt = ui->Display->text();
    qDebug()<< (validateString(displayTxt) ? "valid string" : "invalid String");


 /*  std::vector<Token> tokens;
    tokens.reserve(displayTxt.size());

    for(const QChar& c : displayTxt)
    {
        if(c == '*' || c == '/' || c == '-' || c == '+')
        {
            tokens.push_back(Token(tokenType::Operator, c));
        }
    }
*/
}

void MainWindow::deletePressed()
{
    QString displayTxt = ui->Display->text();
    if(displayTxt.isEmpty() || displayTxt == "0" || displayTxt == "0.0")
        return;
    if(displayTxt.size() == 1)
        ui->Display->setText("0");
    else
        ui->Display->setText(displayTxt.mid(0,displayTxt.size()-1));
}

MainWindow::~MainWindow()
{
    delete ui;
}
