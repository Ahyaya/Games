# Games
### Basically bash files or some other codes for game servers



## Deploy SSH keygen
  
  cd ~/.ssh && ssh-keygen -t rsa -f **$Key_Name**
  
  ssh-copy-id -i "${Key_Name}.pub"  "**root**@**${Host_ip}**"

## Bad local keygen could be removed with
  
  ssh-keygen -f "/home/**${usr_Name}**/.ssh/known_hosts" -R **${Host_ip}**

## Source engine Server query

  https://developer.valvesoftware.com/wiki/Server_queries

## ffmpeg rectangular filter & disable audio

  ffmpeg -i Racht.mp4 -filter:v "crop=100:80:123:456" -an Racht_out.mp4
  Rectangular starts from x=123, y=456 with width 100, height 80; -an will disable audio
