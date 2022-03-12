如果执行程序 status放回-16则使用sudo运行
若不想使用sudo 则查看88-rule是否执行，可以通过修改 /etc/udev/udev.conf 为debug
$ journalctl -xeu systemd-udevd
查看是否成功

前提是执行过上级目录下的install.sh

待解决的问题:
    由于还使用了MVview，所以在对libMVSDK.so连接的时候一直连到Dahuatech下的libMVSDK
    于是编译能成功，运行时就会报undefined的错误。