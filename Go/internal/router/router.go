package router

import (
	"scheduler/Go/internal/handler"
	"net/http"
)

func SetupRouter() *http.ServeMux {
	mux := http.NewServeMux()
	mux.HandleFunc("/", handler.SchedulerHandler)
	return mux
}
																							