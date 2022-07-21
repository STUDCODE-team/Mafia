package main

import (
	"math/rand"
	"net"
	"strconv"
	"strings"
	"time"
	// "fmt"
)

type Room struct {
	ID      string
	players []User
}

var RoomList map[string]Room = make(map[string]Room)

func (room Room) removePlayerByIndex(index int) Room {
    room.players = append(room.players[:index], room.players[index+1:]...)
	return room
}

func (room Room) removePlayerByUser(user User) Room {
	for i, player := range room.players {
		if player.ID == user.ID {
			return room.removePlayerByIndex(i)
		}
	}
	return Room{}
}

//roomID is unique key that belongs to each room
func generateRoomID() string {
	//bind seed to timer
	rand.Seed(time.Now().UnixNano())
	for {
		// generate new roomId until it unique
		id := rand.Intn(9000) + 1000
		_, ok := RoomList[strconv.Itoa(id)]
		if !ok {
			return strconv.Itoa(id)
		}
	}
}

type User struct {
	ID        string
	room      string
	replyChan chan string
}

var UserList map[chan string]User = make(map[chan string]User)

// To identify user we need a unique key.
// Remote addres is bad in case user changes wi-fi point or connection way.
// So every client generate own unique key named deviceID,
// using which we can identify client and connect it back to room
// if for example connection lost
func identifyUser(id string, newReplyChan chan string) {
	for replyChan, user := range UserList {
		// if there was user with same deviceID,
		// it means client has reconnected and we have to
		// change channels by deleting old and creating a new one
		if user.ID == id {
			UserList[newReplyChan] = user
			delete(UserList, replyChan)
			// userBack
			sendUserState(getUserState(newReplyChan))
			return
		}
	}

	// if there wasn't user with same deviceID,
	// it is new user
	UserList[newReplyChan] = User{ID: id, room: "", replyChan: newReplyChan}
}

//This function generate reply form to tell client his status in app
// EXAMPLE 1: possible return REP:STATE:{ROOM::{}}
// means client now not in room
// EXAMPLE 2: possible return REP:STATE:{ROOM:1232:{12,122,223}}
// means client now in room 1232 that full with client 12, 122 and 223
func getUserState(replyChan chan string) (string, chan string) {
	room := UserList[replyChan].room
	if room == "" {
		return "ROOM::{}", replyChan
	}

	status := "ROOM:" + room + ":{"
	for _, usr := range RoomList[room].players {
		status += usr.ID + ","
	}
	return status[0:len(status)-1] + "}", replyChan

}

// This function calls if client had lost connection and then,
//it returned back
//It's supposed to send all info back to client
func sendUserState(state string, replyChan chan string) {
	replyChan <- "REP:STATE:{" + state + "}"
}

func sendUserStateForAll(roomNum string) {
	room := RoomList[roomNum]
	for _, user := range room.players {
		sendUserState(getUserState(user.replyChan))
	}
}

func main() {
	// server creation
	dstream, err := net.Listen("tcp", ":5893")
	if err != nil {
		return
	}
	defer dstream.Close()

	// handle new connections in a loop
	for {
		// accept new connection
		con, err := dstream.Accept()
		if err != nil {
			return
		}
		// procced connection above in separated virtual thread
		go handle(con)
	}
}

func handle(con net.Conn) {
	defer con.Close()
	// create new channel to send replies
	replyChan := make(chan string)
	// get new client requests in loop in new thread
	go func() {
		for {
			buf := make([]byte, 128)
			rlen, err := con.Read(buf) // get request
			//error check
			if err != nil {
				return
			}
			// send request pack to parse it via function
			go parseRequest(string(buf[:rlen]), replyChan)
		}
	}()

	//sending replies to client in the loop
	for {
		reply := <-replyChan
		con.Write([]byte(reply + "#"))
	}
}

//Requests may come together
//so we need to split it to single ones
func parseRequest(request string, replyChan chan string) {
	//requests are separated with '#'
	requestList := strings.Split(request, "#")
	for _, singleRequest := range requestList {
		if singleRequest != "" {
			go proceedRequest(singleRequest, replyChan)
		}
	}
}

func proceedRequest(request string, replyChan chan string) {

	switch strings.Split(request, ":")[1] {
	// client sends connection check requests
	// and it is needed to be answered
	case "CONN":
		replyChan <- "REP:CONN:OK"

	// after every connection client sends devideID key.
	// It's supposed to identify user and recover his
	// session if needed
	case "DEVICEID":
		go identifyUser(strings.Split(request, ":")[2], replyChan)

	// this request sends if client is needed in new room
	// It's supposed to create new room and
	// move current user into that room
	case "NEWROOM":
		roomID := generateRoomID()
		user := UserList[replyChan]
		user.room = roomID
		UserList[replyChan] = user
		RoomList[roomID] = Room{ID: roomID, players: []User{user}}

		sendUserState(getUserState(replyChan))

	//this request sends if client is needed to to exit a room he in
	case "EXITROOM": //?
		// delete user from room players list

		user := UserList[replyChan]
		room := RoomList[user.room]
		RoomList[user.room] = room.removePlayerByUser(user);
		// if room is empty, delete it
		if len(RoomList[user.room].players) == 0 {
			delete(RoomList, user.room)
		} else {
			sendUserStateForAll(user.room)
		}
		// delete room from user's field
		user.room = ""
		UserList[replyChan] = user


		sendUserState(getUserState(replyChan))


	case "CONROOM":
		roomNum := strings.Split(request, ":")[2]
		room, ok := RoomList[roomNum]
		if !ok {
			return
		}
		user := UserList[replyChan]
		user.room = roomNum
		room.players = append(room.players, user)
		UserList[replyChan] = user
		RoomList[roomNum] = room

		sendUserStateForAll(roomNum)

	}

}
