#include "analyser.h"

Analyser::Analyser(const QString value)
{
    this->s = value;
    QRegExp spaces("([ \t\n])");
    s.replace(spaces,"");
}

void Analyser::detectVariables()
{
    variables = "";
    for(int i=0;i<s.length(); ++i)
    {
        if(isVariable(s[i]) && !variables.contains(s[i]))
            variables+=s[i];
    }
}

void Analyser::run()
{
    if(s.isEmpty())
        throw QString("Error! Input is empty!");

    // what if user inputs this: hello world
    // or even this: 100101001010101
    for(int i=0;i<s.length()-1; ++i)
        if((isVariable(s[i]) && isVariable(s[i+1])) ||
           (isBit(s[i]) && isBit(s[i+1])))
            throw QString("Error! Bad input: " + s[i] + s[i+1]);

    // optimization. Now full equal expressions calculates REALLY fast.
    this->detectEqualExpressions();
    this->detectVariables();

    // show progress bar
    pb->setValue(0);
    pb->show();

    // start timer
    QTime timer = QTime::currentTime();

    const int vlen = variables.length();
    const long long imax = pow(2., vlen);

    // main calculation loop
    for(long long i = 0; i<imax; ++i)
    {
        QString expression = s;

        if(vlen > 0) // optimization
        {
            QString bits = dec2bin(i, vlen);
            for(int j=0; j<vlen; ++j)
            {
                expression.replace(variables[j], bits[j]);
            }
        }

        QChar answer = eval(expression);

        if(answer == ZERO)
            throw QString("Answer is 0. \nTime elapsed: " + QString::number(timer.elapsed()) + " ms");
        else
        {
            int prev = 0;
            int pbvalue = (double)i / (imax/100);
            if(pbvalue > prev) // small optimization.
                pb->setValue(pbvalue);
            prev = pbvalue;
        }
    }
    throw QString("Answer is 1. \nTime elapsed: " + QString::number(timer.elapsed()) + " ms");
}

void Analyser::setPb(QProgressBar *value)
{
    this->pb = value;
}

inline bool Analyser::isVariable(const QChar &c) const
{
    return c.isLetter();
}

inline bool Analyser::isOperator(const QChar &c) const
{
    if( c == NOT ||
        c == AND ||
        c == OR  ||
        c == XOR ||
        c == IMP ||
        c == EQ)
        return true;
    else
        return false;
}

inline bool Analyser::isBit(const QChar &c) const
{
    if(c==ONE || c==ZERO)
        return true;
    else
        return false;
}

inline bool Analyser::isGoodBinary(const QString &s) const
{
    if(s.length() == 3)
        if(isBit(s[0]) && isBit(s[2]))
            return true;
    return false;
}

inline bool Analyser::isGoodUnary(const QString &s) const
{
    if(s.length() == 2)
        if(isBit(s[1]))
            return true;
    return false;
}

inline QString Analyser::dec2bin(long long i, int num)
{
    QString b = QString::number(i,2);
    if(b.length() == num)
        return b;
    else{
        QString result(num,ZERO);
        result.append(b);
        return result;
    }

}

inline QChar Analyser::eval(QString q)
{
    /* brackets
     * not
     * and
     * or
     * xor
     * imp
     * eq
     */


    int o;
    while(q.length() > 1)
    {
        // brackets
        int l = q.lastIndexOf(LB);
        while(l != -1)
        {
            int r = q.indexOf(RB,l);
            if( r == -1)
                throw QString("Syntax error: extra '(' at " + QString::number(l));

            // what if user is dumb or tester? input: ()()()()() or ((((()))))
            if(r-l == 1)
            {
                q.replace(l,2,"");
                l = q.lastIndexOf(LB);
                continue;
            }

            if(l>0 && !isOperator(q[l-1]))
                throw QString("Syntax error: missed operator at " + QString::number(l-1));
            if(r+1<q.length() && !isOperator(q[r+1]))
                throw QString("Syntax error: missed operator at " + QString::number(r+1));

            // if seems good
            QString sub = q.mid(l+1,r-1-l);
            q.replace(l, r-l+1, eval(sub));
            l = q.lastIndexOf(LB);
        }
        int r = q.indexOf(RB);
        if(r != -1)
            throw QString("Syntax error: extra ')' at " + QString::number(r));

        // NOT
        o = q.indexOf(NOT);
        while( o != -1 )
        {
            // sequence of not
            if(q[o+1] == NOT)
            {
                q.replace(o,2,"");
                o = q.indexOf(NOT,o);
                continue;
            }

            QString sub = q.mid(o,2);
            if(!isGoodUnary(sub))
                throw QString("Syntax error. Bad NOT operator: " + q[o] + q[o+1]);
            if(sub[1] == ZERO)
                q.replace(o,2,ONE);
            else if(sub[1] == ONE)
                q.replace(o,2,ZERO);
            else
                throw QString("Unexpected symbol " + q[o+1] + " at " + QString::number(o+1));
            o = q.indexOf(NOT,o);
        }

        // AND
        o = q.indexOf(AND);
        while( o != -1 )
        {
            QString sub = q.mid(o-1, 3);
            if(!isGoodBinary(sub))
                throw QString("Syntax error. Bad AND operator: " + q[o-1] + q[o] + q[o+1]);
            if(sub[0] == ONE && sub[2] == ONE)
                q.replace(o-1,3,ONE);
            else
                q.replace(o-1,3,ZERO);
            o = q.indexOf(AND, o);
        }

        // OR
        o = q.indexOf(OR);
        while( o != -1 )
        {
            QString sub = q.mid(o-1, 3);
            if(!isGoodBinary(sub))
                throw QString("Syntax error. Bad OR operator: " + q[o-1] + q[o] + q[o+1]);
            if(sub[0] == ZERO && sub[2] == ZERO)
                q.replace(o-1,3,ZERO);
            else
                q.replace(o-1,3,ONE);
            o = q.indexOf(OR, o);
        }

        // XOR
        o = q.indexOf(XOR);
        while( o != -1 )
        {
            QString sub = q.mid(o-1, 3);
            if(!isGoodBinary(sub))
                throw QString("Syntax error. Bad XOR operator: " + q[o-1] + q[o] + q[o+1]);
            if(sub[0] == sub[2])
                q.replace(o-1,3,ZERO);
            else
                q.replace(o-1,3,ONE);
            o = q.indexOf(XOR, o);
        }

        // IMP
        o = q.indexOf(IMP);
        while( o != -1 )
        {
            QString sub = q.mid(o-1, 3);
            if(!isGoodBinary(sub))
                throw QString("Syntax error. Bad IMP operator: " + q[o-1] + q[o] + q[o+1]);
            if(sub[0] == ONE && sub[2] == ZERO)
                q.replace(o-1,3,ZERO);
            else
                q.replace(o-1,3,ONE);
            o = q.indexOf(IMP, o);
        }

        // EQ
        o = q.indexOf(EQ);
        while( o != -1 )
        {
            QString sub = q.mid(o-1, 3);
            if(!isGoodBinary(sub))
                throw QString("Syntax error. Bad EQ operator: " + q[o-1] + q[o] + q[o+1]);
            if(sub[0] == sub[2])
                q.replace(o-1,3,ONE);
            else
                q.replace(o-1,3,ZERO);
            o = q.indexOf(EQ, o);
        }
    }

    if(q.isEmpty())
        throw QString("Error! Input is empty!");
    else if(!isBit(q[0]))
        throw QString("Error! Bad input.");

    return q[0];
}

// what if user inputs two equal expressionsthis:
// A and B ~ A and B
void Analyser::detectEqualExpressions()
{
    int p = this->s.indexOf(EQ);
    if( p != -1) // if it is found
    {
        QStringList l = s.split(EQ);  // split by EQUAL
        int size = l.size();          // A and B ~ A and B ~ A and B -> size = 3
        int d = l.removeDuplicates(); // number of duplicates
        if(d == size - 1)
            s = ONE;
        else if(d < size - 1 && d > 0)
            s = l.join(EQ);
    }
}
