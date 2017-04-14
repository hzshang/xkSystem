#operate on mysql

import gl

def signin(recvData):
	cur=gl.conn.cursor()
	sql="select id,pwd from user where id=%s and pwd=%s"%(recvData["user"],recvData["pwd"])
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
	sql="insert into user values('%s','%s',%s,'%s','%s')"\
	%(id,recvData["name"],str(recvData["grade"]),recvData["did"],recvData["pwd"])
	cur.execute(sql)
	gl.conn.commit()
	#log
	print("%s sign up"%id)
	return {"user":id}

def resetpwd(recvData):
	cur=gl.conn.cursor()
	sql="update user set pwd=%s where id=%s and name=%s"\
	%(recvData["pwd"],recvData["user"],recvData["name"])
	cur.execute(sql)
	cur.commit()
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
	return [i[0]+"."+i[1] for i in cur.fetchall()]

def askForSchedule(recvData):
	cur=gl.conn.cursor()
	sql="select cname,rid,ctime,user.name\
	from course,user,sc\
	where sc.sid='%s' and sc.cid=course.cid and course.tid=user.id"%recvData["user"]
	cur.execute(sql)
	return [dict(cname=i[0],rid=i[1],ctime=i[2],tname=i[3]) for i in cur.fetchall()]

def askForCourses(recvData):#学生查看可选课程
	cur=gl.conn.cursor()
	sql="select cid,cname,current,max,rid,ctime,name,creditfrom course,user\
	where tid=id and cid not in (select cid from sc where sid='%s') and cname like '%%%s%%'\
	and cid like %%%s%% and tname like %%%s%%"%(recvData["user"],recvData["cname"],recvData["cid"],recvData["tname"])
	cur.execute(sql)
	piece=cur.fetchall()[recvData["index"]*10:recvData["index"]*10+10]
	array=[dict(cid=i[0],cname=i[1],current=i[2],max=i[3],ctime=i[4],tname=i[5],rid=i[6],credit=i[7]) for i in piece]
	return {"total":len(cur.fetchall()),"courses":array}

def initSchedule(user):
	cur=gl.conn.cursor()
	time=["0000000000" for i in range(0,7)]
	sql="select ctime from sc,course where sc.cid=course.cid and sc.sid='%s'"%recvData["user"]
	cur.execute(sql)
	for i in cur.fetchall():
		ctime=i[0]
		j=0
		while j<int(ctime[2]):
			time[int(ctime[0])][int(ctime[1])+j]='1'
			j=j+1
	return time

def checkTimeIsOK(ctime,time):#检查课程时间是否冲突
	j=0
	ok=True
	while j<int(ctime[2]):
		if time[int(ctime[0])][int(ctime[1])+j]!='0':
			ok=False
			break
	return ok

	
def setCurrentCourse(cid,added):
	sql="select current,max from course where cid='%s'"%cid
	cur=gl.conn.cursor()
	cur.execute(sql)
	current,max=cur.fetchall()[0]
	if current+added<=max and current+added>=0:
		sql="update course set current=%s where cid='%s'"%((current+added),cid)
		return True
	else:
		return False

def selectCourse(recvData):#学生选课
	time=initSchedule(recvData["user"])

	cur=gl.conn.cursor()
	sql="select ctime from course where cid='%s'"%recvData["cid"]
	cur.execute(sql)
	ctime=cur.fetchall()[0][0]

	if not checkTimeIsOK(ctime,time):
		return {"state":False}
	try:
		if setCurrentCourse(recvData["cid"],1):
			sql="insert into sc values('%s','%s')"%(recvData["user"],recvData["cid"])
			cur.execute(sql)
			cur.commit()
			ret={"state":True}
		else:
			ret={"state":False}
	except Exception as e:
		print(e)
		ret={"state":False}
	return ret


def quitCourse(recvData):#学生退课
	cur=gl.conn.cursor()
	try:
		sql="delete from sc where sid='%s' and cid='%s'"%(recvData["user"],recvData["cid"])
		cur.execute(sql)
		cur.commit()
		setCurrentCourse(cid,-1)
		ret={"state":True}
	except Exception as e:
		print(e)
		ret={"state":False}
	return ret

def findMaxCourseID(tid):#查找最大的课程id
	cur=gl.conn.cursor()
	sql='select max(cid) from course where tid="%s"'%tid
	cur.execute()
	if len(cur.fetchall())==0:
		cid=tid+"00"
	else:
		cid=int(cur.fetchall()[0][0])+1
	return cid

def addCtimeToRoom(rid,ctime):#将课程时间添加到某教室时间表
	dic={'1':'monday','2':'tuesday','3':'wednesday','4':'thursday','5':'friday','6':'satarday','7':'sunday'}
	cur=gl.conn.cursor()
	sql="select %s from room where rid='%s'"%(dic[ctime[0]],rid)
	cur.execute(sql)
	time=cur.fetchall()[0][0]
	j=0
	while j<int(ctime[2]):
		time[int(ctime[1])+j]='1'
		j=j+1
	sql="update room set %s=%s where rid='%s'"%(dic[ctime[0]],time,rid)
	cur.execute(sql)
	cur.commit()
	return


def addCourse(recvData):#老师加课
	cur=gl.conn.cursor()
	sql="select * from room where rid='%s'"%recvData["rid"]
	cur.execute(sql)
	if not checkTimeIsOK(recvData["ctime"],cur.fetchall()[0][1:]):
		return {"state":False}
	cid=findMaxCourseID(recvData["tid"])
	sql='insert into course values("%s","%s",%s,%s,"%s","%s","%s",%s)'\
	%(cid,recvData["cname"],0,recvData["max"],recvData["user"],recvData["ctime"],recvData["rid"],recvData["credit"])
	cur.execute(sql)
	cur.commit()
	addCtimeToRoom(recvData["rid"],recvData["ctime"])
	return {"state":True,"cid":cid}

def delCtimeFromRoom(rid,ctime):#将课程时间从某教室时间表移除
	dic={'1':'monday','2':'tuesday','3':'wednesday','4':'thursday','5':'friday','6':'satarday','7':'sunday'}
	cur=gl.conn.cursor()
	sql="select %s from room where rid='%s'"%(dic[ctime[0]],rid)
	cur.execute(sql)
	time=cur.fetchall()[0][0]
	j=0
	while j<int(ctime[2]):
		time[int(ctime[1])+j]='0'
		j=j+1
	sql="update room set %s=%s where rid='%s'"%(dic[ctime[0]],time,rid)
	cur.execute(sql)
	cur.commit()
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
		cur.commit()
		delCtimeFromRoom(*data)
	except Exception as e:
		print(e)
		cur.rollback()
		return {"state":False}
	return {"state":True}

def seeMyCourse(recvData):#老师查看所有开设的课程
	sql="select * from course where tid='%s'"%recvData["user"]
	cur=gl.conn.cursor()
	cur.execute(sql)
	return [dict(cid=i[0],cname=i[1],current=i[2],max=i[3],ctime=i[5],rid=i[6],credit=i[7]) for i in cur.fetchall()]


def seeMyCourseStudent(recvData):#老师查看选择某课的学生
	sql="select user.id,user.name from user,sc where user.id=sc.sid and sc.cid='%s'"%recvData["cid"]
	cur=gl.conn.cursor()
	cur.execute(sql)
	return [dict(id=i[0],name=i[1]) for i in cur.fetchall()]

def askForInformation(recvData):#请求用户信息
	sql="select name,dname from user,department where user.did=department.did and id='%s'"%recvData["user"]
	cur=gl.conn.cursor()
	cur.execute(sql)
	fet=cur.fetchall()[0]
	return dict(name=fet[0],dname=fet[1])

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
	13:askForInformation,
	14:askForRoomInformation,
	15:askForRoom
}

def operate(recvData):
	print("request pid:",recvData["pid"])
	return switch[recvData["pid"]](recvData)