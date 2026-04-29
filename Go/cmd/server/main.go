package main

import (
	"scheduler/Go/internal/router"
	"fmt"
	"net/http"
)

func main() {
	mainMux := http.NewServeMux()

	mainMux.Handle("/", router.SetupRouter())

	fmt.Println("Server started at :9191")
	if err := http.ListenAndServe(":9191", mainMux); err != nil {
		fmt.Println("Error starting server:", err)
	}
}
