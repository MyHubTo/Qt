#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    inti();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inti()
{
    MainWindow::setMinimumSize(800,600);
    //setCentralWidget(ui->tableWidget);
    //状态栏初始化
    labCellIndex =new QLabel("当前单元坐标：",this);
    labCellIndex->setMinimumWidth(250);
    labCellType=new QLabel("当前单元类型：",this);
    labCellType->setMinimumWidth(200);
    labStoID=new QLabel("学生 ID：",this);
    labStoID->setMinimumWidth(200);

    //添加到状态栏
    ui->statusbar->addWidget(labCellIndex);
    ui->statusbar->addWidget(labCellType);
    ui->statusbar->addWidget(labStoID);

    //设置SpinBox最大值
    ui->spinBox_2->setMaximum(10000);
    ui->spinBox_2->setValue(5);

}

void MainWindow::creatItemsARow(int rowNo, QString Name, QString Sex, QDate birth, QString Nation, bool isPM, int score)
{

    //创建一行单元格
    QTableWidgetItem *item;
    QString str;
    //学号基数
    double StuID=2190221126;
    /*
     * @text:单元格显示的文字；
     * @Type:节点类型；
     * explicit QTableWidgetItem(const QString &text, int type = Type);
    */

    //姓名
    item=new QTableWidgetItem(Name,MainWindow::ctName);
    //对齐方式
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    //学号
    StuID+=rowNo;
    //设置Data

    item->setData(Qt::UserRole,QVariant(StuID));
    ui->tableWidget->setItem(rowNo,MainWindow::colName,item);

    //性别
    QIcon icon;
    if(Sex=="男")
    {
        icon.addFile("/home/mrzhao/Desktop/icons/person.svg");
    }
    else
    {
        icon.addFile("/home/mrzhao/Desktop/icons/people.svg");
    }

    item=new QTableWidgetItem(Sex,MainWindow::ctSex);
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colsex,item);

    //出生日期

    //日期转成字符串
    str=birth.toString("yyyy-MM-dd");
    item=new QTableWidgetItem(str,MainWindow::ctBirth);

    item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,MainWindow::colBirth,item);

    //民族
    item =new QTableWidgetItem(Nation,MainWindow::ctNation);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colNation,item);

    //是否是党员
    item=new QTableWidgetItem("党员",MainWindow::ctPartyM);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    if (isPM)
    {
        item->setCheckState(Qt::Checked);
    }
    else {
        item->setCheckState(Qt::Unchecked);
    }
    ui->tableWidget->setItem(rowNo,colPartyM,item);
    //分数
    item->setBackgroundColor(Qt::yellow);
    ui->tableWidget->setItem(rowNo,MainWindow::ctPartyM,item);

    str.setNum(score);
    item =new QTableWidgetItem(str,MainWindow::ctScore);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colScore,item);

}

void MainWindow::on_pushBut_setTabHead_clicked()
{
    QTableWidgetItem *headItem;
    QStringList headText;
    headText<<"姓名"<<"性别"<<"出生日期"<<"民 族"<<"分 数"<<"是否是党员";
    //设置列数
    ui->tableWidget->setColumnCount(headText.count());
    //ui->tableWidget->setColumnCount(headText.length());
    //ui->tableWidget->setColumnCount(headText.size());

    for(int i=0;i<ui->tableWidget->columnCount();i++)
    {
        //设置表头内容：headText.at(0)=“姓名”；
        headItem =new QTableWidgetItem(headText.at(i));

        //设置字体
        QFont font=headItem->font();

        font.setBold(false);
        font.setPointSize(12);

        headItem->setTextColor(Qt::red);
        headItem->setFont(font);

        //设置为水平头Item
        ui->tableWidget->setHorizontalHeaderItem(i,headItem);


    }

}

void MainWindow::on_pushBu_setRowNum_clicked()
{
    on_pushBut_setTabHead_clicked();
    int count=ui->spinBox_2->value();
    ui->tableWidget->setRowCount(count);
}

void MainWindow::on_pushBut_insetRow_clicked()
{
    int row=ui->tableWidget->currentRow();

    //在行号为row 的前面插入一行；
    ui->tableWidget->insertRow(row);
    //插入一行不会自动为单元格创建Item；
    creatItemsARow(row,"SLAM","男",QDate::fromString("1999-01-27","yyyy-MM-dd"),"汉族",false,85);
}


void MainWindow::on_pushBut_InitTab_clicked()
{
    //设置行数
    on_pushBu_setRowNum_clicked();
    //设置间隔行颜色
    on_checkBox_RowColor_clicked(true);
    QString strName,strSex;
    bool isParty=false;
    QDate Birth;
    Birth.setDate(1980,4,7);
    ui->tableWidget->clearContents();
    int Rows=ui->tableWidget->rowCount();
    for(int i=0;i<Rows;i++)
    {
        strName=QString::asprintf("学生%d",i);
        if (i%2==0)
        {
            strSex="男";

        }
        else {
            strSex="女";

        }
        creatItemsARow(i,strName,strSex,Birth,"汉族",isParty,70);
        Birth=Birth.addDays(20);
        isParty=!isParty;
    }
}

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    QTableWidgetItem *item=ui->tableWidget->item(currentRow,currentColumn);
    if(item==NULL)
    {
        return  ;
    }
    labCellIndex->setText(QString::asprintf("当前单元的坐标：%d行,%d列",currentRow+1,currentColumn+1));

    //获取单元格类型
    /* ctName=1000,
        ctSex,
        ctBirth,
        ctNation,
        ctPartyM,
        ctScore
    */
    int cellType=item->type();
    labCellType->setText(QString::asprintf("当前单元格的类型：%d",cellType));

    //第一列的Item
    item=ui->tableWidget->item(currentRow,MainWindow::colName);

    //读取用户自定数据
    int ID=item->data(Qt::UserRole).toInt();
    labStoID->setText(QString::asprintf("学生 ID：%d",ID));

}

void MainWindow::on_pushBut_InsertCol_clicked()
{

    int rowCount=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    creatItemsARow(rowCount,"SLAM","男",QDate::fromString("1999-01-27","yyyy-MM-dd"),"汉族",false,85);
}

void MainWindow::on_pushBut_DelCurrRow_clicked()
{
    int currow=ui->tableWidget->currentRow();
    //删除指定行
    ui->tableWidget->removeRow(currow);
}

void MainWindow::on_pushBut_AutoHei_clicked()
{
    //自动调整所有行的高度，以适应其内容；
    ui->tableWidget->resizeRowsToContents();
    /*
    *  //自动调整指定行的高度，以适应其内容
    *    int currow=ui->tableWidget->currentRow();
    *    ui->tableWidget->resizeRowToContents(currow);
    */

}
/*
    *QTbableWidget的EditTriggers属性表示是否可编辑，以及进入编辑状态的方式
    */

void MainWindow::on_checkBox_EnEdit_clicked(bool checked)
{
    if(checked)
    {
        //双击获取焦点后单击，进入编辑状态
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    }
    else
    {
        //不能编辑
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void MainWindow::on_pushBut_AutoWeid_clicked()
{
    //自动调整列宽以适应其内容；
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_checkBox_ShowRowHead_clicked(bool checked)
{
    //是否显示行表头
    ui->tableWidget->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_checkBox_ShowColHead_clicked(bool checked)
{
    //是否显示列表头
    ui->tableWidget->verticalHeader()->setVisible(checked);
}

/*
 *setAlternatingRowColors()设置表格的行是否交替底色显示，若为交替底色显示，则间隔的一行会用灰色作为底色
*/
void MainWindow::on_checkBox_RowColor_clicked(bool checked)
{

    ui->tableWidget->setAlternatingRowColors(checked);
}

void MainWindow::on_radioBut_RowChoice_clicked()
{
    //单元格选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::on_radioButton_ColChoice_clicked()
{
    //单元格选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void MainWindow::on_pushBut_ReadToText_clicked()
{
    QString str;
    QTableWidgetItem *cellitem;
    ui->plainTextEdit->clear();
    int rowcount=ui->tableWidget->rowCount();
    int colcount=ui->tableWidget->columnCount();
    for(int i=0;i<rowcount;i++)
    {
        str=QString::asprintf("第%d行：",i+1);

        //不循环到最后一列，最后一列需要下面的判断
        for(int j=0;j<colcount-1;j++)
        {
            cellitem=ui->tableWidget->item(i,j);
            str=str+cellitem->text()+"  ";
        }

        //最后一列
        cellitem=ui->tableWidget->item(i,colPartyM);
        if(cellitem->checkState()==Qt::Checked)
        {
            str=str+"党员";
        }
        else {
            str=str+"群众";
        }

        ui->plainTextEdit->appendPlainText(str);
    }

}
