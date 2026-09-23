FROM debian:bookworm-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
        mingw-w64 make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN mkdir -p /out && \
    x86_64-w64-mingw32-g++ -std=c++17 -O2 -static -mconsole -municode \
        L1_2/L1_2.cpp -o /out/L1_2.exe

FROM scratch
COPY --from=build /out/L1_2.exe /L1_2.exe
