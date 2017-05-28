#
# Regular cron jobs for the babel package
#
0 4	* * *	root	[ -x /usr/bin/babel_maintenance ] && /usr/bin/babel_maintenance
