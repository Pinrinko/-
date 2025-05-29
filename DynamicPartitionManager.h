#pragma once
#pragma once
#include <QString>
#include <QVector>

enum class AllocationAlgorithm {
    FirstFit = 1,
    BestFit,
    WorstFit
};

struct MemoryBlock {
    int start;
    int size;
    bool is_allocated;
    QString job_name;
};

class DynamicPartitionManager {
public:
    DynamicPartitionManager();
    void init(int total_size);
    bool allocateJob(const QString& jobName, int jobSize, AllocationAlgorithm algo, QString& msg);
    bool releaseJob(const QString& jobName, QString& msg);
    QVector<MemoryBlock> getBlocks() const;
    void reset(int total_size);

private:
    void mergeFreeBlocks();
    QVector<MemoryBlock> blocks;
};