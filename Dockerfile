# Используем образ Ubuntu
FROM ubuntu:22.04

# Устанавливаем необходимые пакеты
RUN apt update && apt install -y \
    build-essential \
    g++ \
    make \
    libc6-dev \
    libasound2-dev \
    libopenal-dev \
    libssl-dev \
    nodejs \
    npm \
    && apt clean

WORKDIR /app
COPY . .

RUN npm init -y
RUN npm install express body-parser

# Устанавливаем рабочую директорию для sound_player
RUN mkdir -p /app/HCNetSDK/lib
COPY ./linux64/lib /app/HCNetSDK/lib
COPY ./include /app/HCNetSDK/include
COPY ./src /app

# Компилируем ваше приложение
RUN g++ -o sound_player /app/HSWP.cpp -I/app/HCNetSDK/include -L/app/HCNetSDK/lib -lhcnetsdk -lAudioRender -lNPQos -lSuperRender -lPlayCtrl -lHCCore -lhpr -lopenal -lssl -lcrypto -lz

# Открываем порт 3001
EXPOSE 3001

# Запуск приложения
CMD ["node", "app.js"]