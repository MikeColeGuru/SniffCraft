
# SniffCraft

SniffCraft is a cross-platform C++ proxy which let you inspect the content of each packet sent through any minecraft client and server. 

It works as a man-in-the-middle: instead of connecting directly to the server, you ask your client to connect to SniffCraft which then is connected to the server. All packets are transmitted to their original recipient and are simultaneously logged on-the-fly.

## Features and limitations

- Supported minecraft versions: all official releases from 1.12.2 to 1.17.1
- Packet logging with different levels of details (ignor packet, log packet name only, log full packet content)
- Compression is supported
- Configuration (which packet to log/ignore) can be changed without restarting
- Automatically create a session file to log information, can also optionally log to console at the same time
- Creating a [replay mod](https://github.com/ReplayMod/ReplayMod) capture of the session is also possible, see [Replay Mod section](#replay-mod) for more details

Here is an example of a captured session:
```javascript
[0:1:98:98514] [S --> C] Level Event
[0:1:100:100160] [S --> C] Level Event
[0:1:100:100160] [S --> C] Level Event
[0:1:111:111457] [C --> S] Container Click
{
  "button_num": 0,
  "carried_item": {
    "present": false
  },
  "changed_slots": {
    "46": {
      "item_count": 1,
      "item_id": 37,
      "present": true
    }
  },
  "click_type": 0,
  "container_id": 6,
  "slot_num": 46
}

[0:2:122:122761] [S --> C] Player Info
```


### /!\ Encryption is not supported /!\

As both the client and the server must share the same public key when authenticating to Mojang's session server, we can't use an intermediate decrypt/encrypt layer. I'd like to support encryption too, but I'm affraid that's simply not possible without a modification in the client, but in this case, SniffCraft is useless as you could modify it directly to log the packets. Feel free to open an issue if you have any suggestion on this matter. Use SniffCraft only with server with online-mode=false.

## Dependencies

You don't have to install any dependency to build SniffCraft, everything that is not already on your system will be automatically downloaded and locally built during the build process.

- [asio](https://think-async.com/Asio/)
- [nlohmann json](https://github.com/nlohmann/json)
- [zlib](https://github.com/madler/zlib)
- [botcraft](https://github.com/adepierre/botcraft) (actually, I'm only using protocolCraft lib, but as I haven't separated it from my Botcraft repo, everything is downloaded)

## Build and launch

To build for the latest game version:
```
git clone https://github.com/adepierre/SniffCraft.git
cd sniffcraft
mkdir build
cd build
cmake -DGAME_VERSION=latest ..
make all
```

If you are on Windows, you can replace the last four steps by launching cmake GUI and then compiling the generated .sln from Visual Studio.

Once built, you can start SniffCraft with the following command line:

```
sniffcraft listening_port server_address logconf_filepath
```

logconf_filepath is an optional json file, and can be used to filter out the packets. Examples can be found in the [conf](conf/) directory. With the default configuration, only the names of the packets are logged. When a packet is added to an ignored list, it won't appear in the logs, when it's in a detail list, its full content will be logged. Packets can be added either by id or by name (as registered in protocolCraft), but as id can vary from one version to another, using names is safer.

server_address should match the address of the server you want to connect to, with the same format as in a regular minecraft client. Custom URL with DNS SRV records are supported (like MyServer.Example.net for example). You can then connect your official minecraft client to SniffCraft as if it were a regular server. If you are running SniffCraft on the same computer as your client, something as 127.0.0.1:listening_port should work.

## Replay Mod

If ``LogToReplay`` is present and set to true in the configuration file when the session starts, all packets will also be logged in a format compatible with [replay mod](https://github.com/ReplayMod/ReplayMod). When the capture stops, you'll get a ``XXXX.mcpr`` file that can be opened by the replay mod viewer inside minecraft.

Please note that the current player will **not** appear on this capture, as the replay mod artifically adds some packets to display it.

## License

GPL v3
