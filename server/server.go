package main

import (
	"net"
)

func main() {
	// создание сервера
	dstream, err := net.Listen("tcp", ":5893")
	if err != nil {
		return
	}
	defer dstream.Close()
	// обработка новых подключений в цикле
	for {
		// принять новое подключение
		con, err := dstream.Accept()
		if err != nil {
			return
		}
		//обработка подключения в новом виртуальном потоке
		go handle(con)
	}
}

func handle(con net.Conn) {
	defer con.Close()
	// обработка запросов в цикле
	for {
		buf := make([]byte, 32)
		rlen, err := con.Read(buf) // принять запрос

		//проверка на наличие ошибок и верификация подключения
		if err != nil {
			return
		}
		// con.Write([]byte(con.RemoteAddr().String()))
		if string(buf[:rlen]) == "connection:check" {
			con.Write([]byte("connection:confirmed"))
			continue
		}
		con.Write(buf[:rlen])
	}
}
