#include "yhj.h"

YHJ::YHJ()
    : m(0), n(0)
{
}

void YHJ::setProcessResource(int processCount, int resourceCount,
    const QVector<QVector<int>>& max,
    const QVector<QVector<int>>& allocation,
    const QVector<int>& available)
{
    m = processCount;
    n = resourceCount;
    MAX = max;
    ALLOCATION = allocation;
    AVAILABLE = available;
    NEED = QVector<QVector<int>>(m, QVector<int>(n));
    FINISH = QVector<bool>(m, false); // 新增：初始化FINISH
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            NEED[i][j] = MAX[i][j] - ALLOCATION[i][j]; // 自动计算
}

QString YHJ::showStatus() const
{
    QString res;
    res += "当前系统状态:\n";
    res += "进程号\t已分配资源\t剩余需求\n";
    for (int i = 0; i < m; ++i) {
        res += QString::number(i) + "\t";
        for (int j = 0; j < n; ++j)
            res += QString::number(ALLOCATION[i][j]) + " ";
        res += "\t\t";
        for (int j = 0; j < n; ++j)
            res += QString::number(NEED[i][j]) + " ";
        res += "\n";
    }
    res += "系统剩余资源:\n";
    for (int i = 0; i < n; ++i)
        res += QString::number(AVAILABLE[i]) + " ";
    res += "\n";
    return res;
}

QString YHJ::getSafeSequence() const
{
    QVector<int> safeSeq;
    QString info;
    if (safeCheck(safeSeq, info)) {
        info += "安全序列: ";
        for (int i = 0; i < safeSeq.size(); ++i)
            info += QString::number(safeSeq[i]) + (i < safeSeq.size() - 1 ? " -> " : "\n");
    }
    return info;
}

QString YHJ::requestResource(int processId, const QVector<int>& request)
{
    QString res;
    if (processId < 0 || processId >= m)
        return "进程号不合法。\n";

    for (int i = 0; i < n; ++i) {
        if (request[i] > NEED[processId][i])
            return "错误：请求超过进程需求！不予分配！\n";
        if (request[i] > AVAILABLE[i])
            return "错误：请求超过系统资源！不予分配！\n";
    }

    // 试分配
    for (int i = 0; i < n; ++i) {
        AVAILABLE[i] -= request[i];
        ALLOCATION[processId][i] += request[i];
        NEED[processId][i] -= request[i];
    }

    QVector<int> safeSeq;
    QString info;
    if (safeCheck(safeSeq, info)) {
        res += "同意分配请求！\n";
        res += info;
    }
    else {
        // 回滚
        for (int i = 0; i < n; ++i) {
            AVAILABLE[i] += request[i];
            ALLOCATION[processId][i] -= request[i];
            NEED[processId][i] += request[i];
        }
        res += "请求被拒绝，正在回滚...\n";
        res += info;
    }
    return res;
}

bool YHJ::safeCheck(QVector<int>& safeSeq, QString& info) const
{
    QVector<int> work = AVAILABLE;
    QVector<bool> finish = FINISH;
    QVector<QVector<int>> need = NEED;
    QVector<QVector<int>> allocation = ALLOCATION;
    int count = 0;
    int proc = m;
    safeSeq.clear();

    while (count < proc) {
        bool found = false;
        for (int i = 0; i < proc; ++i) {
            if (finish[i]) continue;
            bool canAllocate = true;
            for (int j = 0; j < n; ++j) {
                if (need[i][j] > work[j]) {
                    canAllocate = false;
                    break;
                }
            }
            if (canAllocate) {
                for (int j = 0; j < n; ++j)
                    work[j] += allocation[i][j];
                finish[i] = true;
                safeSeq.push_back(i);
                ++count;
                found = true;
                break;
            }
        }
        if (!found) {
            info += "系统不安全！\n";
            return false;
        }
    }
    info += "系统安全。\n";
    return true;
}

