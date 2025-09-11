echo "Starting script..."

cp /boot/firmware/config.txt ./configs
hostname -I | awk '{print $1}' > ~/CES334-2025/raspberryPi/configs/ip.md
git add .
git commit -m"updated config files and ip"
git push

echo "Done!"