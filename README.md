[wordCount项目博文传送门](https://www.cnblogs.com/hyzyh/p/9630253.html)

# wordCount
一个用C++实现的小项目。

# 功能
- 基本功能
	- 统计C语言源文件的字符数
	- 统计C语言源文件的词的数目
	- 统计C语言源文件的行数
- 拓展功能
	- 递归处理目录下符合条件的文件
	- 返回更复杂的数据（代码行 / 空行 / 注释行）

# 说明
首先编译`wc.cpp`源文件，生成执行文件。在Windows系统下会生成wc.exe，在Linux系统上会生成a.out。
下面以Linux系统为例进行说明。
1. 查询字符数
```
./a.out -c FILE
```
2. 查询词数
```
./a.out -w FILE
```
3. 查询行数
```
./a.out -l FILE
```
4. 查询更复杂的信息
```
./a.out -a FILE
```
5. 以上参数可以一起使用，比如查询所有信息
```
./a.out -cwla FILE
```
6. 此外，还可以递归查询目录
```
./a.out -cwlas DIR
```

# TODO
实现图形界面
