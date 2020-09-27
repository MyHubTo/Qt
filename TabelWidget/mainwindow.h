
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDate>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //设置表头
    void on_pushBut_setTabHead_clicked();
    //设置行数
    void on_pushBu_setRowNum_clicked();
    //插入行
    void on_pushBut_insetRow_clicked();
    //初始化表格
    void on_pushBut_InitTab_clicked();
    //当前单元格内容改变
    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);


    //添加行（此处写错了，不应该是col）
    void on_pushBut_InsertCol_clicked();
    //删除当前行
    void on_pushBut_DelCurrRow_clicked();
    //自动行高
    void on_pushBut_AutoHei_clicked();
    //是否可以编辑
    void on_checkBox_EnEdit_clicked(bool checked);
    //自动宽度
    void on_pushBut_AutoWeid_clicked();
    //显示行头
    void on_checkBox_ShowRowHead_clicked(bool checked);
    //显示列头
    void on_checkBox_ShowColHead_clicked(bool checked);
    //间隔行颜色
    void on_checkBox_RowColor_clicked(bool checked);
    //行选择
    void on_radioBut_RowChoice_clicked();
    //单元格选则（同样笔误）
    void on_radioButton_ColChoice_clicked();
    //读取单元格所有内容
    void on_pushBut_ReadToText_clicked();

private:
    Ui::MainWindow *ui;
    //自定义单元格Type的类型，在创建单元格的Item时用
    enum CellType
    {
        ctName=1000,
        ctSex,
        ctBirth,
        ctNation,
        ctPartyM,
        ctScore
    };
    //名字段在表格中的列号
    enum FileColum
    {
        colName=0,
        colsex,
        colBirth,
        colNation,
        colScore,
        colPartyM
    };
    //状态栏上用于显示单元格的行号、列号
    QLabel *labCellIndex;
    //状态栏上用于显示单元格的type
    QLabel *labCellType;
    //状态栏上用于显示学号
    QLabel *labStoID;
    //为某一行创建item
    void inti();
    void creatItemsARow(int rowNo,QString Name,QString Sex,QDate birth,
                        QString Nation,bool isPM,int score);
};

#endif // MAINWINDOW_H
