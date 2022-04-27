## 这是一个基于fuse框架的naive文件系统
+ 在second下
    + 当我们echo "xixi" > /srb/zyl 时
      我们会在srb里面发现一个zyl文件，在zyl里面发现一个srb文件,文件内容是写进去的字符串
+ 不在second下
    + 当我们echo "xixi" > /b1/zyl 时
      我们会在b1里面发现一个zyl文件，在b2里面发现一个zyl文件,文件内容是写进去的字符串