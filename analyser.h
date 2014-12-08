#ifndef ANALYSER_H
#define ANALYSER_H

#include "defines.h"
#include <QString>
#include <QDebug>
#include <QProgressBar>
#include <math.h>
#include <QTime>

class Analyser{
public:
    Analyser(const QString value);

    //void detectCopy();
    void detectVariables();
    void run();

    void setPb(QProgressBar *value);

private:
    inline bool isVariable(const QChar &c) const;
    inline bool isOperator(const QChar &c) const;
    inline bool isBit(const QChar &c) const;
    inline bool isBracket(const QChar &c) const;

    inline bool isGoodBinary(const QString &s) const;
    inline bool isGoodUnary(const QString &s) const;

    inline QString dec2bin(long long i, int num);

    inline QChar eval(QString q);

    QString s;

    QString input;

    QString variables;

    QProgressBar *pb;


};

#endif // ANALYSER_H
