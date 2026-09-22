FROM debian:bookworm-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
        mingw-w64 make \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /out
WORKDIR /src
COPY . .

RUN x86_64-w64-mingw32-g++ -std=c++17 -O2 -static -municode \
    L1_2/L1_2.cpp -o /out/L1_2.exe

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=build /out/L1_2.exe .
CMD ["cp", "/app/L1_2.exe", "/output/L1_2.exe"]
