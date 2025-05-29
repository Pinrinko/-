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
    int ntime;      // ������ʱ��
    int rtime;      // ������ʱ��
    int timeslice;  // �����Լ���ʱ��Ƭ
    char state;     // ״̬
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