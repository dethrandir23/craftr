#!/bin/sh
USER_HOME=$(getent passwd $SUDO_USER | cut -d: -f6)
if [ -n "$USER_HOME" ]; then
    mkdir -p "$USER_HOME/.local/share/craftr"
    cp -r /usr/local/share/craftr/* "$USER_HOME/.local/share/craftr/"
    chown -R $SUDO_USER:$SUDO_USER "$USER_HOME/.local/share/craftr"
fi
