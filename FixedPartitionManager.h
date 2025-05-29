#pragma once
#pragma once
#include <QString>
#include <QVector>

struct Partition {
    int id;
    int size;
    bool is_allocated;
    QString job_name;
};

class FixedPartitionManager {
public:
    FixedPartitionManager();
    void reset();
    bool allocateJob(const QString& jobName, int jobSize, QString& msg);
    bool releaseJob(const QString& jobName, QString& msg);
    QVector<Partition> getPartitions() const;

private:
    QVector<Partition> partitions;
};