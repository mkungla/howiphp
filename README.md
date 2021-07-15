# howiphp

**Ancient ultimate PHP version manager**  
**NB!** It is only tested on Fedora!

```
$ ./howiphp

Run checks on libhowi-php installation:
        check                         Check system dependencies.
   -p   --ping                        Check mirrors and repositories by pinging them..
   -d   --download                    Determine fastest mirrors. (-p flag is not needed)

Run initial or post configuration:
        configure                     Run interactive configuration.

Install new php version:
        install        master         Build PHP from git master
        install        <version>      Build specified php version.

Remove php versions installed with libhowi-php:
        remove         <version>      Remove specified php version

Switching between installed php versions:
        use            <version>      Switch to php version specified.

Listing php versions:
        list                          List all installed and available php releases and tags!
   -i   --installed                   List only installed php versions
   -a   --available                   List only available not installed php versions
   -r   --releases                    List only available not installed php versions

Optional arguments:
   -h   --help                        This help
   -n   --no-colors                   Show non colored output
   -v   --version                     Version information


```

## Getting Started

### Prerequisites

```
sudo dnf install \
  autoconf \
  automake \
  boost-devel \
  libgit2-devel \
  oniguruma-devel
```

### Prepare

```
git clone git@github.com:mkungla/howiphp.git
cd howiphp
git submodule init
git submodule update

```

### Build

```
chmod +x autogen.sh
./autogen.sh
cd build && ../configure
make
```

### Initial Setup

- **Export or modify \$PATH environment variable in `~/.bashrc`**  
  e.g. `export PATH=$HOME/.howiphp/bin:$PATH;`


1. Configuration will automatically detect for you fastest mirrors for your PHP updates. Initial mirror check may take few minutes depending on your network speed.

```
$ ./howiphp configure
[ >> ]: Starting configuration...                                                                                                                                                [ INFO  ]
   Would you like to erase your current configuration and reset defaults? [y/n]
[you]$ y
```

2. PHP buildconf for builds are dset by default, but you can customize it

```
[ >> ]: PHP buildconf parameters are already configured!                                                                                     [  OK   ]
   Configure php buildconf? [y/n]
[you]$ n
   Please give ./buildconf flags [default = --force]
[you]$    Press ENTER to keep default or previously set value
...
< prints default build config >
...
Do you want to change default php ./configure flags? [y/n]
[you]$ n
```

3. Setup your initial PHP versions

```
[ >> ]: Checking available PHP releases done!                                                                                                [  OK   ]
[161 ]: Checking php-src Git repository!                                                                                                     [ INFO  ]
[ >> ]: Git repo is not checked out! Run configure and in step checkout git master to build php from git TAGS.                               [ INFO  ]
   Check out php master to get possibility to build php from git TAGS? [y/n]
[you]$ n
```

4. Run `howiphp check`

- **ERRORS**, install dependencies if here are errors.
- **WARNINGS**, Missing dependency warning will just let you know which dependencies you need if you decide set some of the build flags and are not required by default.

### Install

run `sudo make install` or copy `./build/howiphp` to somewhere on your path


## Usage

### Check installed versions
```
$ howiphp list --installed
[ >> ]: Total installed 0 and total available 266
```

### Install php versions

1. e.g install 8.0.8

```
$ howiphp install 8.0.8
[ >> ]: If ./configure finished with errors then type (n) and check your dependencies and configuration

Do you want to run (make) command now? [y/n]
[you]$ y

...

[ >> ]: Running (make test) can take long time! you can skip this step!

Do you want to run (make test)? [y/n]
[you]$ n

Do you want to run (make install) now? [y/n]
[you]$ y

```

2. then install another version

```
$ howiphp install 7.4.21
[ >> ]: If ./configure finished with errors then type (n) and check your dependencies and configuration

Do you want to run (make) command now? [y/n]
[you]$ y

...

[ >> ]: Running (make test) can take long time! you can skip this step!

Do you want to run (make test)? [y/n]
[you]$ n

Do you want to run (make install) now? [y/n]
[you]$ y

```

### Switch between PHP versions

```
$ howiphp list --installed
7.4.21                 release 
8.0.8                  release

$ howiphp use 8.0.8
[ >> ]: Switched to php versions: 8.0.8

$ php -v
PHP 8.0.8 (cli) ...

$ howiphp use 7.4.21
[ >> ]: Switched to php versions: 7.4.21

$ php -v
PHP 7.4.21 (cli) ...
```
