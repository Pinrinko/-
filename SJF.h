#pragma once
#pragma once
#include <QString>
#include <QList>

struct SJFJob {
    QString name;
    int arrive_time;
    int run_time;
    int start_time;
    int finish_time;
    bool is_finished;
};

class SJF {
public:
    SJF();
    void clear();
    void addJob(const QString& name, int arrive_time, int run_time);
    QString schedule();

private:
    QList<SJFJob> jobs;
};