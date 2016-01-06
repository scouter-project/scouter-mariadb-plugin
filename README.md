# scouter-mariadb-plugin
MariaDB Plugin for Scouter

 

Mariadb plugin for scouter
==============================================
This is alpha version, so we recommand to use test Environments.

Current version has been tested in Mariadb version 10.0.20 and 10.0.21.
We will support for other versions soon.


**Feature**

1. Plugin for mariadb monitoring on scouter apm tool env.
2. No SQL Method, it use API Function in MariaDB source
3. So Minimun resource use ( 0.5% lower cpu, 1% lower memory)
4. Easy install & uninstall ( it's mariadb plugin)

## Preinstallation Task

### Software requirements

Verify that the following software is installed on your system:

   * git
   * gunzip
   * GNU tar
   * gcc 2.95.2 or later
   * g++
   * GNU make 3.75 or later
   * bison (2.0 for MariaDB 5.5)
   * libncurses
   * zlib-dev
   * cmake
   * jemalloc 

On Linux you can get those programs with your package manager. An easy way to install the needed programs on Linux is to run one of the following commands(depending on your Linux distribution)
```sh
  sudo apt-get build-dep mysql-server # for Debian and Ubuntu-based distributions
  sudo yum-builddep mysql # for Fedora 17-18 (part of the yum-utils package)
  sudo yum-builddep mariadb # for Fedora 19+ (part of the yum-utils package)
  sudo zypper si -d mysql-community-server # for OpenSUSE   
```

### System requirements
Scouter-mariadb-plugin sends performance data to Scouter server via socket. Verify that network ports open.
   * 6100 / tcp, udp  (source : mariadb server, target : scouter server)




## Build
    
   1. Download mariadb source from maria site (https://downloads.mariadb.org/ or mirror site 
        ftp://ftp.kaist.ac.kr/mariadb/mariadb-10.0.21/source/mariadb-10.0.21.tar.gz)
   2. unzip source file
   
   ```sh
   tar zxvf mariadb-xxx.xxx.xxx.tar.gz  (xxx means mariadb version)
   ```
   3. download scouter-maradb-plugin source. [[mariadb-source-dir]] is a mariadb source directory.
    
   ```sh
    cd <<mariadb-source-dir>>/plugin                            
    git clone https://github.com/scouter-project/scouter-mariadb-plugin
   ```
   4. Run **cmake** command in the mariadb source directory.
   
   ```sh
   cd <<maraiadb-soucrce-dir>>
   cmake -DBUILD_CONFIG=mysql_release -DWITH_JEMALLOC=no . 
   ```
   5. Run **make** command in the scouter-mariadb-plugin source directory.
   
   ```sh
   cd <<mariadb-source-dir>>/plugin/scouter-mariadb-plugin
   make 
   ```
   If make runs successfully, you'll find libspotter.so file in the \<<mariadb-source-dir\>>/plugin/scouter-mariadb-plugin/src.
 
  
 
## Install
   1. Copy scouter-mariadb-plugin library to mariadb plugin directory. \<<mariadb-install-dir\>> is a mariadb installation directory.
   
   ```sh
        cd <<mariadb-source-dir>>/plugin/scouter-mariadb-plugin/src
        cp libspotter.so <<mariadb-install-dir>>/lib/plugin
   ```
      
   2. A plugin located in a plugin library file can be loaded at runtime with the INSTALL PLUGIN statement. 
   
   ```sh
     INSTALL PLUGIN spotter SONAME 'libspotter.so';
   ```
   3. Setting system global variable for scouter server ip address
   
   ```sh
   show variables like 'spotter%'
   set global spotter_server_ip="xxx.xxx.xxx.xxx";
   ```

## Uninstall plugin 
To uninstall scouter-mariadb-plugin, use UNINSTALL PLUGIN command.

```sh
   uninstall plugin spotter;
```
    
## License

Maridb plugin for scouter is specifically available only under version 2 of the GNU
General Public License (GPLv2)
