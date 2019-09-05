#!/bin/bash
current_Tick=$(date +%Y%m%d%H%M)
cd ~/.klei/DoNotStarveTogether
zip -r "DST_Cluster_${current_Tick}.zip" Cluster_1
echo -e "\n\n\nDST shell: new zip has been saved, tick = ${current_Tick}\n\n\n"
