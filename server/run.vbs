Dim RunKcptun
Set fso = CreateObject("Scripting.FileSystemObject")
Set WshShell = WScript.CreateObject("WScript.Shell")
cmdLine ="python server.py"
WshShell.Run cmdLine
WScript.quit