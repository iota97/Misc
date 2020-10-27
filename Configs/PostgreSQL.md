# Working with local PostgreSQL and pgAdmin on ArchLinux

## Daily Usage

Start postgreSQL

```shell   
$ sudo systemctl start postgresql
```

Stop postgreSQL

```shell   
$ sudo systemctl stop postgresql
```


Starting pgAdmin 

```shell   
$ pgadmin4
```

<hr>

## Initital Setup

### Install packages

```shell   
$ sudo pacman -S postgresql pgadmin4
```


### Setup postgresql

PostgreSQL automatically creates a new system user called **postgres**. Most of the PostgreSQL commands must be executed by this user. Commands that should be run as the postgres user are prefixed by [postgres]$ in this article. You can switch to this user with:

```shell   
$ sudo -iu postgres
```

After installing the package you have to init the database cluster (execute as postgres user):

```shell   
[postgres]$ initdb -D /var/lib/postgres/data
```

You can now exit to your local user by:

```shell   
$ exit
```

Now start the postgreSQL Database with:

```shell   
$ sudo systemctl start postgresql
```

If you want postgreSQL to start automatically on system boot also type: 

```shell   
$ sudo systemctl enable postgresql
```

Add a new database user (execute as postgres):   

```shell 
$ sudo -iu postgres  
[postgres]$ createuser --interactive -P
```
> The `-P` flag creates a user with a password


### Usefull Links:   
https://wiki.archlinux.org/index.php/PostgreSQL

<hr>

## Using pgAdmin

Run pgAdmin by selecting the apllication from your launcher or type:  

```shell   
$ pgadmin4
```


Make sure the PostgreSQL Server is running. You can see the server's log messages (including the port number, which will be neccessary later on) in systemd's log. Access the log with:   

```shell   
$ journalctl -e
```

> Exit the journal by pressing `q`

<br>

### Add your PostgreSQL server to pgAdmin

Now add the PostgreSQL Server in the pgAdmin Control Panel by clicking on "Add New Server". 

Give your server configuration a name.

Under the "Connection" Tab set the following properties

- Use `127.0.0.1` (localhost) as Hostname
- The port, your server is listening on. (By default this is 5432)
- Use `postgres` as Maintenance database
- The username and password of the PostgeSQL user you created so far

If this is a lucky day everything should work now!

This was made by superjojo140, I just made a copy as backup
