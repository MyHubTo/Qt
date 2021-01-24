## Label自动换行：

QLabel属性栏中的wordWrap属性可以实现文本的自动换行。如果文本过长时，不想自动换行而是在后面自动省略，那么可以使用<font color=blue>**QFontMetrics类，该类用来计算给定字体的字符或字符串的大小，其中包含了多个实用函数**</font>。要想实用QFontMetrics,则可以通过创建对象的方式，或通过QWidget::fontMetrics()来返回当前部件字体的QFontMetrics对象。

```cpp
    QString string=tr("需要省略的文本");
    QString str=ui->label->fontMetrics().elidedText(string,Qt::TextElideMode::ElideRight,50);
    ui->label->setText(str);
```
QFontMetrics类中的elidedText(）函数用来进行文本的省略，第一个参数用来指定要省略的文本，第二个参数是省略模式，就是“...”省略号的位置，主要有以下：
```cpp
     enum TextElideMode {
        ElideLeft,
        ElideRight,
        ElideMiddle,
        ElideNone
    };
```
## 播放gif图片
1.添加头文件#include<QMovie>;
```cpp
    QMovie *movie=new QMovie(":/images/images/gif.gif");
    ui->label->setMovie(movie);
    movie->start();
```
