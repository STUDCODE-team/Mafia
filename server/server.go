package main

import (
	// ch "./channels"
	"fmt"
	"net"
	"strings"
)

type User struct {
	ID        string
	room      string
	replyChan chan string
}

var UserList map[string]User = make(map[string]User)

func identifyUser(id string, newReplyChan chan string) {
	user, ok := UserList[id]
	if ok {
		user.replyChan = newReplyChan
		UserList[id] = user
		userBack(id)
	} else {
		UserList[id] = User{ID: id, room: "", replyChan: newReplyChan}
	}
}

func userBack(id string) {
	sendToClient(id, "ALL USER DATA")
	// send all user info to client
}

func sendToClient(id, data string) {
	replyChan := getReplyChan(id)
	replyChan <- data
}

func getReplyChan(id string) chan string {
	user := UserList[id]
	return user.replyChan
}

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
	//
	// defer delete(UserList, "1011377391")
	//

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
			go parseRequest(string(buf[:rlen]), replyChan)
		}
	}()

	//отправка ответов клиенту
	for {
		reply := <-replyChan
		con.Write([]byte(reply + "#"))
	}
}

func parseRequest(request string, replyChan chan string) {
	requestList := strings.Split(request, "#")
	for _, singleRequest := range requestList {
		if singleRequest != "" {
			go proceedRequest(singleRequest, replyChan)
		}
	}
}

func proceedRequest(request string, replyChan chan string) {
	fmt.Println(request)
	switch strings.Split(request, ":")[1] {

	case "CONN":
		replyChan <- "REP:CONN:OK"

	case "DEVICEID":
		go identifyUser(strings.Split(request, ":")[2], replyChan)

	case "NEWROOM":
		replyChan <- "REP:NEWROOM:1000"
		// update user info
	}
}
