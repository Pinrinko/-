#pragma once
#pragma once
#include <QString>
#include <QList>

struct HRNJob {
    QString name;
    int arrive_time;
    int run_time;
    int start_time;
    int finish_time;
    bool is_finished;
};

class HRN {
public:
    HRN();
    void clear();
    void addJob(const QString& name, int arrive_time, int run_time);
    QString schedule();

private:
    QList<HRNJob> jobs;
};