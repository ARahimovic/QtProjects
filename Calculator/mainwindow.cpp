#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <vector>
#include <stack>


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


void MainWindow::equalPressed()
{

    QString displayTxt = ui->Display->text();
    qDebug()<< (validateString(displayTxt) ? "valid string" : "invalid String");

    if(!validateString(displayTxt))
        return;


    auto myTokens = tokenize(displayTxt);
    auto postFix = convertToPostFix(myTokens);
    double result = evaluatePostFix(postFix);

    ui->Display->setText(QString::number(result, 'g', 16));


#ifdef DEBUGGIN
    qDebug()<<" equal sign pressed , evaluate string";
    qDebug()<<"token size " << myTokens.size();
    for(const Token& token : myTokens)
    {
        qDebug()<< "tokenType " << (token.type == tokenType::Number ? " number" : "operator")
                 << "token Val = " << token.val << "tokenString = " << token.op;
    }

    qDebug()<<"postfix size " << postFix.size();
    for(const Token& token : postFix)
    {
        qDebug()<< "tokenType " << (token.type == tokenType::Number ? " number" : "operator")
                 << "token Val = " << token.val << "tokenString = " << token.op;
    }
#endif

}


bool MainWindow::validateString(const QString& str)
{
    QChar firstChar = str.front();
    //should not start with operation
    if (firstChar == '*' || firstChar == '/' || firstChar == ')' || firstChar == '%' || firstChar == '.')
        return false;
    QChar lastChar = str.back();
    //should not end with operation
    if (lastChar == '*' || lastChar == '/' || lastChar == '(' || lastChar == '-' || lastChar == '+' || lastChar == '%' || lastChar == '.')
        return false;

    //stack to validate parentheses
    std::stack<QChar> myStack;
    bool hasDecimal = false;

    for(uint8_t i = 0;  i < str.size() ; i++)
    {
        QChar currentChar = str.at(i);
        //consecutive operators not allowed
        if(currentChar == '*' || currentChar == '-' || currentChar == '+' || currentChar == '/')
        {
            if ( (i < str.size() - 1) && ( str.at(i + 1) == '*' || str.at(i + 1) == '-' || str.at(i + 1) == '+' || str.at(i + 1) == '/'))
                return false;
        }


        else if(currentChar == '.')
        {
            // decimal point should be preceeded and followed by a digit
            if( !str.at(i + 1).isDigit() || !str.at(i - 1).isDigit())
                return false;
            //can only be one decimal point in a number
            if(hasDecimal)
                return false;

            hasDecimal = true;
        }

        else if (currentChar.isDigit())
        {
          //if digit is a new number, resert decimal flag
            if (i == 0 || (!str.at(i - 1).isDigit() && str.at(i - 1) != '.') ) {
                hasDecimal = false;
            }
        }

        else if(currentChar == '(')
        {
            //empty brackets not allowed
            if (i < str.size() - 1 && str.at(i + 1) == ')')
                return false;

            myStack.push(str.at(i));
        }
        else if (currentChar == ')')
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


std::vector<Token> MainWindow::tokenize(const QString& txt)
{
    std::vector<Token> tokens;
    QString number;
    for(const QChar& c : txt)
    {
        if(c.isDigit() || c == '.')
        {
            //append it to number
            number += c;
        }
        else if(c == '*' || c == '/' || c == '-' || c == '+' || c == '%' || c == '(' || c == ')')
        {
            if(!number.isEmpty())
            {
                tokens.push_back({tokenType::Number, number.toDouble()});
                number.clear();
            }

            tokens.push_back({tokenType::Operator, 0, c});
        }

    }

    //the last characters where digits we need to push the number
    if(!number.isEmpty())
    {
        tokens.push_back({tokenType::Number, number.toDouble()});
        number.clear();
    }

    return tokens;
}

uint8_t precendece(QChar ch)
{
    if(ch == '*' || ch == '/')
        return 2;
    else if (ch == '+' || ch == '-')
        return 1;

    return 0;
}

std::vector<Token> MainWindow::convertToPostFix(const std::vector<Token>& tokens)
{
    std::vector<Token> postFixTokens;
    std::stack<Token> opStack;

    for(const Token& token : tokens)
    {
        if(token.type == tokenType::Number)
        {
            postFixTokens.push_back(token);
        }

        else if(token.type == tokenType::Operator)
        {
            if(token.op == '(')
                opStack.push(token);

            else if(token.op == ')')
            {
                while( !opStack.empty() && opStack.top().op != '(')
                {
                    postFixTokens.push_back(opStack.top());
                    opStack.pop();
                }
                if (!opStack.empty() && opStack.top().op == '(')
                {
                    opStack.pop(); // Remove '(' from the stack (but don't add to output)
                }
            }
            else
            {
                //if the stack is not empty, and the top element has higher precendence than current operator,
                while(!opStack.empty() && ( precendece(opStack.top().op) >= precendece(token.op) ) )
                {
                    //push top element it into queue
                    postFixTokens.push_back(opStack.top());
                    //remove it from stack
                    opStack.pop();
                }
                opStack.push(token);
            }

        }
    }

    //pop all the remaining operatos from the stack into the queue
    while(!opStack.empty())
    {
        //push it into queue
        postFixTokens.push_back(opStack.top());
        //remove it from stack
        opStack.pop();
    }

    return postFixTokens;
}


double MainWindow::evaluatePostFix(const std::vector<Token>& postFix)
{

    std::stack<double> operandsStack;

    for(const Token& token : postFix)
    {
        if(token.type == tokenType::Number)
            operandsStack.push(token.val);
        else
        {
            //top of stack is right operand
            double b = operandsStack.top();
            operandsStack.pop();

            double a = operandsStack.top();
            operandsStack.pop();

            QChar ch = token.op;

            if(ch == '*')
                operandsStack.push(a * b);
            else if (ch == '/')
                operandsStack.push(a/b);
            else if (ch == '-')
                operandsStack.push(a-b);
            else if(ch == '+')
                operandsStack.push(a+b);
            else if (ch == '%')
                operandsStack.push((int)a % (int)b);
        }
    }

    return operandsStack.top();
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
