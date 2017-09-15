# TrackingBlocker

This is an application for updating a list of DNS names and re-routing the domain name IP address query
to the localhost (127.0.0.1)

The effect of re-routing the IP address lookup is that instead of some website receiving the request
the request never leaves your computer. By doing this, you block access to that website on the computer
regardless of which browser is being used. Your computer thinks the website in question (e.g. espn.com)
is at IP address 127.0.0.1 so no information or request is received by the website.

This transparency of this application is ideal to security. For plugins like AdBlocker, you cannot
see where those requests are being sent to. Also, you cannot see what AdBlocker is doing with your
requests (hint: they are logging those requests and your traffic and they make money selling that
information to companies willing to pay for it so, really, AdBlocker removes ads but your information
is still compromised).

The C++ program "combine_hosts" is used to sort and update the list of domains being blocked. Run:

$ ./combine_hosts -h

to see the command line options. You have to specify your original hosts file (located in /etc/hosts on
Linux and macOS, sorry no Windows support). The application will also download the latests WINHOSTS
file, which is a freely available HOSTS file that maintains a database of websites known for tracking.

The Python application "TrackingBlocker.py" handles the usage. It is wrapped around a bash script
"tracking_blocker" that ensures that the necessary privileges are acquired to modify the HOSTS file.
This python application will handle turning the tracking blocker on and off and allow one to add
domains (e.g. you are on a website, see an ad, right-click and copy the link address of the ad,
and then use:

$ sudo tracking_blocker add ads.gsd.com

Strip all the junk after the .com/.net/etc. and do not include with the www. prefix or anything before
that (e.g. http://). Two entries: ads.gsd.com and www.ads.gsd.com will be added to the tracking blocker
and then run:

$ sudo tracking_blocker status on

to active the updated list. If at any time you want to disable the tracking/ad blocker, run:

$ sudo tracking_blocker status off

To remove a particular entry, replace 'add' with 'remove', e.g.:

$ sudo tracking_blocker remove ads.gsd.com

The removals are stored because on occassion, when updating from WINHOSTS, previously removed
domains will be added back in. To re-remove this previous removals run:

$ sudo tracking_blocker remove -l
or
$ sudo tracking_blocker remove --load-previous-removes

There is preliminary support for a Qt interface (not fully working currently) and preliminary
support for different profiles (default is '0' i.e., hosts.profile.0). This idea behind this
is the ability to maintain a basic adblocker (for adult use) and another for children/teenager
use i.e. this is an EXCELLENT way to ensure parental-controls because it is browser-independent,
free, and ensures no work-around without the superuser password.

