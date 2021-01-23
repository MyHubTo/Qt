void Widget::on_pushButton_clicked()
{
    bool ok;
    //其中QLineEdit::Normal是编码格式；可以选择具有小黑点的形式的password;
    //enum EchoMode { Normal, NoEcho, Password, PasswordEchoOnEdit };
    QString string=QInputDialog::getText(this,tr("输入字符串对话框"),tr("请输入用户名:"),QLineEdit::Normal,tr("admin"),&ok);
    if(ok) qDebug()<<"string"<<string;
    
    //其中1表示的是step即每次增量的大小；
    int value=QInputDialog::getInt(this,tr("输入整数对话框"),tr("请输入-1000到1000之间的数值"),0,-1000,1000,1,&ok);
    if(ok)
        qDebug()<<"value:"<<value;

    //其中2表示保留小数的位数的多少；
    double valued=QInputDialog::getDouble(this,tr("请输入doublel类型数值"),tr("请输入-1000到1000之间的数值"),0.0,-1000,1000,2,&ok);
    if(ok)
        qDebug()<<"valued:"<<valued;

    QStringList items;
    items<<tr("条目1")<<tr("条目2");

    //其中0表示当前索引值
    QString item=QInputDialog::getItem(this,tr("输入条目对话框"),tr("请选择或者输入一个条目"),items,0,true,&ok);
    if(ok)
        qDebug()<<"item:"<<item;

}
