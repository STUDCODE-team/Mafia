package main

import (
	// ch "./channels"
	"net"
	"strings"
)

type User struct {
	ID        int
	room      int
	replyChan chan string
}

var UserList map[net.Conn]User

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

	replyChan := make(chan string)

	//функция для получения запросов от клиента
	go func() {
		// обработка запросов в цикле
		for {
			buf := make([]byte, 128)
			rlen, err := con.Read(buf) // принять запрос

			//проверка на наличие ошибок и верификация подключения
			if err != nil {
				return
			}

			// отправка данных в канал для обработки
			go proceedRequest(string(buf[:rlen]), replyChan)
		}
	}()

	//отправка ответов клиенту
	for {
		reply := <-replyChan
		con.Write([]byte(reply))
	}
}

func proceedRequest(request string, replyChan chan string) {
	switch strings.Split(request, ":")[1] {

	case "CONN":
		replyChan <- "REP:CONN:OK"
		// update user info
		// add user

	case "NEWROOM":
		replyChan <- "REP:NEWROOM:1000"
		// update user info
	}
}
