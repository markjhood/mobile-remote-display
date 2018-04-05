# mobile-remote-display
MWC 2010 demo archive: mobile handset to desktop remote screen with proximity detection

In early 2010 while working at Aplix Corporation I designed a proof-of-concept
demonstration of remote display capabilities for phones running Aplix Java
virtual machines (JBlend / J2ME / MIDP3).

The idea was that a user with an JBlend-powered phone could approach a Mac or
Linux desktop, or an embedded Linux car infotainment panel, and receive a
proximity-based notification indicating that the Mac or Linux display was
available to use as a remote display for the phone.  A connection could then
be made through either WiFi or Bluetooth to enable the phone to use the
desktop or panel as an external display and receive user interactions from it.
Once out of proximity the connection would be torn down and the display and
user control would transfer seemlessly back to the phone.

I led a small international team to implement the demo in time for the 2010
Mobile World Congress, with engineers in Asia working on display assets and
the modifications to the JBlend JVM, while I implemented the proximity
detection, the network display protocol, and the display clients for Mac OSX
and Linux.  The display clients were Linux-based and made cross-platform to
OSX by using the GTK-OSX jhbuild system (http://gtk-osx.sourceforge.net).

Most of documentation of the design and build instructions are in the included
doc directory.  Since this package was also intended to be built and used by
customer engineers, special attention was made to provide as much hand-holding
as possible when building, installing, and configuring the demonstration, but
it's unlikely that this package could be built and run today.
