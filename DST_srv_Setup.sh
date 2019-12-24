#!/bin/bash
#You may firstly install the related libs from wget or apt/yum command as follow:
#===============================================================================================
#Ubuntu
sudo apt-get install libstdc++6:i386 libgcc1:i386 libcurl4-gnutls-dev:i386
#-----------------------------------------------------------------------------------------------
#CentOS
#yum -y install glibc.i686 libstdc++.i686 libcurl.i686 --setopt=protected_multilib=false
#ln -s /usr/lib/libcurl.so.4 /usr/lib/libcurl-gnutls.so.4
#===============================================================================================



#Install steamCMD
#===============================================================================================
mkdir ~/steamcmd
cd ~/steamcmd
wget https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz
tar -xvzf steamcmd_linux.tar.gz
#===============================================================================================



#With steamcmd installed, use the following command to install DSTsrv
#===============================================================================================
./steamcmd.sh +login anonymous +force_install_dir ~/DST_server +app_update 343050 validate +quit
#Download the save and cfg files from github or manually upload
mkdir -p ~/.klei/DoNotStarveTogether
mkdir -p ~/.klei/Agreements/DoNotStarveTogether
cd ~/.klei/DoNotStarveTogether
wget https://github.com/Ahyaya/publicTest/raw/master/Cluster_1.zip
unzip -o Cluster_1.zip
#===============================================================================================

mkdir ~/backup
cp ~/.klei/DoNotStarveTogether/Cluster_1.zip ./

#Setup mods
#===============================================================================================
echo -e 'ServerModSetup("1378549454")\nServerModSetup("356398534")\nServerModSetup("362175979")\nServerModSetup("374550642")\nServerModSetup("375850593")\nServerModSetup("378160973")\nServerModSetup("382177939")\nServerModSetup("458940297")\nServerModSetup("466732225")
ServerModSetup("666155465")\nServerModSetup("672208231")\nServerModSetup("721491336")\nServerModSetup("758532836")\nServerModSetup("770901818")\nServerModSetup("818739975")\nServerModSetup("972139614")' >> ~/DST_server/mods/dedicated_server_mods_setup.lua
#===============================================================================================

cd ~
echo -e "All necessary files for Don't Starve Together server deployed!\n\nYou may try ./DST_srv_Launcher.sh to launch it."

#When finished, you may choose a way to start the server
#===============================================================================================
#Option-1 utilize the backstage command
#cd ~/DST_server/bin
#run_shared=(./dontstarve_dedicated_server_nullrenderer)
#run_shared+=(-console)
#run_shared+=(-cluster Cluster_1)
#run_shared+=(-monitor_parent_process $$)
#"${run_shared[@]}" -shard Caves  | sed 's/^/Caves:  /' &
#"${run_shared[@]}" -shard Master | sed 's/^/Master: /'
#-----------------------------------------------------------------------------------------------
#Option-2 use screen command to open 2 shell separately,
#shell 1: 
#cd ~/DST_server/bin && ./dontstarve_dedicated_server_nullrenderer -console -cluster Cluster_1 -monitor_parent_process $$ -shard Master
#shell 2: 
#cd ~/DST_server/bin && ./dontstarve_dedicated_server_nullrenderer -console -cluster Cluster_1 -monitor_parent_process $$ -shard Caves
#===============================================================================================
