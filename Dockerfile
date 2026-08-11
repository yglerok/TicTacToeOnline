# Builder stage
FROM ubuntu:latest AS build

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt ./

COPY third_party/ ./third_party/

COPY src/ ./src/

COPY public/ ./public

RUN cmake -S . -B build -DBUILD_TESTS=OFF && \
    cmake --build build -j$(nproc)

# Runtime stage

FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=build /app/build/TicTacToe .

COPY --from=build /app/public/ ./public

EXPOSE 8080

CMD [ "./TicTacToe" ]