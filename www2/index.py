#!/usr/bin/python
import sys, time, os
sys.path.insert(0, "/usr/lib/python2.7/bridge")
from bridgeclient import BridgeClient

client=BridgeClient()
temp = float(client.get("RCT03_Temperature"))
humidity = float(client.get("RCT03_Humidity"))
sump = client.get("Sump_Power")
if "QUERY_STRING" in os.environ:
    unit = os.environ["QUERY_STRING"]
else:
    unit = "F"
if unit == "":
    unit = "F"
if unit=="C":
    temp = (temp - 32)*5/9

print "Content-type: text/html"
print
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
print ("%.1f" % temp)
print """
</p>
    <p id="unit" class="lcd">
"""
print unit
print """
</p>
    <p id="humidity" class="lcd">
"""
print ("%.1f%%" % humidity)
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
<p id="sump">Sump is
"""
print sump
print """
</p>
</body>
</html>
"""
