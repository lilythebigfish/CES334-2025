# Lab Notes (9/11/2025)

## Previous or Left-over Actions

- [x]  Create a github account (if you haven't already)
- [x]  Create a repo for 334 if you haven't already
- [x]  Create a folder in your 334 repo called raspi-ip (or something) and in that folder, a doc named ip.md
- [x]  Enable SSH [raspi-config]
- [x]  SSH into your RPi from your laptop (you will have to be on the Yale network)[CLI]
- [x]  **Generate an SSH key (or redo if yours doesn’t work)** for use with GitHub so you don't need to enter your creds to pull/push [see GitHub docs below] and add to your GitHub keys. [from Lab 1]
- [x]  **Write a bash script** that writes the current IP address to the ip.md file in your repo and (**if SSH Key is working**) uploads to github.
    - I needed to add a config file to `/.ssh`  to ensure that the key is added locally on reboot

```jsx
Host *
	AddKeysToAgent yes
	IdentityFile ~/.ssh/key
```

## New Actions: Systemd and config file backup

- [x]  Automate uploading so that every time the system boots, the script is run and the IP uploaded to GitHub [Systemd/.bashrc]
    - Understanding systemd: https://documentation.suse.com/smart/systems-management/html/systemd-basics/index.html
    
    ![image.png](Lab%20Notes%20(9%2011%202025)%2026bc6c91db4b802eac13cf3c0f717766/image.png)
    
    - Need to run after the pi is online
    - Need to run as user in same directory as raspberrypi
    - Restart on failure (good if pi is not completely online)
    - Script:

```jsx
[Unit]
Description=Sync Files
After=network-online.target

[Service]
User=student334
ExecStart=/home/student334/Desktop/cs3340/raspberrypi/startup.sh
WorkingDirectory=/home/student334/Desktop/cs3340/raspberrypi/
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

Lily needed to create a new ssh key because the old key has a passkey

- [x]  Create a repo for your config files on GitHub or use same as the ip.md file
    - I also renamed the [`ip.md`](http://ip.md) to `ip.txt` for consistency
- [x]  Discover what config files are important to your system (environment) and add code to your startup script/process to copy those config files to your local config repo [CLI].
    - I save the firmware config, ssh config, and the systemd service config
- [x]  **Push your configs** to GitHub so you can easily recreate your system functionality when your SD card fails.

Realized that the bash script seemed like it wasn’t working because it was rewriting the old IP address, so it seemed like nothing changed and nothing is committed! Here is the updated script:

```jsx
echo "$(date '+%Y-%m-%d %H:%M:%S') - $(hostname -I | awk '{print $1}')" >> ~/CES334-2025/raspberryPi/configs/ip.md

//this adds a timestamp as well
```

### SCP File Copy for Backup to Remote Server or Laptop

- [ ]  Manually copy a file from your Pi to your laptop using [**SCP**](https://man7.org/linux/man-pages/man1/scp.1.html). This is a quick and dirty way to move files around when there's no time for doing the whole Git spiel. [CLI]
    - Must know user, password, and IP of target computer
- [ ]  Outline (in text, in a document to be uploaded to our knowledge share page) the precise steps for using your SSH key (generated above) to automatically send files from your Pi to your Laptop using SCP. [net research]
- [ ]  Write a (Bash) script using the above outline to copy your ip.md file to your laptop.
    - This should authenticate with an ssh key in the same way as pi→github.com.
    - The script should run every 10 minutes that the pi is on. [CLI]
        - This necessitates *always* knowing the IP of your laptop/target computer. For now, just hard-code the IP based on your current location.
        - Test using an every minute schedule
    - Use `crontab` for the scheduling.
- [ ]  Once successful, disable the above 10-minute push cycle, because…