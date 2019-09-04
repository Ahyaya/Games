#!/bin/bash
#update L4D2 version

./steamcmd.sh +login anonymous +app_update 222860 +quit


#execute L4DServ process

cd ~/Steam/steamapps/common/Left\ 4\ Dead\ 2\ Dedicated\ Server/
screen -dmS 'L4D2' bash -c './srcds_run -game left4dead2 -secure +hostport 27015 -condebug +exec server.cfg'

echo "L4D2 server reboot..."
echo "=============================================================="
echo -e "One screen session activated, use command:\n\t'screen -r -S L4D2'\nto rettach the session"
echo "=============================================================="
