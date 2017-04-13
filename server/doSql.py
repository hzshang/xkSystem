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
	sql="select max(id) from user where id like \"%s%%\""%prefix
	cur.execute(sql)
	fet=cur.fetchall()
	if fet[0][0]==None:
		id= prefix+"0001"
	else:
		id=str(int(fet[0][0])+1)
	sql="insert into user values(\"%s\",\"%s\",%s,\"%s\",\"%s\")"\
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
	where sc.sid=\"%s\" and sc.cid=course.cid and course.tid=user.id"%recvData[user]
	cur.execute(sql)
	return [dict(cname=i[0],rid=i[1],ctime=i[2],tname=i[3]) for i in cur.fetchall()]

def askForCourses(recvData):#学生查看可选课程
	cur=gl.conn.cursor()
	sql="select cid,cname,current,max,ctime,name,ridfrom user,course \
	where id=tid and cid not in \
	(select cid from sc where sid=\"%s\")\""%recvData["user"]
	cur.execute(sql)
	piece=cur.fetchall()[recvData["index"]:recvData["index"]+10]
	array=[dict(cid=i[0],cname=i[1],current=i[2],max=i[3],ctime=i[4],tname=i[5],rid=i[6]) for i in piece]
	return {"total":len(cur.fetchall()),"courses":array}


def selectCourse(recvData):#学生选课
	pass
def quitCourse(recvData):#学生退课
	pass
def addCourse(recvData):#老师加课
	pass
def delCourse(recvData):#老师删课
	pass
def seeMyCourse(recvData):#老师查看所有开设的课程
	pass
def seeMyCourseStudent(recvData):#老师查看选择某课的学生
	pass
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
	12:seeMyCourseStudent
}

def operate(recvData):
	print("request pid:",recvData["pid"])
	return switch[recvData["pid"]](recvData)