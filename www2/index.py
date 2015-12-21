#!/usr/bin/python
import sys, time, os
sys.path.insert(0, "/usr/lib/python2.7/bridge")
from bridgeclient import BridgeClient

# function to format humidity and temperature values
def format (val, extra=""):
    if val == "" or val == None:
        return("NA")
    return(("%.1f" % float(val)) + extra)


# get units 
if "QUERY_STRING" in os.environ:
    unit = os.environ["QUERY_STRING"]
if unit != "C": # only allow C or F
    unit = "F"

# grab temp, humidity, and sump status
client=BridgeClient()
temp = client.get("RCT03_Temperature")
if unit=="C":
    if temp != "" and temp != None:
        temp = float(temp)
        temp = (temp - 32)*5/9
temp = format(temp)
humidity = format(client.get("RCT03_Humidity"), "%")
sump = client.get("Sump_Power")

# print out web page
print "Content-type: text/html"
print """
<!doctype html>
<html>
    <head>
        <title>Karlduino temp/humidity monitor</title>
        <style>body { font-family: sans-serif; }</style>
        <link rel="stylesheet" href="/css/normal.css" type="text/css"/>
        <meta http-equiv="refresh" content="10">
        <meta charset="utf-8">
    </head>
<body>
<div id="panel">
    <p id="temperature" class="lcd">
"""
print temp
print """
</p>
    <p id="unit" class="lcd">
"""
print unit
print """
</p>
    <p id="humidity" class="lcd">
"""
print humidity
print """</p>
    <p id="date" class="lcd">
"""
print time.strftime("%Y-%m-%d")
print """
</p>
    <p id="time" class="lcd">
"""
print time.strftime("%H:%M")
print """
</p>
    <input type="button" id="btnUnit"
"""
if unit == "F":
    print "onclick=\"javascript:location.href='/index.py/?C'\""
else:
    print "onclick=\"javascript:location.href='/index.py/?F'\""
print """
           value="Unit"/>
    <input type="button" id="btnRefresh"
           onclick="location.reload();"
           value="Refresh"/>
</div>    
<p id="sump" style="font-family:monospace;">Sump is
"""
print sump
print """
</p>
</body>
</html>
"""
