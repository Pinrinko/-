#include "SegmentPageManager.h"

SegmentPageManager::SegmentPageManager() {
    init();
}

void SegmentPageManager::init() {
    // 初始化段表
    segment_table = {
        {0, 3},  // 段0 有3页
        {0, 2},  // 段1 有2页
        {0, 4}   // 段2 有4页
    };
    // 初始化页表
    page_table = {
        {5, 6, 7},         // 段0 页表
        {12, 13},          // 段1 页表
        {20, 21, 22, 23}   // 段2 页表
    };
}

QVector<SegmentTableEntry> SegmentPageManager::getSegmentTable() const {
    return segment_table;
}

QVector<QVector<int>> SegmentPageManager::getPageTable() const {
    return page_table;
}

QString SegmentPageManager::translateAddress(int segment, int page_index, int offset) const {
    QString res;
    res += QString("输入地址 -> 段: %1，页: %2，偏移: %3\n").arg(segment).arg(page_index).arg(offset);

    if (segment < 0 || segment >= segment_table.size()) {
        res += QString("错误：段号 %1 超出范围。\n").arg(segment);
        return res;
    }
    if (page_index < 0 || page_index >= segment_table[segment].limit) {
        res += QString("错误：页号 %1 超出段 %2 的页数限制（limit = %3）。\n")
            .arg(page_index).arg(segment).arg(segment_table[segment].limit);
        return res;
    }
    if (offset < 0 || offset >= PAGE_SIZE) {
        res += QString("错误：偏移量 %1 超出页大小限制（最大 %2）。\n").arg(offset).arg(PAGE_SIZE - 1);
        return res;
    }
    int page_frame = page_table[segment][page_index];
    int physical_address = page_frame * PAGE_SIZE + offset;

    res += QString("段表信息: base = %1, limit = %2\n")
        .arg(segment_table[segment].base).arg(segment_table[segment].limit);
    res += QString("页表[%1][%2] = %3（页框号）\n").arg(segment).arg(page_index).arg(page_frame);
    res += QString("转换后物理地址: %1\n").arg(physical_address);
    return res;
}