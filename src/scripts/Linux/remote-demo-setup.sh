#!/bin/bash
#
# Set up the Aplix MIDP 3.0 Remote Display Demo on Ubuntu 8.04.  This will
# take existing values from the BlueProximity config file if it exists and
# then overwrite the entire file.
#
# The demo software must already be installed.  If Bluetooth proximity
# detection is needed then BlueProximity must also be installed and the phone
# must already be paired with the bluetooth module running on the laptop.
# This script will also set up ad-hoc WiFi networking if desired; otherwise
# the phone must connect to an existing WiFi access point.
#
# This script will start the remote-jblend application and optionally
# BlueProximity before exiting.  They can be afterwards be run from the
# launcher icons on the desktop without invoking this script.  Starting
# BlueProximity by itself will not launch the remote display until the phone
# moves out of proximity and back again; if this is not desired behavior, then
# either use this script or run the remote display manually from the launcher
# icon after starting BlueProximity.

zenity --question --text "This script sets up the information needed to run \
the MIDP3 Remote Display demo on a specific phone, and then starts the demo.  

After running this script the demo can be run again with the same phone by \
either 1) starting BlueProximity to have the remote display launched based \
on phone proximity, 2) running the remote display manually, or 3) by running \
this script again.  If you need to use a different phone, you'll have to run \
this script again.

The script will ask you for the demo mode.  You can either display the small \
or large dashboard mockups and use BlueProximity to launch the demo, or \
display a slide presentation window with manual launching.   You will need to \
confirm the WiFi IP addresses of the phone and optionally the Bluetooth MAC \
address if proximity detection is desired.  This script then overwrites the \
contents of your current BlueProximity configuration file \
(~/.blueproximityrc). Any running instances of BlueProximity and \
remote-jblend will be killed.

The demo software must already be installed.  For proximity detection the \
BlueProximity software must also be installed, and the phone must already be \
paired with the bluetooth module on the laptop.  This script will set up \
ad-hoc WiFi networking if desired; otherwise the phone must connect to an \
existing WiFi access point.

Continue?"
if [ $? != 0 ]; then exit; fi

# Make sure the demo isn't running.
prog=`ps -ef | grep remote-jblend | grep -v grep | awk '{print $2}'`
if [ -n "$prog" ]; then
  kill $prog
fi

btprox=`ps -ef | grep proximity.py | grep -v grep | awk '{print $2}'`
if [ -n "$btprox" ]; then
  kill $btprox
fi

# Get current bt addr and ip addr from here.  Also use the file to hold the
# demo mode.
conf=~/.blueproximityrc

# Get the current demo mode.
mode=1
if [ -r $conf ]; then
  mode=`grep demo_mode $conf | awk '{print $3}'`
  if [ -z "$mode" ]; then
    mode=1
  fi
fi

# Ask for the desired demo mode or confirm the current one.  Use --entry
# instead of --list since the latter doesn't respond to a Return key for
# accepting the current setting as the default.
mode=$(zenity --entry --entry-text="$mode" --title="Demo Mode" \
  --text="Enter the desired demo mode:

1  for the small dashboard mockup;
2  for the large dashboard mockup;
3  for the slide presentation window.")
if [ $? != 0 ]; then exit; fi

# Proximity detection is used for demo modes 1 and 2.
if [ $mode == 1 ] || [ $mode == 2 ]; then
  useproximity=1
else
  useproximity=0
fi

btaddr="11:22:33:44:55:66"
if [ -r $conf ]; then
  btaddr=`grep ^device_mac $conf | awk '{print $3}'`
fi
  
if [ $useproximity == 1 ]; then
  # Ask for a new Bluetooth MAC address or confirm an existing one.
  btaddr=$(zenity --entry --title="Bluetooth Address" \
    --text="Enter or confirm the Bluetooth MAC address of the phone." \
    --entry-text=$btaddr)
  if [ $? != 0 ]; then exit; fi
fi

ipaddr="111.222.333.444"
if [ -r $conf ]; then
  ipaddr=`grep ^unlock_command $conf | awk '{print $5}'`
fi

# Offer to set up an ad-hoc WiFi network.  
zenity --question --text "Do you want to use an existing WiFi network?

Press Cancel if you want to set up a new ad-hoc WiFi network instead.  Make \
sure WiFi is turned on and that you are not connected to a managed WiFi access \
point already, or the ad-hoc setup will fail.  You may need to connect a cable \
and start a wired connection to break an automatic association with a known \
access point."
if [ $? != 0 ]; then
  gksudo wifi-adhoc
  name=`hostname`
  zenity --info --text="Connect the phone to the ad-hoc network named \
\"$name\" using the WEP network key \"1301130113\"."
fi

# Ask for a new phone IP address or confirm an existing one.
ipaddr=$(zenity --entry --title="IP Address" \
  --text="Enter or confirm the IP address of the phone." \
  --entry-text=$ipaddr)
if [ $? != 0 ]; then exit; fi

# Create a new BlueProximity configuration file.  The demo_mode attribute
# isn't used by BlueProximity but doesn't seem to affect it.  It can't be
# placed in a comment since BlueProximity also overwrites this file, but
# without comments for attributes it hasn't read.  Fortunately, BlueProximity
# will also write out the demo_mode attribute if it reads it.
cat > $conf <<EOF
demo_mode = $mode
device_mac = $btaddr
device_channel = 1
lock_command = pkill remote-jblend
unlock_command = remote-jblend -c $ipaddr -p 9999 -s $mode -bg 1
lock_distance = 1
lock_duration = 2
unlock_distance = 0
unlock_duration = 2
proximity_command = gnome-screensaver-command -p
proximity_interval = 60
buffer_size = 1
log_to_syslog = False
log_syslog_facility = local7
log_to_file = True
log_filelog_filename = $HOME/blueproximity.log
EOF

# Start BlueProximity if needed.
if [ $useproximity == 1 ]; then
  blueproximity &
fi

# start the remote display
remote-jblend -c $ipaddr -p 9999 -s $mode -bg 1
