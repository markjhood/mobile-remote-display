#! /bin/bash
# Run this script with sudo to set WiFi into ad-hoc, link-local mode.
# Things are probably not done optimally here.
#
# Ex: sudo wifi-adhoc

# eth1 is the wireless interface on a ThinkPad T43 running Ubuntu 8.04.
ifconfig eth1 down

# iwconfig is like ifconfig except specifically for wireless.
iwconfig eth1 mode ad-hoc

# Would like to use 'auto' but it doesn't get accepted.
iwconfig eth1 channel 4

# This is the SSID - set to the hostname
iwconfig eth1 essid `hostname`

# This is the WEP encryption key.  It must be set and must be 10 digits.
iwconfig eth1 key 1301130113

# Fake up a link-local IP address with ifconfig and then bring the interface
# back up.  Could also run 'dhclient eth1', but it takes a while waiting for
# what will be a non-existent DHCP offer.
ifconfig eth1 169.254.101.1
ifconfig eth1 up
