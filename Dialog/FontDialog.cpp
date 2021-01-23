void Widget::on_pushButton_3_clicked()
{
    //用于标记有没有单击ok按钮；
    bool ok=false;
    QFont font=QFontDialog::getFont(&ok,this);

    //如果点击了ok按钮，此时ok为true,表示接受所选择的字体,否则退出
    if(ok)
        qDebug()<<"font:"<<font;
    else
        qDebug()<<tr("没有选择字体！");
}
