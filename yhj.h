#ifndef YHJ_H
#define YHJ_H

#include <QString>
#include <QVector>

class YHJ
{
public:
    YHJ();
    int getProcessCount() const { return m; }
    int getResourceCount() const { return n; }
    void setProcessResource(int processCount, int resourceCount,
        const QVector<QVector<int>>& max,
        const QVector<QVector<int>>& allocation,
        const QVector<int>& available);
    
    const QVector<QVector<int>>& getAllocation() const { return ALLOCATION; }
    const QVector<int>& getAvailable() const { return AVAILABLE; }

    QString showStatus() const;
    QString requestResource(int processId, const QVector<int>& request);
    QString getSafeSequence() const;

private:
    int m, n;
    QVector<int> AVAILABLE;
    QVector<QVector<int>> MAX;
    QVector<QVector<int>> ALLOCATION;
    QVector<QVector<int>> NEED;
    QVector<bool> FINISH;
    QVector<int> p; // °²È«ÐòÁÐ

    bool safeCheck(QVector<int>& safeSeq, QString& info) const;
};

#endif // YHJ_H#pragma once
