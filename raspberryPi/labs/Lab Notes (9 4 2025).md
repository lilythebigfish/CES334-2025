# Lab Notes (9/4/2025)

### Actions

- [x]  Be sure you have filled this out for access to AKW 123: [https://www.notion.so/scottpetersen/F24-Names-and-NetIDs-855a7c76a0cf46acbc7d5ab1cc0ef892](https://www.notion.so/855a7c76a0cf46acbc7d5ab1cc0ef892?pvs=21)
- [x]  Create and keep a log of the steps you take below. Include links for working solutions, sticking points, etc. Highlights should be added to the [lab one knowledge share](https://www.notion.so/9eee5ea62b064025802b7fffec7715dc?pvs=21) page.
- [x]  Create a github account (if you haven't already)
- [x]  Create a repo for 3340 if you haven't already (must be public)
- [x]  Create a folder in your 3340 repo called raspberrypi (or something like that) and in that folder, a *sane* file structure — probably one that mimics the file structure on your pi for all the config and other files you will want to save.
- [x]  Somewhere that makes sense, in the above folder/file structure, create a doc named ip.md
- [x]  Plugin in the HDMI and peripherals for your Pi, then the power plug **last**.
    - [x]  **Never unplug your Pi while it is running.**
    - [x]  **Shutdown always using the GUI or by holding down the power button.**
    - [x]  **When shutting down the Pi, wait until the green light stops flashing to unplug**
        - [x]  to shutdown via ssh or CLI $`sudo shutdown -h now`
- [x]  SSH into your RPi from your laptop (you will have to be on the yale wireless network)[CLI]
    - [x]  You will need your Pi’s IP address
        - YaleSecure on computer (yale wireless on raspberry pi) works for SSH
        - Login format: ssh student334@xx.xx.xx.xx
    - [x]  All Pis: username = student334, password = cpsc334!
- [x]  Generate an SSH key for use with GitHub so you don't need to enter your creds to pull/push [see GitHub docs below] and add to your GitHub keys.
    - [x]  You can use the GitHub instructions below, but if you want the Pi to be able to automatically (without entering a password… because how can it?..) upload your [ip.md](http://ip.md) you must create the key without a passphrase.
        - After generating key, remember to “ssh-add” to load key for ssh use
        - “ssh -T git@github.com” to test connection (helper provided by github to ensure ssh key is valid, not related to normal git use)
    - Logging into git from a new machine:
        1. ssh-add ./private_key_file
        2. passphrase
        3. Testing: ssh -T git@github.com
- [x]  Write a script that:
    - [x]  copies your important config files (/boot/config.txt and anything else important to your workflow) to somewhere in your user folder structure
    - [x]  records the current IP address to a file named [ip.md](http://ip.md) to the same location as your configs.
    - [x]  The script should run *once* per login **after** networking has been achieved (see below).*

Michael’s Script:

```jsx
cp /boot/firmware/config.txt ./config
hostname -I | awk '{print $1}' > ./ip.md
git add .
git commit -m "auto: update raspberrypi files" .
git push
```

- Firmware now stored at /boot/firmware/config.txt (not /boot/config.txt)
- Config name/email for github for first-time push
- Use local directories and keep script in repo to ensure compatibility

```jsx
Script to share:
1. copy anything *.config.text to CES334-2025->raspberryPi->configs
2. hostname -I | awk '{print $1}' > ~/CES334/raspberrypi/configs/ip.md
3. git add .
4. script needs to be in the git folder

echo "Starting script..."

cp /boot/firmware/config.txt /student334/CES334-2025/raspberryPi/configs/
hostname -I | awk '{print $1}' > ~/CES334-2025/raspberryPi/configs/ip.md
git add .
git commit -m"updated config files and ip"
git push

echo "Done!"

```

- IF THERE'S TIME
- [ ]  Instead of copying your files locally, requiring manual backup, have your script copy the files to your local GitHub folder location and push them up to GitHub. [CLI]
- [ ]  Push your configs (including /boot/firmware/config.txt) to GitHub