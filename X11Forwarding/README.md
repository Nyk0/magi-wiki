# X11Forwarding on Magi

The purpose of X11Forwarding is to use the local windows manager (aka Windows Manager or X11 Server or Xorg) to display remote graphical services. In a nutshell you see on your screen applications that are executed on remote ressources. This can be helpful if you want to have graphic rendering of data located on magi on your local desktop. X11Forwarding is native on Linux and MacOS but requires an installation on Windows.

## Installing X11 Server implementation

First, you must install an X11 server on your windows desktop. You can use Xming. You can download it from here, or use winget from an administrator cmd / powershell:

```console
PS C:\Users\Nyko> winget install Xming
```

Once you have Xming installed, search for Xming in the Start Menu and run it. The process will run in the background and will show a system tray icon.

> [!NOTE]
> Xming does not start with your system automatically. You will need to start Xming each time you start your computer to use X11 forwarding.

## Using X11 Server

Before connecting to the remote host, you must specify an environment variable to tell your SSH client that remote applications must use graphical services (Xming) of your local desktop. This can be achieved in the cmd / powershell session before SSH connection:

```console
PS C:\Users\Nyko> setx DISPLAY localhost:0
```

Then you can launch your SSH connection with the addition on -Y parameter:

```console
PS C:\Users\Nyko> ssh -Y -p 2822 nicolas.greneche@magi.univ-paris13.fr
```

Once you’re connected on Magi this way, each application you launch will use your local display (but runs on Magi resources).
