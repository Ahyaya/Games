# Games
### Basically bash files or some other codes for game servers



## Deploy SSH keygen

  Key_Name=\<choose_a_file_name\>
  
  Host_ip=\<server_ip\>
  
  cd ~/.ssh && ssh-keygen -t rsa -f $Key_Name
  
  ssh-copy-id -i "${Key_Name}.pub"  "root@${Host_ip}"

  ### Bad local keygen could be removed with
  
  usr_Name=$(whoami)
  
  ssh-keygen -f "/home/${usr_Name}/.ssh/known_hosts" -R ${Host_ip}
