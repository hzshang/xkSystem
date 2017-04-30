学生选课管理系统  
=============

##版本特点
- 采用服务端、客户端分离开发，保证数据库安全
- 使用TCP连接传送数据
- 用户密码采用md5算法加密  
- 支持用户重置密码
- 学生选课支持模糊查询
- 支持老师自定义添加课程
- 支持两轮选课，第一轮只能选本院系课程，第二轮可选所有课程
  
[设计文档](resources/design.md)


##运行要求  
- python 3.5 or newer   
- pip  
- mysql  

##测试环境  
Ubuntu

##安装
###step1
```bash
git clone https://github.com/hzshang/xkSystem.git
cd xkSystem/server
```  
###step2
创建数据库并导入sql目录下的struct.sql文件  
###step3
安装python模块cymysql
```bash
pip3 install cymysql
```
###step4
复制sample-config.json并重命名为config.json,修改参数  
```json
{
	"db_host":"yourHost",//数据库地址 
	"db_database":"database",//数据库名字
	"db_user":"user",//user
	"db_passwd":"passwd",//password
	"listen":"0.0.0.0",//监听地址
	"port":3307,//监听端口
	"round":1//选课轮数
}
```
###step5
修改MTU  
由于单次发送的数据包过大，应将MTU(一个数据包的最大字节数)设为4096  
```bash
#查看MTU  
cat /sys/class/net/eth0/mtu
#修改MTU  
echo "4096" > /sys/class/net/eth0/mtu  
#重启网络使修改生效
/etc/init.d/networking restart
```

##运行
```bash
python3 server.py
```
后台运行  
```bash
./run.sh 
```
停止
```bash  
./stop.sh
```
重启
```bash  
./restart.sh
```

##作者  
server by 尚红泽<15307130154@fudan.edu.cn>  
client by 刘俊涛<15307130161@fudan.edu.cn>

