## 1.输入验证
### 说明  #include/<QValidator/>
1.The class itself is abstract. Two subclasses, **QIntValidator and QDoubleValidator**, provide basic numeric-range checking, and **QRegExpValidator** provides general checking using a custom regular expression.
QValidator具有两个子类，分别为**QIntValidator and QDoubleValidator**，可以用于正数与浮点数的验证操作。
若需要对字符进行输入验证在需要利用QRegExpValidator用**正则表达**是进行构造；

2.使用验证器的作用在于，只有指定格式或范围的输入才是有效的，否则无效；

```cpp
  //新建验证器，指定范围0-100；
    QValidator *validator=new QIntValidator(0,100,this);
    ui->lineEdit->setValidator(validator);
```
**注意**：此处将QIntValidator类型赋值于QValidator； 

## 2.自动补全
### 说明： #include<QCompleter/>
  ```cpp
  //自动补齐
    QStringList wordlist;
    wordlist<<"Qt"<<"Qt Creator"<<tr("你好");
    QCompleter *completer=new QCompleter(wordlist,this);

    /*enum CaseSensitivity {
        CaseInsensitive, 不区分大小写
        CaseSensitive    区分大小写
    */
    completer->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);//不区分大小写
    ui->lineEdit_2->setCompleter(completer);
 
```
