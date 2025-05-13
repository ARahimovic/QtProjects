#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <vector>

enum class tokenType
{
    Number,
    Operator,
};

struct Token
{
    tokenType type;
    double val;
    QChar op ='n';
};


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

    /**
     * @brief validateString check if the user input is valid :
     * only valid characters, no consecutive oeprators, no empty brackets, cant start or end with * / , balanced brackets
     * @param userInput
     * @return true if the string is valid and is ready for tokenization
     */
    bool validateString(const QString& userInput);
    /**
     * @brief tokenize : create a vector of tokens of the numbers and operators, to make it easy for converstion to postfix notation
     * @param validInput : takes the valid user input
     * @return vector of tokens
     */
    std::vector<Token> tokenize(const QString& validInput);
    /**
     * @brief convertToPostFix : we convert the infix notation into postfix for easy calculcation using the shunting yard algorithm
     * @param infixTokens : takes the infix tokens
     * @return postfix tokens
     */
    std::vector<Token> convertToPostFix(const std::vector<Token>& infixTokens);
    /**
     * @brief evaluatePostFix : evaluate the postfix expression and return the result
     * @param postfixTokens
     * @return return the result
     */
    double evaluatePostFix(const std::vector<Token>& postfixTokens);
    Ui::MainWindow *ui;
    QList<QPushButton*> myButtons;

private slots:
    void inputPressed();
    void clearDisplay();
    void equalPressed();
    void deletePressed();
};
#endif // MAINWINDOW_H
