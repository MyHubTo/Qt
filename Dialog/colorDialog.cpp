void Widget::on_pushButton_2_clicked()
{
    //方式1；
    //其中Qt::white表示初始颜色,QColorDialog::ShowAlphaChannel表示显示alpha设置，可以实现透明；
    //enum ColorDialogOption ShowAlphaChannel = 0x00000001,NoButtons = 0x00000002,DontUseNativeDialog = 0x00000004
    
    //    QColor color=QColorDialog::getColor(Qt::white,this,tr("选择一个颜色"),QColorDialog::ShowAlphaChannel);
    //    qDebug()<<"color:"<<color;//color: QColor(ARGB 1, 1, 0, 0);后面四个数字分别表示的是：alpha,red,green,blue分量取值范围均是（0,1);
    //    qDebug()<<"color:"<<color.name();//颜色名color: "#ff0000"
    
    //方式2；
    QColorDialog dialog(Qt::red,this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    dialog.exec();//    以模态的方式打开对话框
    QColor color_=dialog.currentColor();
    qDebug()<<"color:"<<color_;
}
