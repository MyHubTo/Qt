void Widget::on_pushButton_clicked()
{
    bool ok;
    QString string=QInputDialog::getText(this,tr("输入字符串对话框"),tr("请输入用户名:"),QLineEdit::Normal,tr("admin"),&ok);
    if(ok) qDebug()<<"string"<<string;

    int value=QInputDialog::getInt(this,tr("输入整数对话框"),tr("请输入-1000到1000之间的数值"),0,-1000,1000,1,&ok);
    if(ok)
        qDebug()<<"value:"<<value;

    double valued=QInputDialog::getDouble(this,tr("请输入doublel类型数值"),tr("请输入-1000到1000之间的数值"),0.0,-1000,1000,2,&ok);
    if(ok)
        qDebug()<<"valued:"<<valued;

    QStringList items;
    items<<tr("条目1")<<tr("条目2");

    QString item=QInputDialog::getItem(this,tr("输入条目对话框"),tr("请选择或者输入一个条目"),items,0,true,&ok);
    if(ok)
        qDebug()<<"item:"<<item;

}
