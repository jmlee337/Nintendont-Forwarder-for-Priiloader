# Nintendont Forwarder for Priiloader
Go directly from power-on to Nintendont by autobooting Nintendont through this forwarder.

Download from the [releases page](https://github.com/jmlee337/Nintendont-Forwarder-for-Priiloader/releases).

You can't autoboot Nintendont from Priiloader out of the box, unfortunately.
I'm not quite sure why this is, but it is.
So slap this forwarder in your `apps/` folder and set Priiloader to autoboot it instead.
It forwards to `apps/Nintendont/boot.dol`, which should be where you installed Nintendont.

If you do it right your SD card should have this folder structure:
```
SD
  apps
    forwarder
      boot.dol
      meta.xml
    Nintendont
      boot.dol
      icon.png
      meta.xml
```

I extracted this `.dol` out of Badablek's forwarder WAD, listed on the [GBAtemp Nintendont page](https://gbatemp.net/threads/nintendont.349258/).
