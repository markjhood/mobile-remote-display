This directory contains the the Java installation CABs for JBlend Windows
Mobile.  The desired CAB, along with the multidisplay demo midlet, needs to be
copied to the phone through ActiveSync or some other application and then
installed onto the phone.

20100210B-Midp3DemoCab.CAB is the first VM that worked with the demo midlet.
It sometimes has problems allocating the large images used in the midlet.
This will appear as a hang of the midlet when selecting a POI (Point Of
Interest).

20100329-Midp3DemoCab.CAB fixes the allocation problem by mmap'ing a large
shared memory area into the VM.  However, it is a little slower, usually
requiring 1 - 3 seconds to allocate the images and send them to the remote
display.  However, it also works with the Life Manager demo and so it is the
recommend CAB to install.

Eventually the allocation problem must be fixed in the demo midlet itself.
It must render the POI icons onto a shared background image instead of
allocating separate full images for each POI display.

The display protocol is implemented a little strangely in these VMs.  There is
a separate thread, invoked through a timer, that once per second flushes
whatever image was rendered last for the remote display.  Since it runs
outside the VM, it will continue to send FLUSH commands to the remote display
even after the midlet or VM has exited, so the remote display always remains
visible and showing the last transmitted image.  This happens even though the
connection is closed by the midlet since the connection is immediately
re-established by the timer thread when the midlet exits.

The remote display works around this by detecting the closed connection and
then setting the current image format to IMAGE_NULL before hiding itself.  If
a FLUSH then is then received without a new image being transmitted (the image
format remaining set to IMAGE_NULL), then the flush is ignored and the remote
display remains hidden.
