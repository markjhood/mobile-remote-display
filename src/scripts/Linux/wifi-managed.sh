#! /bin/bash
# Run this script with sudo to set WiFi into managed (normal) mode.  
# Ex: sudo ./wifi-managed

# eth1 is the wireless interface on a ThinkPad T43 running Ubuntu 8.04.
# Might be different for a different distro.
ifconfig eth1 down

# iwconfig is like ifconfig except specifically for wireless.
iwconfig eth1 mode managed

# bring eth1 interface back up
ifconfig eth1 up
