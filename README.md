EDIT: Dreamweave-Classic is now a dead project, as of 07/17/25. Due to upstream changes in GitHub's build environment, it is now no longer physically possible to compile DW-Classic without making edits to the source code, which is pointless since these are not problems OpenMW proper is affected by.

For further engine developments, refer to the [Dreamweave-MP organization on GitLab](https://gitlab.com/dreamweave-mp). 

Dreamweave
======

Copyright (c) 2008-2015, OpenMW Team
Copyright (c) 2016-2022, David Cernat & Stanislav Zhukov
Copyright (c) 2023-, Dreamweave Community

Dreamweave is a project adding multiplayer functionality to [OpenMW](https://github.com/OpenMW/openmw), an open-source game engine that supports playing "The Elder Scrolls III: Morrowind" by Bethesda Softworks. Its source code is originally based on the TES3MP fork of OpenMW, with many modifications. Dreamweave's goal is to extend and refactor much of the core functionality of TES3MP itself. Please support the original project(s).

* Dreamweave version: 0.1.0
* TES3MP version: 0.8.1
* OpenMW version: 0.47.0
* License: GPLv3 with additional allowed terms (see [LICENSE](https://github.com/TES3MP/TES3MP/blob/master/LICENSE) for more information)

Font Licenses:
* DejaVuLGCSansMono.ttf: custom (see [files/mygui/DejaVuFontLicense.txt](https://github.com/TES3MP/TES3MP/blob/master/files/mygui/DejaVuFontLicense.txt) for more information)

Project status
--------------

Being based on TES3MP's latest version, most features (and caveats) of gameplay apply to Dreamweave, with some exceptions to be documented. See the original, unmodified project status below:

[Version changelog](https://github.com/TES3MP/TES3MP/blob/master/tes3mp-changelog.md)

As of version 0.8.1, TES3MP is fully playable, providing very extensive player, NPC, world and quest synchronization, as well as state saving and loading, all of which are highly customizable via [serverside Lua scripts](https://github.com/TES3MP/CoreScripts).

Remaining gameplay problems mostly relate to AI and the fact that clientside script variables need to be placed on a synchronization whitelist to avoid packet spam.

TES3MP now also has a [VR branch](https://github.com/TES3MP/TES3MP/tree/0.8.1-vr) that combines its code with that of Mads Buvik Sandvei's [OpenMW VR](https://gitlab.com/madsbuvi/openmw).

Donations
---------------

You can benefit the project by donating on Patreon to the OpenMW team:
[CapoStrophic](https://www.patreon.com/capostrophic)
[AnyOldName3](https://www.patreon.com/AnyOldName3)
[David Cernat](https://www.patreon.com/davidcernat)
[Psi29a](https://www.patreon.com/psi29a)
[Foal](https://ko-fi.com/madsbuvi)
[Wazabear](https://www.patreon.com/wazabear)

Contributing
---------------

Helping us with documentation, bug hunting and video showcases is always greatly appreciated.

For code contributions, it's best to start out with modestly sized fixes and features and work your way up. There are so many different possible implementations of more major features – many of which would cause undesirable code or vision conflicts with OpenMW – that those should be talked over in advance with the existing developers before effort is spent on them.

Feel free to contact the [team members](https://github.com/Dreamweave-mp/dreamweave/blob/master/dreamweave-credits.md) for any questions you might have.

Getting started
---------------

* [Quickstart guide](https://github.com/TES3MP/TES3MP/wiki/Quickstart-guide)
* [Steam group](https://steamcommunity.com/groups/mwmulti) and its [detailed FAQ](https://steamcommunity.com/groups/mwmulti/discussions/1/353916184342480541/)
* [TES3MP section on OpenMW forums](https://forum.openmw.org/viewforum.php?f=45)
* [Discord server](https://discord.gg/ECJk293)
* [Subreddit](https://www.reddit.com/r/tes3mp)
* [Known issues and bug reports](https://github.com/TES3MP/TES3MP/issues)
