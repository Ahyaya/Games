sudo yum clean all
sudo yum makecache
sudo yum upgrade

sudo yum -y install vim screen gcc unzip
sudo yum -y install ia32-libs.i686 glibc.i686 libstdc++.i686 libcurl.i686 --setopt=protected_multilib=false
ln -s /usr/lib/libcurl.so.4 /usr/lib/libcurl-gnutls.so.4

cd ~
wget https://github.com/Ahyaya/Games/raw/master/left4dead2.zip
unzip -o left4dead2.zip

mkdir ~/steamcmd
cd ~/steamcmd
wget https://steamcdn-a.akamaihd.net/client/installer/steamcmd_linux.tar.gz
tar -xvzf steamcmd_linux.tar.gz

./steamcmd.sh +login anonymous +app_update 222860 +quit
