#!/bin/bash

# Установите путь к библиотекам HCNetSDK
HCNETSDK_LIB_PATH="./linux64/lib"
SOUND_PLAYER_PATH="./sound_player"

# Устанавливаем переменную окружения для поиска библиотеки
export LD_LIBRARY_PATH="$HCNETSDK_LIB_PATH:$LD_LIBRARY_PATH"

# Запускаем sound_player с переданными аргументами
exec "$SOUND_PLAYER_PATH" "$@"