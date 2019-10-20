# Games
### Basically bash files or some other codes for game servers



## Deploy SSH keygen

  Key_Name=_choose_a_file_name_
  
  Host_ip=_server_ip_
  
  cd ~/.ssh && ssh-keygen -t rsa -f **$Key_Name**
  
  ssh-copy-id -i "${Key_Name}.pub"  "**root**@**${Host_ip}**"

  ### Bad local keygen could be removed with
  
  Host_ip=_server_ip_to_remove_
  
  usr_Name=$(whoami)
  
  ssh-keygen -f "/home/**${usr_Name}**/.ssh/known_hosts" -R **${Host_ip}**
