#!/usr/bin/env bash

REWARD_SHOP_PATH_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

DB_CHARACTERS_CUSTOM_PATHS+=(
	$REWARD_SHOP_PATH_ROOT"/sql/chars-base/"
)

#
# UPDATES
#

DB_CHARACTERS_UPDATE_PATHS+=(
	$REWARD_SHOP_PATH_ROOT"/sql/chars-updates/"
)
