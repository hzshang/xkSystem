学生选课管理系统  （待补）
===============
----

-----
## **ID格式**  
#### userID
- 9位数字 ( char(9) )
- 第一位：0：老师 1,2,3,4：大一，大二，大三，大四
- 中间四位院系ID  
- 后四位流水号

#### 课程ID
- 6位
- 前四位老师ID
- 最后两位流水

## **登录界面** 

- 输入账号（学号、教职工号），密码（md5加密）  
- 登录，注册，忘记密码

## **忘记密码**
- 输入账号，姓名
- 如果正确，填写新密码



## **注册界面**  

#### 学生注册  
- 学生姓名
- 密码
- 再次填写密码
- 所在学院
- 年级    
  
#### 教师注册
- 姓名
- 密码
- 再次输入密码
- 所在学院
- （年级一栏强制为0）


## **学生界面**  
- (模仿学校选课系统)右上方上方课程表
- 右下方为学校课程名录(每页显示10条，可翻页)
- 选课允许模糊匹配  
- 左侧为学生基本信息（年级，姓名，学号，所在学院）


## **老师界面**
- 左侧为老师基本信息
- 右上方为开设的课程
- 右下方为开设课程选课的学生（学号，姓名）
- 有添加课程选项，取消课程选项

### 添加课程界面  
- 课程名
- 限制人数
- 课程教室 (输入完成后检索出可用的开课时间)
- 开课时间 (可多选)
- 课程学分
- (课程ID自动生成)
- 选课成功后出现提示窗口

### 取消课程界面
- 出现询问窗口
- 执行数据库操作

## **数据库设计**
```sql
create table department
(
	did char(4), 
	dname varchar(30),
	primary key(did)
);
create table user
(
	id char(9),
	name varchar(30),
	grade int,            
	did char(4),
	pwd varchar(30),
	primary key(id),
	foreign key(did) references department(did)
);
create table room
(
	rid char(4),
	monday char(10),
	tuesday char(10),
	wednesday char(10),
	thursday char(10),
	friday char(10),
	saturday char(10),
	sunday char(10),
	primary key(rid)
);
create table course
(
	cid char(6),
	cname varchar(30),
	current int,
	max int,
	tid char(9),
	ctime char(3),#"122"代表周一第二节开始两个课时
	rid char(4),
	credit int,
	primary key(cid),
	foreign key(tid) references user(id),
	foreign key(rid) references room(rid)
);
create table sc
(
	sid char(9),
	cid char(6),
	primary key(sid,cid),
	foreign key(sid) references user(id),
	foreign key(cid) references course(cid)
);

```
## **选课限制**  

第一轮选课
- 学生只能选本院系，本年级课程  

第二轮选课  
- 学生可选其他院系，其他年级课程
- 无限制条件

## **服务请求规则**  
#### 登录(Done)
请求
```json
{
    "pid":1,
    "user":"200010001",
    "pwd":"asdffgghhj"
}
```

返回  
```json
{
    "state":true
}
```

#### 注册(Done)
请求
```json
{
    "pid":2,
    "name":"Tom",
    "did":"0001",
    "grade":2,
    "pwd":"asdfghj"
}
```

返回
```json
{
    "user":"200010001"
}
```

#### 忘记密码(Done)
请求
```json
{
    "pid":3,
    "user":"200010001",
    "name":"Tom",
    "pwd":"newpwd"
}
```
返回
```json
{
    "state":true
}
```

#### 请求学院(Done)
请求
```json
{
    "pid":4
}
```
返回
```json
[
    "0001.计算机",
    "0002.数学"
]
```

#### 请求学生课程表信息(Done)
请求  
```json
{
    "pid":5,
    "user":100010001
}
```
返回
```json
[
   {
        "cid":"000101"
        "cname":"程序设计",
        "rid":"2108",
        "ctime":"112",
        "tname":"李华"
   },
   {
        "cid":"000101"
        "cname":"数据库",
        "rid":"2108",
        "ctime":"112",
        "tname":"李华"    
   }
]
```
查询操作
```sql
select cname,rid,ctime,user.name
from course,user,sc
where sc.sid="100010001" and sc.cid=course.cid and course.tid=user.id
```
####  查看可选课程(Done)
请求  
未选课程过多，每页显示10个课程，index表示第一个课程的序号，翻页时序号加10
```json
{
    "pid":6,
    "index":0,
    "user":"100010001"
}
```
返回  
total表示所有待选可课程（方便计算页数）
```json
{
    "total":98,
    "courses":
    [
        {
            "cid":"000101",
            "cname":"程序设计",
            "current":15,
            "max":45,
            "rid":"2108",
            "ctime":"112",
            "tname":"李华"
            "credit":2
        },
        {
            "cid":"000102",
            "cname":"数据库",
            "current":15,
            "max":45,
            "rid":"2108",
            "ctime":"112",
            "tname":"李华",
            "credit":2     
        }
    ]
}
```
查询
```sql
select cid,cname,current,max,ctime,name
from user,course
where id=tid and cid not in (
    select cid from sc where sid="100010001"
);
```

#### 学生选课操作(Done)  
请求  
```json
{
    "pid":7,
    "user":"100010001",
    "cid":"000302"
}
```
返回  
```json
{
    "state":true
}
```

#### 学生退课操作(Done)  
```json
{
    "pid":8,
    "user":"100010001",
    "cid":"000302"
}
```
返回  
```json
{
    "state":true
}
```
操作
```sql
delete from sc where sid="100010001" and cid="000302"
```

#### 老师添加课程(Done)  
请求
```json
{
    "pid":9,
    "user":000010001,
    "cname":"程序设计",
    "rid":"2109",
    "ctime":"312",
    "max":45,
    "credit":2
}
```
返回
```json
{
    "state":true,
    "cid":"000101"
}
{
    "state":false
}
```

#### 老师删除课程(Done)  
请求  
```json
{
    "pid":10,
    "cid":"000101",
    "user":"000010001"
}
```
返回  
```json
{
    "state":true
}
{
    "state":false
}
```
#待补  




#### 老师查看自己开设的课程  
#### 老师查看选择自己课程的学生  
#### 请求所有教室占用情况
## 网络接口   

```c++
    //每次调用send后必须调用recv函数
    bool send(const QJsonObject &);//发送
    QJsonDocument recv();//接受
```


