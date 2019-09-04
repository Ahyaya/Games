# Games
### Basically bash files or some other codes for game servers

## Deploy SSH keygen

  Key_Name=<choose a file name>
  
  Host_ip=<server ip>
  
  cd ~/.ssh && ssh-keygen -t rsa -f $Key_Name
  
  ssh-copy-id -i "${Key_Name}.pub"  "root@${Host_ip}"
