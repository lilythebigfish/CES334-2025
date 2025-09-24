**Vibration** is a generative art program co-created by Alicia Deng and Lily Lin. It generates a from made of a grid of circles with dynamically changing colors and shape that grows randomly according to a gaussian distribution. This creates an impression of rhythm, thus the name "vibration". This program runs automatically and indefinitely at reboot in kiosk (full screen), and adapts to every screen size.

To run this program, download this folder, but move "p5starter.service" into your systemd systems folder (for example, /etc/systemd/system). Make sure you have Firefox installed as a web browser.

In your terminal, run:  
>sudo systemctl enable p5starter.service  
>sudo systemctl daemon-reload  

Make sure the program works without errors before rebooting:  
>sudo systemctl start p5starter.service  
>systemctl status p5starter.service  

Try rebooting:  
>sudo reboot  

If the program doesn't start automatically, check whether the program is properly linked into the boot chain.   
>systemd-analyze critical-chain p5starter.service  

If p5starter.service is not included in this chain, manually link the service file to graphical-target:  
>sudo ln -sf /etc/systemd/system/p5starter.service /etc/systemd/system/graphical.target.wants/p5starter.service  
>sudo systemctl daemon-reload  

Then, reboot. The program should now run automatically at reboot, after the graphical interfaces is loaded and network is connected. 

[Here is video of the program running automatically at reboot.](https://vimeo.com/1121262707)
