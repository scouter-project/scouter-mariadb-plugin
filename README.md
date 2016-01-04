# scouter-mariadb-plugin
MariaDB Plugin for Scouter


Mariadb plugin for scouter
==============================================
Current version has been tested in Mariadb version 10.0.20 and 10.0.21.
We will support for other versions soon.

**preInstallation Task**
**Build**
    
    1. Download mariadb source from maria site (https://downloads.mariadb.org/ or mirror site 
        ftp://ftp.kaist.ac.kr/mariadb/mariadb-10.0.21/source/mariadb-10.0.21.tar.gz)
    2. tar zxvf mariadb-10.0.21.tar.gz 
    3. cd mariadb-10.0.21/plugin
    4. download scouter-maradb-plugin source (git clone https://github.com/scouter-project/scouter-mariadb-plugin)
    5. cd .. ( mariadb root path)
    6. in Redhat or CentOS box,install following basic build package
        git
        gunzip
        GNU tar
        gcc 2.95.2 or later
        g++
        GNU make 3.75 or later
        bison (2.0 for MariaDB 5.5)
        libncurses
        zlib-dev
        cmake
        jemalloc 
     CentOS,Redhat> yum install git gunzip gcc gcc-g++ make bison libcurses zlib-dev cmake jemalloc 

     7. in maria source root 
       >cmake -DBUILD_CONFIG=mysql_release -DWITH_JEMALLOC=no . 

     8. cd mariadb-10.0.21/plugin
     9. make 
        will make libspotter.so 

     


**Install:**

  * 1. cp libspotter.so  in maria server install directory 
    ( ex /opt/mariadb-10.0.21-linux-x86_64/lib/plugin)
    2. vi /etc/my.cnf
     and add 
     spotter_server_ip="10.3.0.102"    // your scouter server ip 
     spotter_server_tcp_port ="6100"   // default 
    3. in mysql daemon
     > INSTALL PLUGIN spotter SONAME 'libspotter.so';
     > show plugins;                   // will display libspotter.so
     if uninstall 
     >UNINSTALL PLUGIN spotter;
    4. restart mariadb service 



**License**

Maridb plugin for scouter is specifically available only under version 2 of the GNU
General Public License (GPLv2)









