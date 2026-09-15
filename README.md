## Skyerip

#### A (Work in Progress) tool made out of frustration with one file.

I haven't found a decent [StaxRip](https://github.com/staxrip/staxrip) replacement for linux yet, so I'm making my own. That's the entire premise. Just the functionality I used on windows, just without the crashes under wine, since everything (at least in theory) will be native code.

#### Functions implemented so far:
- Asking for file name
- Asking for video encoder preset name
- Log file generation spamming your /var/tmp folder. Have fun :3
- Asking for output file name
- Checking installed python3 for meeting minimum version requirements of [VapourSynth](https://www.vapoursynth.com/)
- The sorriest excuse of a makefile replacement
- Parsers for custom encoder preset and definition formats.
- A bunch of notes on how everything should be implemented in the near future.

### Functions NOT implemented so far:
- Encoding anything (TECHNICALLY, IT NOW PRODUCES VALID FRAMESERVER-PIPED-INTO-ENCODER COMMAND STRINGS, x265 DOESN'T COMPLAIN.)
- A GUI or at least a CLI that doesn't suck
- Dynamic config path loading
- Any Audio, Subtitle and MUXING SUPPORT
- Any (optional) deinterlacing support
- queue
- HDR10/HDR10+ support (and DV)
- Any real optimizations
- Input cropping and trimming

## Licensing info
This program is licensed under the BSD 3-Clause license, (see LICENSE for terms, it's short).

Third party libraries:
This program relies on third party libraries, listed below in form of links to their licenses. Copies of said licenses are also stored in "ThirdParty",
[MediaInfoLib](https://mediaarea.net/en/MediaInfo/License)



## README.MD updated 2026-09-15 20:31 Central Europe Standard Time.
