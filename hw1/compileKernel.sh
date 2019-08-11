#!/bin/bash

decision=0
while true; do 
    read -n1 -resp "Choose:(1) Fast; (2) Controlled; (3) Fast+Modules; (4) Cont+Modules (e) Exit > " key
    if [ "$key" = '1' ]; then
		break
    elif [ "$key" = '2' ]; then
        decision=1
		break
	elif [ "$key" = '3' ]; then
        decision=4
		break
	elif [ "$key" = '4' ]; then
        decision=2
		break
    elif [ "$key" = 'e' ]; then
		exit
    else 
		echo ""
	    echo ">>>  Press 'c' - to Continue or 'e' - to Exit :  "
		continue
    fi
done
cd /usr/src/linux-2.4.18-14custom
make bzImage
if (( $decision == 1 || $decision == 2 )); then
    echo ""
    echo -e "\e[31m>>>  Check above if your code compiled well, no errors and etc\e[0m"
	while true; do 
		read -n1 -rsp ">>>  Press 'c' - to Continue or 'e' - to Exit :  " key
            echo ""
		if [ "$key" = 'c' ]; then
			break
		elif [ "$key" = 'e' ]; then
			echo "Good luck!"
			exit
		else 
			echo ""
			continue
		fi
	done
fi
if (( $decision == 2 || $decision == 4 )); then
	make modules
	make modules_install
fi
cd arch/i386/boot
sleep 2
cp bzImage /boot/vmlinuz-2.4.18-14custom -f
sleep 2

if (( $decision == 1 || $decision == 2 )); then
	while true; do 
		read -n1 -rsp ">>>  Press 'y' - to reboot or 'n' - to exit :  " key
		if [ "$key" = 'y' ]; then
			reboot
		elif [ "$key" = 'n' ]; then
			echo "Good luck!"
		else 
			echo ""
			continue
		fi
		exit
	done
fi
reboot
