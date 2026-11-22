
FROM gcc:14

WORKDIR /app

COPY . .

RUN g++ -std=c++17 -O2 server/main.cpp -o sandhi_server -Iserver -pthread

EXPOSE 8080

CMD ["./sandhi_server"]
