#pragma once
#pragma once
#ifndef VARIABLEROUNDROBIN_H
#define VARIABLEROUNDROBIN_H

#include <QString>
#include <QList>
#include <QPair>
#include <tuple>

struct VariableRRPCB {
    QString name;
    int ntime;      // 总运行时间
    int rtime;      // 已运行时间
    int timeslice;  // 进程自己的时间片
    char state;     // 状态
    VariableRRPCB* link;
};

class VariableRoundRobin {
public:
    VariableRoundRobin();
    void setProcessList(const QList<std::tuple<QString, int, int>>& plist); // name, ntime, timeslice
    QString runSimulation();

private:
    VariableRRPCB* ready;
    void enqueue(VariableRRPCB* p);
    QString display();
    void clear();
};

#endif // VARIABLEROUNDROBIN_H