echo "Starting script..."

cp /boot/firmware/config.txt ./configs
echo "$(date '+%Y-%m-%d %H:%M:%S') - $(hostname -I | awk '{print $1}')" >> ~/CES334-2025/raspberryPi/configs/ip.md
git add .
git commit -m"updated config files and ip"
git push

echo "Done!"