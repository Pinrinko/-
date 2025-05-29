#pragma once
#pragma once
#include <QVector>
#include <QString>

struct SegmentTableEntry {
    int base;   // ҳ����ʼ�±꣨�����ò���������չ��
    int limit;  // ҳ����ÿ�����ҳ����
};

class SegmentPageManager {
public:
    static constexpr int MAX_SEGMENTS = 3;
    static constexpr int MAX_PAGES = 5;
    static constexpr int PAGE_SIZE = 1024;

    SegmentPageManager();

    // ���öα��ҳ������չΪ��̬��ʼ����
    void init();

    // ��ַת��������ת������ַ���
    QString translateAddress(int segment, int page_index, int offset) const;

    // ��ȡ�α��ҳ�����ڽ�����ʾ��
    QVector<SegmentTableEntry> getSegmentTable() const;
    QVector<QVector<int>> getPageTable() const;

private:
    QVector<SegmentTableEntry> segment_table;
    QVector<QVector<int>> page_table;
};