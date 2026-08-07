## Skyerip

#### A (Work in Progress) tool made out of frustration with one file.

I haven't found a decent [StaxRip](https://github.com/staxrip/staxrip) replacement for linux yet, so I'm making my own. That's the entire premise. Just the functionality I used on windows, just without the crashes under wine, since everything (at least in theory) will be native code.

#### Functions implemented so far:
- Asking for file name
- Asking for video encoder preset name
- Log file (lousy implementation, ngl)
- Checking installed python3 for meeting minimum version requirements of [VapourSynth](https://www.vapoursynth.com/)
- The sorriest excuse of a makefile replacement
- Almost complete parser for custom encoder preset and definition formats.
- A bunch of notes on how everything should be implemented in the near future.

### Functions NOT implemented so far:
- Encoding anything (YET, so close yet so far)
- A GUI or at least a CLI that doesn't suck
- Directory listing
- Log file rotation (yes, really, it won't do another log until you delete logfile.log. HAVE FUN. 2026-08-07 update: still no work on that front)
- Config file paths


## README.MD updated 2026-08-07 22:57 Central Europe Standard Time.
