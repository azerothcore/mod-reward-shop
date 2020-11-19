# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
## Ingame Reward System

This is a module for [AzerothCore](http://www.azerothcore.org)

- Latest build status with azerothcore: [![Build Status](https://github.com/azerothcore/mod-reward-shop/workflows/core-build/badge.svg?branch=master&event=push)](https://github.com/azerothcore/mod-reward-shop)

Current features:

-** This Module Makes a NPC do database check for a code that can redeemed ingame, The NPC checks which action is called in the Database to apply the correct items/features

Database Actions:

1 = item
2 = gold
3 = name change
4 = faction change
5 = race change

Upcoming features:
###1) If a person feels like they could make a website to work with this module it will be great i will merge into the source and also give credit.


## Requirements

Reward Shop module currently requires:

AzerothCore v1.0.1+

## How to install

###1) Simply place the module under the `modules` folder of your AzerothCore source folder.

###2) Input the SQL file to the character database.

###3) Re-run cmake and launch a clean build of AzerothCore

**That's it.**

### (Optional) Edit module configuration

If you need to change the module configuration, go to your server configuration folder (e.g. **etc**), copy `reward_shop.conf.dist` to `reward_shop.conf` and edit it as you prefer.

# Show your appreciation
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SBJFTAJKUNEXC)







