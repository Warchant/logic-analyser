#ifndef DEFINES_H
#define DEFINES_H

#include <QString>

const QChar EQ  = QString::fromWCharArray(L"~")[0];
const QChar NOT = QString::fromWCharArray(L"¬")[0];
const QChar AND = QString::fromWCharArray(L"∧")[0];
const QChar XOR = QString::fromWCharArray(L"⊕")[0];
const QChar OR  = QString::fromWCharArray(L"∨")[0];
const QChar IMP = QString::fromWCharArray (L"→")[0];

const QChar ONE  = QString::fromWCharArray (L"1")[0];
const QChar ZERO = QString::fromWCharArray (L"0")[0];

const QChar LB = QString::fromWCharArray (L"(")[0];
const QChar RB = QString::fromWCharArray (L")")[0];



class AnalysisError
{
public:

    enum EXCEPTIONS
    {
        WORD,
        EMPTY_INPUT
    };

    AnalysisError(EXCEPTIONS type)
    {
        this->type = type;
    }

    AnalysisError(EXCEPTIONS type, int position, QChar character)
    {
        this->type = type;
        this->position = position;
        this->character = character;
    }

    operator EXCEPTIONS()
    {
        return type;
    }


    EXCEPTIONS type;
    QString character; // symbol
    int position;


};
#endif // DEFINES_H
