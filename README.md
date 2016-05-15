#CMDU for GNOME Panel Applet  
基于GTK2，StartOS(GNOME2)通过，在任务栏显示网速，鼠标悬停显示开机时长、CPU、内存、网速。

![image](preview.png)

开发库：  
Ubuntu：libpanel-applet

StartOS：gnome-panel-dev，gconf-dev，gorbit-dev，libbonobo-dev，libbonoboui-dev，libgnomecanvas-dev，libart-lgpl-dev，gnome-vfs-dev，libcanberra-dev，popt-dev。

Geany编译命令：gcc -o "%e" "%f" -lm `pkg-config --cflags --libs libpanelapplet-2.0`

添加方法：
编译applet.c，修改applet.server文件里的location=程序路径，复制applet.server文件到/usr/lib/bonobo/servers/路径下，重启gnome-panel进程，然后在面板右键里点击添加到面板。
![image](Add%20Applet.png)
