#!/bin/bash

cd ~/DST_server/bin

screen -dmS 'DST_srv_Master' bash -c './dontstarve_dedicated_server_nullrenderer -console -cluster Cluster_1 -monitor_parent_process $$ -shard Master'

screen -dmS 'DST_srv_Caves' bash -c './dontstarve_dedicated_server_nullrenderer -console -cluster Cluster_1 -monitor_parent_process $$ -shard Caves'

echo "Don't Starve Together's world initiating"
echo "=============================================================="
echo -e "Two screen sessions are activated, use command:\n\t'screen -r -S DST_srv_Master'\nand\n\t'screen -r -S DST_srv_Caves'\nto rettach the sessions"
echo "=============================================================="

