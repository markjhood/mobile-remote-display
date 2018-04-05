This directory contains the MIDP3 Navigation Demo midlet (NavigationDemo.jad
and .jar) and the Slide Presentation midlet (PPTMIDlet.jad and .jar).  They
need to be copied to the phone through ActiveSync or some other
synchronization application and then installed into the JBlend Java VM.  The
directory jvm-wm-cab contains CAB files that work with these midlets.

The CAB is installed by clicking on the file in the phone's File Explorer
utility.  The midlet can then be installed by clicking on the midlet's JAD
file.  The JAR and JAD file extensions are usually obscured on Windows Mobile,
so the best way to tell them apart is by looking at their sizes: the JAD file
is only a few bytes.  Note that the phone will delete the midlet files once
the midlet is installed, so you may want to keep a copy around on the phone in
case you need to re-install them.

The midlet sometimes has a problem with allocating the large images that are
used to cover the phone's display.  It should be fixed at some point, but for
now the latest CAB works around the problem by mapping shared memory into the
VM.  See the README.txt in the jvw-wm-cab directory for more information.
