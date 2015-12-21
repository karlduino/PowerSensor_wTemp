## temperature/humidity web panel

Slightly modified version of Project 1 in "Arduino meets Linux" by Bob Hammell.

I'm using an
[RHT003 temperature/humidity sensor](https://www.sparkfun.com/products/10167)
(equivalent to [DHT22 at Adafruit](https://www.adafruit.com/products/385))
rather than the
[MCP9808 temperature sensor](https://www.adafruit.com/products/1782) that he used.

Original version of the css, fonts, and images at
<http://www.arduinomeetslinux.com/download/project1-a.tar.gz>.

Added the following to my `/etc/config/uhttpd` file:

```
# certificate defaults for px5g key generator
config cert px5g
# config for 2nd website
config uhttpd site2
	list listen_http	0.0.0.0:80
	option home		/mnt/sda1/www2
	option rfc1918_filter	0
	option max_requests	2
	option network_timeout	30
	option tcp_keepalive	1
	option index_page	index.py
	list interpreter	".py=/usr/bin/python"	
	option script_timeout	60
```

(Also revised the main configuration to have

```
    list listen_http    0.0.0.0:81`
```

Created a directory `/mnt/sda1/www2` and moved `index.py`, `css/`,
`fonts/`, and `images/` there. Installed `TempHumid_WebPanel.ino` on
Yun.
