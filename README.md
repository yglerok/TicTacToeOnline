# Tic-Tac-Toe Online
 
A real-time multiplayer tic-tac-toe game, built as a hands-on learning project spanning C++ backend development, vanilla JavaScript, PostgreSQL, and Docker.
 
## Overview
 
Two players are matched into a game session over WebSocket. The server is authoritative: it owns the board state, validates every move, detects wins/draws, and broadcasts the resulting state to both clients. The browser client is a thin renderer — it sends move attempts and redraws only in response to what the server reports.
 
## Tech Stack
 
- **Backend:** C++23, cpp-httplib (HTTP server for the static frontend + WebSocket handling for real-time events)
- **Serialization:** nlohmann/json for WebSocket message payloads
- **Database:** PostgreSQL, accessed via libpqxx
- **Build system:** CMake, with the core game/connection/database logic split into a `TicTacToeCore` library so it can be linked by both the server executable and the test suite
- **Testing:** GoogleTest (fetched via CMake's `FetchContent`), run as a separate CI step outside the Docker build
- **Frontend:** Vanilla HTML/CSS/JS — no framework. `EventEmitter` + `Connection` handle the WebSocket transport; `BoardView` and `InfoPanelView` render state; `App` wires everything together
- **Infrastructure:** Docker (multi-stage build), Docker Compose, GitHub Actions CI
 
## Getting Started
 
### Local build (CMake)
 
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./TicTacToe
```
 
The server listens on `http://0.0.0.0:8080` and serves the frontend from `./public`, with the WebSocket endpoint at `/ws`.
 
### Docker
 
```bash
docker compose up --build
```
 
Compose brings up the app container alongside a PostgreSQL container with a healthcheck, and initializes the schema automatically from `/docker-entrypoint-initdb.d/` on first run.
 
### Running tests
 
```bash
cd build
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)
ctest
```
 
`BUILD_TESTS` defaults to `ON` locally but is turned `OFF` for the Docker image build, so test dependencies never end up in the runtime container.
 
## Roadmap
 
1. ~~Game logic~~ — board, win/draw detection, turn management
2. ~~HTTP server~~ — serves the static game page
3. ~~WebSocket matchmaking~~ — pairs players into a room, assigns X/O
4. ~~Frontend~~ — board rendering, click-to-move, live status updates
5. Rooms and lobby — shareable room codes, public/private toggle, symbol picker
6. Reconnection — server-issued token, `resume` / `resumed` / `resumeFailed` protocol
7. Persistence — save finished games, leaderboard and per-player stats
8. Optional accounts — guest play by default, with an upgrade path to a real account
---
 
# Tic-Tac-Toe Online (крестики-нолики)
 
Приложение для игры в крестики-нолики в реальном времени — учебный проект, на котором отрабатываются backend на C++, vanilla JavaScript, PostgreSQL и Docker.
 
## Обзор
 
Два игрока объединяются в игровую сессию через WebSocket. Сервер — единственный источник истины: он хранит состояние поля, проверяет каждый ход, определяет победу/ничью и рассылает актуальное состояние обоим клиентам. Браузерный клиент отвечает лишь за рендерер: он отправляет попытки хода и перерисовывает поле только в ответ на сообщения сервера.
 
## Стэк технологий
 
- **Backend:** C++23, cpp-httplib (HTTP-сервер для статической страницы игры + обработка WebSocket-событий в реальном времени)
- **Сериализация:** nlohmann/json для тела WebSocket-сообщений
- **База данных:** PostgreSQL, доступ через libpqxx
- **Система сборки:** CMake; основная логика (игра, соединения, база данных) вынесена в библиотеку `TicTacToeCore`, которую линкуют и в серверный исполняемый файл, и в тесты
- **Тестирование:** GoogleTest (подключается через `FetchContent` в CMake), запускается отдельным шагом CI вне сборки Docker-образа
- **Frontend:** чистые HTML/CSS/JS без фреймворков. `EventEmitter` и `Connection` отвечают за транспорт WebSocket; `BoardView` и `InfoPanelView` рендерят состояние; `App` связывает всё воедино
- **Инфраструктура:** Docker (многоэтапная сборка), Docker Compose, GitHub Actions CI
 
## Быстрый старт
 
### Локальная сборка (CMake)
 
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./TicTacToe
```
 
Сервер слушает `http://0.0.0.0:8080`, раздаёт фронтенд из `./public`, WebSocket-эндпоинт — `/ws`.
 
### Docker
 
```bash
docker compose up --build
```
 
Compose поднимает контейнер приложения вместе с контейнером PostgreSQL с healthcheck'ом и автоматически инициализирует схему из `/docker-entrypoint-initdb.d/` при первом запуске.
 
### Запуск тестов
 
```bash
cd build
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)
ctest
```
 
По умолчанию `BUILD_TESTS` включён локально, но выключается при сборке Docker-образа, чтобы зависимости тестов не попадали в рантайм-контейнер.

## Планы развития
 
1. ~~Игровая логика~~ — поле, проверка победы/ничьей, управление очередью ходов
2. ~~HTTP-сервер~~ — раздача статической страницы игры
3. ~~WebSocket-матчмейкинг~~ — объединение игроков в комнату, назначение X/O
4. ~~Frontend~~ — отрисовка поля, ходы по клику, live-обновление статуса
5. Комнаты и лобби — код комнаты для присоединения, переключатель публичная/приватная, выбор символа
6. Переподключение — токен от сервера, протокол `resume` / `resumed` / `resumeFailed`
7. Персистентность — сохранение завершённых игр, таблица лидеров и статистика игроков
8. Опциональная авторизация — по умолчанию игра гостем, с возможностью привязать аккаунт