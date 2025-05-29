#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindowClass;  // 修改为 MainWindowClass
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void runRoundRobin();
    void on_pushButton_2_clicked(); // 新增
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_btnBackToMain_clicked();
    void page0button1();
    void page0button2();
    void page0button3();
    void page0button4();

    void page0button5();
    void initBankerTables(int processCount, int resourceCount);
    void readBankerTables(QVector<QVector<int>>& max, QVector<QVector<int>>& allocation, QVector<int>& available); // Add this declaration  
    void on_pushButtonYHJ_clicked();
    void on_pushButtonYHJRequest_clicked();
    void on_pushButtonFCFS_clicked();
	void on_pushButtonHRN_clicked();    
	void on_pushButtonSJF_clicked();
    void on_pushButtonFixedPartitionAlloc_clicked();
    void updatePartitionTable(); 
    void updateDynamicPartitionTable();
    void on_pushButtonDynamicPartition_clicked();
    void on_pushButtonPage_clicked();
    void updatePageTable();
    void on_pushButtonSegmentPage_clicked();
    void on_pushButtonfile_clicked();
    void updateTreeWidgetFile();
	void on_pushButtonfileCommand_clicked();
    void on_lineEditCmd_returnPressed();
private:
    Ui::MainWindowClass* ui;  // 修改为 MainWindowClass
  
};

#endif // MAINWINDOW_H