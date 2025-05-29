#pragma once
#pragma once
#include <QVector>
#include <QString>

struct SegmentTableEntry {
    int base;   // 页表起始下标（本例用不到，可扩展）
    int limit;  // 页数（每段最多页数）
};

class SegmentPageManager {
public:
    static constexpr int MAX_SEGMENTS = 3;
    static constexpr int MAX_PAGES = 5;
    static constexpr int PAGE_SIZE = 1024;

    SegmentPageManager();

    // 设置段表和页表（可扩展为动态初始化）
    void init();

    // 地址转换，返回转换结果字符串
    QString translateAddress(int segment, int page_index, int offset) const;

    // 获取段表和页表（用于界面显示）
    QVector<SegmentTableEntry> getSegmentTable() const;
    QVector<QVector<int>> getPageTable() const;

private:
    QVector<SegmentTableEntry> segment_table;
    QVector<QVector<int>> page_table;
};