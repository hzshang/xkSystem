#global variables
import json
import cymysql

configFile=open('config.json')
config =json.loads(configFile.read())
conn=None
def connect():
	return cymysql.connect(host=config["db_host"],user=config["db_user"],\
	passwd=config["db_passwd"],db=config["db_database"],charset="utf8")
