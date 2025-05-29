#pragma once
#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H

#include <QString>
#include <QList>
#include <QPair>

struct PCB {
    QString name;
    int ntime;
    int rtime;
    char state;
    PCB* link;
};

class RoundRobin {
public:
    RoundRobin();
    void setProcessList(const QList<QPair<QString, int>>& plist); // 新增
    QString runSimulation();

private:
    PCB* ready;
    int TIME_SLICE;
    void enqueue(PCB* p);
    QString display();
    void clear();
};

#endif // ROUNDROBIN_H
