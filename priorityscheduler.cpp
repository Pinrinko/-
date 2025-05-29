#include "priorityscheduler.h"

PriorityScheduler::PriorityScheduler()
    : ready(nullptr)
{
}

void PriorityScheduler::clear() {
    while (ready) {
        PriorityPCB* p = ready;
        ready = p->link;
        delete p;
    }
}

void PriorityScheduler::sort(PriorityPCB* new_p) {
    PriorityPCB* first, * second;
    int insert = 0;

    if ((ready == nullptr) || (new_p->super > ready->super)) {
        new_p->link = ready;
        ready = new_p;
    }
    else {
        first = ready;
        second = first->link;
        while (second != nullptr) {
            if (new_p->super > second->super) {
                new_p->link = second;
                first->link = new_p;
                insert = 1;
                break;
            }
            first = second;
            second = second->link;
        }
        if (insert == 0) {
            first->link = new_p;
            new_p->link = nullptr;
        }
    }
}

void PriorityScheduler::setProcessList(const QList<QPair<QString, QPair<int, int>>>& plist) {
    clear();
    for (const auto& pair : plist) {
        PriorityPCB* p = new PriorityPCB;
        p->name = pair.first;
        p->super = pair.second.first;
        p->ntime = pair.second.second;
        p->rtime = 0;
        p->state = 'w';
        p->link = nullptr;
        sort(p);
    }
}

QString PriorityScheduler::display(PriorityPCB* running_p) {
    QString res;
    res += "\n**** 当前正在运行的进程是: " + running_p->name + " ****\n";
    res += "-------------------------------------------\n";
    res += "| qname    | state | super | ndtime  | runtime  |\n";
    res += QString("| %1 | %2 | %3 | %4 | %5 |\n")
        .arg(running_p->name, -8)
        .arg(QChar(running_p->state), -5)
        .arg(running_p->super, -5)
        .arg(running_p->ntime, -8)
        .arg(running_p->rtime, -8);

    res += "\n\n**** 当前就绪队列状态 ****\n";
    res += "-------------------------------------------\n";
    res += "| qname    | state | super | ndtime  | runtime  |\n";
    PriorityPCB* pr = ready;
    while (pr != nullptr) {
        res += QString("| %1 | %2 | %3 | %4 | %5 |\n")
            .arg(pr->name, -8)
            .arg(QChar(pr->state), -5)
            .arg(pr->super, -5)
            .arg(pr->ntime, -8)
            .arg(pr->rtime, -8);
        pr = pr->link;
    }
    res += "-------------------------------------------\n";
    return res;
}

QString PriorityScheduler::runSimulation() {
    QString output;
    int h = 0;

    while (ready != nullptr) {
        h++;
        output += QString("\n执行轮次: %1\n").arg(h);

        PriorityPCB* current = ready;
        ready = current->link;
        current->link = nullptr;
        current->state = 'R';

        output += display(current);

        // running
        current->rtime++;
        if (current->rtime == current->ntime) {
            output += QString("\n进程 [%1] 已完成.\n").arg(current->name);
            delete current;
        }
        else {
            current->state = 'w';
            sort(current);
        }
    }

    output += "\n所有进程已完成.\n";
    return output;
}