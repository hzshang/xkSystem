#operate on mysql

import gl
import json
def signin(recvData):
	cur=gl.conn.cursor()
	sql="select id,pwd from user where id='%s' and pwd='%s'"%(recvData["user"],recvData["pwd"])
	cur.execute(sql)
	ret=dict(state=(len(cur.fetchall())==1))
	#log
	if ret["state"]:
		print("%s sign in. Successed" %recvData["user"])
	else:
		print("%s sign in. Failed" %recvData["user"])
	return ret

def signup(recvData):
	cur=gl.conn.cursor()
	prefix=str(recvData["grade"])+recvData["did"]
	sql="select max(id) from user where id like '%s%%'"%prefix
	cur.execute(sql)
	fet=cur.fetchall()
	if fet[0][0]==None:
		id= prefix+"0001"
	else:
		id=str(int(fet[0][0])+1)
		while len(id)<9:
			id='0'+id
	sql="insert into user values('%s','%s',%s,'%s','%s')"%(id,recvData["name"],recvData["grade"],recvData["did"],recvData["pwd"])
	cur.execute(sql)
	gl.conn.commit()
	#log
	print("%s sign up"%id)
	return {"user":id}

def resetpwd(recvData):
	cur=gl.conn.cursor()
	sql="update user set pwd='%s' where id='%s' and name='%s'"\
	%(recvData["pwd"],recvData["user"],recvData["name"])
	cur.execute(sql)
	gl.conn.commit()
	ret=dict(state=(cur.rowcount==1))
	#log
	if ret["state"]:
		print("%s reset pwd. Successed"%recvData["user"])
	else:
		print("%s reset pwd. Failed"%recvData["user"])
	return ret

def askForDepart(recvData):
	cur=gl.conn.cursor()
	sql="select * from department"
	cur.execute(sql)
	depart=[i[0]+"."+i[1] for i in cur.fetchall()]
	#log
	print("askForDeaprt")
	return depart

def askForSchedule(recvData):
	cur=gl.conn.cursor()
	sql="select cname,rid,ctime,user.name,course.cid\
	from course,user,sc\
	where sc.sid='%s' and sc.cid=course.cid and course.tid=user.id"%recvData["user"]
	cur.execute(sql)
	schedule=[dict(cname=i[0],rid=i[1],ctime=i[2],tname=i[3],cid=i[4]) for i in cur.fetchall()]
	#log
	print("askForSchedule")
	return schedule

def askForCourses(recvData):#学生查看可选课程
	cur=gl.conn.cursor()
	if gl.round==2:
		sql="select cid,cname,current,max,rid,ctime,name,credit from course,user\
		where tid=id and cid not in (select cid from sc where sid='%s') and cname like '%%%s%%'\
		and cid like '%%%s%%' and name like '%%%s%%'"%(recvData["user"],recvData["cname"],recvData["cid"],recvData["tname"])
	else:
		sql="select cid,cname,current,max,rid,ctime,name,credit from course,user\
		where tid=id and did in (select did from user where id='%s') and cid not in (select cid from sc where sid='%s') and cname like '%%%s%%'\
		and cid like '%%%s%%' and name like '%%%s%%'"%(recvData["user"],recvData["user"],recvData["cname"],recvData["cid"],recvData["tname"])
	cur.execute(sql)
	fet=cur.fetchall()
	piece=fet[recvData["index"]*10:recvData["index"]*10+10]
	print(len(piece))
	array=[dict(cid=i[0],cname=i[1],current=i[2],max=i[3],ctime=i[5],tname=i[6],rid=i[4],credit=i[7]) for i in piece]
	#Log
	print("askForCourse")
	return {"total":len(fet),"courses":array}

def initSchedule(user):
	cur=gl.conn.cursor()
	time=["00000000000" for i in range(0,7)]
	sql="select ctime from sc,course where sc.cid=course.cid and sc.sid='%s'"%user
	cur.execute(sql)
	for i in cur.fetchall():
		ctime=i[0]
		lis=list(time[int(ctime[0])-1])
		for j in range(0,int(ctime[2])):
			lis[int(ctime[1])+j]='1'
		time[int(ctime[0])-1]="".join(lis)
	return time

def checkTimeIsOK(ctime,time):#检查课程时间是否冲突
	ok=True
	for j in range(0,int(ctime[2])):
		if time[int(ctime[0])-1][int(ctime[1])+j]=='1':
			ok=False
			break
	return ok

def setCurrentCourseNum(cid,added):
	sql="select current,max from course where cid='%s'"%cid
	cur=gl.conn.cursor()
	cur.execute(sql)
	current,max=cur.fetchall()[0]
	if current+added<=max and current+added>=0:
		sql="update course set current=%s where cid='%s'"%((current+added),cid)
		cur.execute(sql)
		gl.conn.commit()
		return True
	else:
		return False

def selectCourse(recvData):#学生选课
	time=initSchedule(recvData["user"])
	cur=gl.conn.cursor()
	sql="select ctime from course where cid='%s'"%recvData["cid"]
	cur.execute(sql)
	fet=cur.fetchall()[0]
	if fet==None or (not checkTimeIsOK(fet[0],time)):
		return {"state":False}
	try:
		if setCurrentCourseNum(recvData["cid"],1):
			sql="insert into sc values('%s','%s')"%(recvData["user"],recvData["cid"])
			cur.execute(sql)
			gl.conn.commit()
			ret={"state":True}
		else:
			ret={"state":False}
	except Exception as e:
		print(e)
		ret={"state":False}
	#log
	print('%s selects course %s:%s'%(recvData["user"],recvData["cid"],ret["state"]))
	return ret

def quitCourse(recvData):#学生退课
	cur=gl.conn.cursor()
	try:
		sql="delete from sc where sid='%s' and cid='%s'"%(recvData["user"],recvData["cid"])
		cur.execute(sql)
		if cur.rowcount==1:
			gl.conn.commit()
			setCurrentCourseNum(recvData["cid"],-1)
			ret={"state":True}
		else:
			gl.conn.rollback()
			ret={"state":False}
	except Exception as e:
		print(e)
		ret={"state":False}
	#log
	print('%s quit course %s:%s'%(recvData['user'],recvData['cid'],ret['state']))
	return ret

def findMaxCourseID(tid):#查找最大的课程id
	cur=gl.conn.cursor()
	sql='select max(cid) from course'
	cur.execute(sql)
	fet=cur.fetchall()[0][0]
	if fet==None:
		cid="100000"
	else:
		cid=int(fet)+1
	return str(cid)

def OpeCtimeOnRoom(rid,ctime,char):#将课程时间添加到某教室时间表
	dic={'1':'monday','2':'tuesday','3':'wednesday','4':'thursday','5':'friday','6':'satarday','7':'sunday'}
	cur=gl.conn.cursor()
	sql="select %s from room where rid='%s'"%(dic[ctime[0]],rid)
	cur.execute(sql)
	time=cur.fetchall()[0][0]
	lis=list(time)
	for j in range(0,int(ctime[2])):
		lis[int(ctime[1])+j]=char
	time="".join(lis)
	sql="update room set %s='%s' where rid='%s'"%(dic[ctime[0]],time,rid)
	cur.execute(sql)
	gl.conn.commit()
	return

def addCourse(recvData):#老师加课
	cur=gl.conn.cursor()
	sql="select * from room where rid='%s'"%recvData["rid"]
	cur.execute(sql)
	if not checkTimeIsOK(recvData["ctime"],cur.fetchall()[0][1:]):
		ret={"state":False}
	else:
		cid=findMaxCourseID(recvData["user"])
		sql='insert into course values("%s","%s",%s,%s,"%s","%s","%s",%s)'\
		%(cid,recvData["cname"],0,recvData["max"],recvData["user"],recvData["ctime"],recvData["rid"],recvData["credit"])
		cur.execute(sql)
		gl.conn.commit()
		OpeCtimeOnRoom(recvData["rid"],recvData["ctime"],'1')
		ret={"state":True,"cid":cid}
	#log
	print('%s adds course:%s'%(recvData['user'],ret['state']))
	return

def delCourse(recvData):#老师删课
#先删除sc,再删除c,再修改room
	cur=gl.conn.cursor()
	try:
		sql="delete from sc where cid='%s'"%recvData["cid"]
		cur.execute(sql)
		sql="select rid,ctime from course where cid='%s'"%recvData["cid"]
		cur.execute(sql)
		data=cur.fetchall()[0]
		sql="delete from course where cid='%s'"%recvData["cid"]
		cur.execute(sql)
		gl.conn.commit()
		OpeCtimeOnRoom(*data,'0')
		ret={"state":True}
	except Exception as e:
		print(e)
		gl.conn.rollback()
		ret={"state":False}
	#log
	print('%s deletes course %s:%s'%(recvData['user'],recvData['cid'],ret['state']))
	return ret

def seeMyCourse(recvData):#老师查看所有开设的课程
	sql="select * from course where tid='%s'"%recvData["user"]
	cur=gl.conn.cursor()
	cur.execute(sql)
	return [dict(cid=i[0],cname=i[1],current=i[2],max=i[3],ctime=i[5],rid=i[6],credit=i[7]) for i in cur.fetchall()]

def seeMyCourseStudent(recvData):#老师查看选择某课的学生
	sql="select user.id,user.name,user.grade,dname from user,sc,department where user.id=sc.sid and sc.cid='%s' and department.did=user.did"%recvData["cid"]
	cur=gl.conn.cursor()
	cur.execute(sql)
	array=[dict(id=i[0],name=i[1],grade=i[2],dname=i[3]) for i in cur.fetchall()]
	return array

def askForUserInformation(recvData):#请求用户信息
	sql="select name,dname from user,department where user.did=department.did and id='%s'"%recvData["user"]
	cur=gl.conn.cursor()
	cur.execute(sql)
	fet=cur.fetchall()[0]
	return dict(name=fet[0],dname=fet[1],round=gl.round)

def askForRoomInformation(recvData):
	cur=gl.conn.cursor()
	sql='select * from room where rid="%s"'%recvData["rid"]
	cur.execute(sql)
	i=cur.fetchall()[0]
	return i[1:]

def askForRoom(recvData):
	cur=gl.conn.cursor()
	sql='select rid from room'
	cur.execute(sql)
	return [i[0] for i in cur.fetchall()]

def askForRound(recvData):
	return {"round":gl.round}

switch={
	1:signin,
	2:signup,
	3:resetpwd,
	4:askForDepart,
	5:askForSchedule,
	6:askForCourses,
	7:selectCourse,
	8:quitCourse,
	9:addCourse,
	10:delCourse,
	11:seeMyCourse,
	12:seeMyCourseStudent,
	13:askForUserInformation,
	14:askForRoomInformation,
	15:askForRoom
}

def operate(recvData):
	print('recvPid:',recvData["pid"])
	return switch[recvData["pid"]](recvData)