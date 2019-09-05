#!/bin/bash
current_Tick=$(date +%Y%m%d%H%M)
cd ~/.klei/DoNotStarveTogether
zip -r "DST_Cluster_${current_Tick}.zip" Cluster_1
mv "DST_Cluster_${current_Tick}.zip" ~/backup/
echo -e "\n\n\nDST shell: new zip has been saved in ~/backup, tick = ${current_Tick}\n\n\n"
