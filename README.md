# Nintendont Forwarder for Priiloader
Go directly from power-on to Nintendont (or Melee-focused variants) by autobooting through this forwarder.

Download from the [releases page](https://github.com/jmlee337/Nintendont-Forwarder-for-Priiloader/releases/latest).

You can't autoboot Nintendont from Priiloader out of the box, unfortunately.
So slap these forwarders in your `apps/` folder and set Priiloader to autoboot them instead.
It forwards to (CASE SENSITIVE):

```
apps/Nintendont/boot.dol
apps/Nintendont Slippi/boot.dol
apps/NintendontForMelee/boot.dol
``` 

which should be where you installed Nintendont and variants.

If you do it right your SD card should have this folder structure:
```
SD
  apps
    Nintendont
      boot.dol
      icon.png
      meta.xml
    Nintendont Forwarder
      boot.dol
      meta.xml
    Nintendont Slippi
      boot.dol
      icon.png
      meta.xml
    Nintendont Slippi Forwarder
      boot.dol
      meta.xml
    NintendontForMelee
      boot.dol
      icon.png
      meta.xml
    NintendontForMelee Forwarder
      boot.dol
      meta.xml
```

I grabbed the source from a [post by FIX94 on GBAtemp](https://gbatemp.net/threads/forwarder-with-meta-xml-support.273165/).
Looks like this code was even handed down a number of times before that.


## Alternatively
You can autoboot Nintendont and any variant by using the correct settings in Priiloader.
HOWEVER, you will have to re-'install' every time you update your Nintendont.
Since this is somewhat error-prone, **I don't recommend this method**.
If you use the forwarder you'll never have to think about it again.

```
Use System Menu IOS:      off
IOS to use for SM:        58
```

![priiloader settings, as above](https://i.imgur.com/OKrlCio.jpg)
